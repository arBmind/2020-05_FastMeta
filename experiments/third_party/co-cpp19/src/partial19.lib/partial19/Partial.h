#pragma once
#include "Bitset.h"
#include "align.h"
#include "array19/SliceOf.carray.h"
#include "array19/SliceOf.max.h"
#include "meta19/RemoveReference.h"
#include "meta19/TypeAt.h"
#include "meta19/TypePack.h"
#include "meta19/isSame.h"

#include <new> // std::launder
#include <stdint.h> // uint8_t
#include <stdlib.h> // malloc

namespace partial19 {

using array19::sliceMaximum;
using array19::sliceOfCArray;
using meta19::Index;
using meta19::index_of_map;
using meta19::IndexTypeMap;
using meta19::isSame;
using meta19::StoredOf;
using meta19::Type;
using meta19::type;
using meta19::TypeAtMap;
using meta19::TypePack;

namespace details {

template<bool> struct ConditionalPlacementNew;
template<> struct ConditionalPlacementNew<true> {
    template<class T> static auto apply(T&& v, void* ptr) { new (ptr) T((T &&) v); }
};
template<> struct ConditionalPlacementNew<false> {
    static auto apply(...) {}
};

} // namespace details

template<class... Ts> //
struct PartialWhich {
    using Bits = Bitset<sizeof...(Ts)>;
    using Map = IndexTypeMap<Ts...>;

    constexpr PartialWhich() = default;
    explicit constexpr PartialWhich(Bits v) : m(v) {}

    template<class... Ws, template<class...> class Tpl = TypePack>
    constexpr static auto makeFor(Tpl<Ws...>* = {}) -> PartialWhich {
        auto bits = Bits{};
        (bits.setAt(index_of_map<Ws, Map>), ...);
        return PartialWhich{bits};
    }

    constexpr operator Bits() const { return m; }

    constexpr bool operator==(PartialWhich o) const { return o.m == m; }
    constexpr bool operator!=(PartialWhich o) const { return o.m != m; }

    constexpr bool at(size_t i) const { return m.at(i); }

    template<class T> constexpr bool of(Type<T>* = {}) const { return m.at(index_of_map<T, Map>); }

private:
    Bits m{};
};

/// Container that can store zero or one of each given type.
/// Storage is dynamically allocated on construction.
/// Changing stored types during live time is not supported.
///
/// Preconditions:
/// • Each type can only be placed once in this Partial implementation!
template<class... Ts> struct Partial {
    using Which = PartialWhich<Ts...>;
    using Bits = typename Which::Bits;
    using Map = typename Which::Map;
    static constexpr size_t sizeof_ts[] = {sizeof(Ts)...};
    static constexpr size_t alignof_ts[] = {alignof(Ts)...};
    enum : size_t { count = sizeof...(Ts), max_align = sliceMaximum(sliceOfCArray(alignof_ts)) };

private:
    Bits m_bits{};
    uint8_t* m_pointer{};

public:
    Partial() noexcept = default;
    ~Partial() { destructAll(); }

    // move
    Partial(Partial&& o) noexcept : m_bits(o.m_bits), m_pointer(o.m_pointer) {
        o.m_bits.reset();
        o.m_pointer = nullptr;
    }
    Partial& operator=(Partial&& o) noexcept {
        destructAll();
        m_bits = o.m_bits;
        m_pointer = o.m_pointer;
        o.m_bits.reset();
        o.m_pointer = nullptr;
        return *this;
    }

    // copy
    Partial(const Partial& o) {
        auto hasValue = [&](auto i) { return o.m_bits.at(i); };
        auto factory = [&]<class T>(Type<T>*, void* ptr) { new (ptr) T(o.of<T>()); };
        *this = fromFactory(hasValue, factory);
    }
    auto operator=(const Partial& o) -> Partial& {
        auto hasValue = [&](auto i) { return o.m_bits.at(i); };
        auto factory = [&]<class T>(Type<T>*, void* ptr) { new (ptr) T(o.of<T>()); };
        *this = fromFactory(hasValue, factory);
        return *this;
    }

    // Fastest way to construct Partial
    //
    // functor: hasValue(size_t index) -> bool - is called twice and has to return same decision!
    // functor: factory(Type<T>*, void* ptr) -> void - should placement new (ptr) T(…)
    template<class HasValue, class Factory> static auto fromFactory(HasValue&& hasValue, Factory&& factory) {
        auto r = Partial{};
        auto size = storageSize(hasValue);
        if (!size) return r;
        if constexpr (alignof(std::max_align_t) < max_align) size += max_align - alignof(std::max_align_t);
        r.m_pointer = static_cast<uint8_t*>(malloc(size));
        r.constructAll(hasValue, factory);
        return r;
    }

    // Simple way to construct from given arguments
    // StoredOf<Vs>... has to be a subset of Ts...
    template<class... Vs> static auto fromArgs(Vs&&... vs) -> Partial {
        auto bits = Bits{};
        (bits.setAt(index_of_map<StoredOf<Vs>, Map>), ...);
        auto hasValue = [&](size_t i) {
            return bits.at(i); //
        };
        auto factory = [&]<class T>(Type<T>*, void* ptr) {
            ((details::ConditionalPlacementNew<isSame(&type<T>, &type<StoredOf<Vs>>)>::apply((Vs &&) vs, ptr), 0),
             ...); //
        };
        return fromFactory(hasValue, factory);
    }

    [[nodiscard]] constexpr auto which() const -> Which { return Which{m_bits}; }

    // returns the value stored as type Ts...[I]
    // precondition: which().at(I) == true
    template<size_t I>[[nodiscard]] auto at(Index<I>* = {}) const -> const TypeAtMap<I, Map>& {
        using T = TypeAtMap<I, Map>;
        return *std::launder(reinterpret_cast<const T*>(alignPointer(max_align, m_pointer) + offsetAt(I)));
    }
    template<size_t I>[[nodiscard]] auto amendAt(Index<I>* = {}) -> TypeAtMap<I, Map>& {
        using T = TypeAtMap<I, Map>;
        return *std::launder(reinterpret_cast<T*>(alignPointer(max_align, m_pointer) + offsetAt(I)));
    }

    // returns the value of type T
    // precondition: which().of<T>() == true
    template<class T>[[nodiscard]] auto of(Type<T>* = {}) const -> const T& {
        constexpr size_t I = index_of_map<T, Map>;
        return *std::launder(reinterpret_cast<const T*>(alignPointer(max_align, m_pointer) + offsetAt(I)));
    }
    template<class T>[[nodiscard]] auto amendOf(Type<T>* = {}) -> T& {
        constexpr size_t I = index_of_map<T, Map>;
        return *std::launder(reinterpret_cast<T*>(alignPointer(max_align, m_pointer) + offsetAt(I)));
    }

    // visit all initialized types stored in this Partial
    // functor: f(const auto&) - called with every actually stored type
    template<class F> auto visitInitialized(F&& f) const {
        auto i = 0u;
        auto offset = size_t{};
        auto ptr = alignPointer(max_align, m_pointer);
        ((m_bits.at(i) ? (offset = alignOffset(alignof_ts[i], offset),
                          f(*std::launder(reinterpret_cast<const Ts*>(ptr + offset))),
                          offset += sizeof_ts[i],
                          ++i)
                       : ++i),
         ...);
    }
    template<class F> auto amendVisitInitialized(F&& f) {
        auto i = 0u;
        auto offset = size_t{};
        auto ptr = alignPointer(max_align, m_pointer);
        ((m_bits.at(i) ? (offset = alignOffset(alignof_ts[i], offset),
                          f(*std::launder(reinterpret_cast<Ts*>(ptr + offset))),
                          offset += sizeof_ts[i],
                          ++i)
                       : ++i),
         ...);
    }

private:
    void destructAll() {
        amendVisitInitialized([]<class T>(T& v) { v.~T(); });
        free(m_pointer);
    }

    template<class HasValue, class Factory> auto constructAll(HasValue&& hasValue, Factory&& factory) {
        auto i = 0u;
        auto offset = size_t{};
        auto ptr = alignPointer(max_align, m_pointer);
        ((hasValue(i) ? (offset = alignOffset(alignof_ts[i], offset),
                         factory(type<Ts>, ptr + offset),
                         m_bits.setAt(i),
                         offset += sizeof_ts[i],
                         ++i)
                      : ++i),
         ...);
    }

    [[nodiscard]] auto offsetAt(size_t index) const {
        auto offset = size_t{};
        for (auto i = 0u; i < index; i++)
            if (m_bits.at(i)) offset = alignOffset(alignof_ts[i], offset) + sizeof_ts[i];
        return alignOffset(alignof_ts[index], offset);
    }

    template<class HasValue> static constexpr auto storageSize(HasValue&& hasValue) {
        auto size = size_t{};
        for (auto i = 0u; i < count; i++)
            if (hasValue(i)) size = alignOffset(alignof_ts[i], size) + sizeof_ts[i];
        return size;
    }
};

} // namespace partial19
