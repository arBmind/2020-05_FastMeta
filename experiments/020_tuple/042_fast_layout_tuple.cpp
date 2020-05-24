#include "array19/Array.h"
#include "meta19/Index.h"
#include "meta19/Type.h"
#include "meta19/TypeAt.h"
#include <cstddef> // std::byte
#include <memory> // std::destroy_at
#include <new> // std::launder
#include <utility>

using meta19::Index;
using meta19::index_of_map;
using meta19::IndexType;
using meta19::IndexTypeMap;
using meta19::type;
using meta19::Type;
using meta19::TypeAtMap;

namespace details {

using array19::Array;

constexpr auto alignOffset(size_t align, size_t offset) -> size_t { return ((offset + align - 1) / align) * align; }

template<class... Ts> constexpr auto tupleLayout() -> Array<size_t, 1 + sizeof...(Ts)> {

    auto r = Array<size_t, 1 + sizeof...(Ts)>{};
    constexpr auto sizes = Array{sizeof(Ts)...};
    constexpr auto aligns = Array{alignof(Ts)...};

    auto c = size_t{};
    for (auto i = 0u; i < sizeof...(Ts); i++) {
        c = alignOffset(aligns.at(i), c);
        r.amendAt(i) = c;
        c += sizes.at(i);
    }
    r.amendAt(sizeof...(Ts)) = c;
    return r;
}

} // namespace details

template<class... Ts> struct Tuple {
private:
    template<class Is> struct IndexedTuple;

    template<size_t... Is> struct IndexedTuple<std::index_sequence<Is...>> {
        struct Map : IndexType<Is, Ts>... {};
        static constexpr auto offset_array = details::tupleLayout<Ts...>();
        enum : size_t {
            npos = sizeof...(Ts),
            storage_size = offset_array.at(npos),
        };

        alignas(Ts...) std::byte storage[storage_size];

        template<size_t I> using At = TypeAtMap<I, Map>;
        template<class T> static constexpr auto index_of = index_of_map<T, Map>;

        template<size_t I> constexpr auto rawPtrAt() -> void* { return storage + offset_array.at(I); }
        template<size_t I> constexpr auto rawPtrAt() const -> const void* { return storage + offset_array.at(I); }

        template<size_t I> constexpr auto ptrAt() -> At<I>* {
            return std::launder(reinterpret_cast<At<I>*>(rawPtrAt<I>()));
        }
        template<size_t I> constexpr auto ptrAt() const -> const At<I>* {
            return std::launder(reinterpret_cast<const At<I>*>(rawPtrAt<I>()));
        }

        template<class T> constexpr auto ptrOf() -> T* {
            return std::launder(reinterpret_cast<T*>(storage + offset_array.at(index_of<T>)));
        }
        template<class T> constexpr auto ptrOf() const -> const T* {
            return std::launder(reinterpret_cast<const T*>(storage + offset_array.at(index_of<T>)));
        }

        constexpr IndexedTuple() { (new (rawPtrAt<Is>()) Ts(), ...); }
        ~IndexedTuple() { (std::destroy_at(ptrAt<Is>()), ...); }

        constexpr IndexedTuple(const IndexedTuple& o) { (new (rawPtrAt<Is>()) Ts(*o.ptrAt<Is>()), ...); }
        constexpr IndexedTuple(IndexedTuple&& o) { (new (rawPtrAt<Is>()) Ts(std::move(*o.ptrAt<Is>())), ...); }
        constexpr IndexedTuple& operator=(const IndexedTuple& o) { ((*ptrAt<Is>() = *o.ptrAt<Is>()), ...); }
        constexpr IndexedTuple& operator=(IndexedTuple&& o) { ((*ptrAt<Is>() = std::move(*o.ptrAt<Is>())), ...); }

        constexpr IndexedTuple(const Ts&... ts) { (new (rawPtrAt<Is>()) Ts(ts), ...); }
        constexpr IndexedTuple(Ts&&... ts) { (new (rawPtrAt<Is>()) Ts(std::move(ts)), ...); }
    };

    using Indexed = IndexedTuple<std::make_index_sequence<sizeof...(Ts)>>;
    enum : size_t {
        npos = sizeof...(Ts),
    };

private:
    Indexed indexed;

public:
    constexpr Tuple() = default;
    constexpr Tuple(const Ts&... ts)
        : indexed(ts...) {}
    constexpr Tuple(Ts&&... ts)
        : indexed(std::move(ts)...) {}

    template<class... Os> static constexpr auto fromArgs(Os&&... os) -> Tuple {
        auto res = Tuple{};
        ((res.amendOf(type<std::remove_const_t<std::remove_reference_t<Os>>>) = std::forward<Os>(os)), ...);
        return res;
    }
    template<class... Os> static constexpr auto fromTuple(const Tuple<Os...>& o) -> Tuple {
        return fromArgs(o.template of<Os>()...);
    }
    template<class... Os> static constexpr auto fromTuple(Tuple<Os...>&& o) -> Tuple {
        return fromArgs(std::move(o.template amendOf<Os>())...);
    }

    template<class O> static constexpr auto has_type = ((std::is_same_v<O, Ts>) || ...);
    template<class O> static constexpr auto index_of = Indexed::template index_of<O>;
    template<class O> static constexpr auto offset_of = Indexed::offset_array.at(index_of<O>);
    template<size_t I> static constexpr auto offset_at = Indexed::offset_array.at(I);

    template<size_t I> using At = typename Indexed::template At<I>;

    template<size_t I> constexpr auto at(Index<I>* = {}) const& -> const At<I>& {
        static_assert(I < npos);
        return *indexed.template ptrAt<I>();
    }
    template<size_t I> constexpr auto amendAt(Index<I>* = {}) & -> At<I>& {
        static_assert(I < npos);
        return *indexed.template ptrAt<I>();
    }

    template<class O> constexpr auto of(Type<O>* = {}) const& -> const O& {
        static_assert(has_type<O>);
        return *indexed.template ptrOf<O>();
    }
    template<class O> constexpr auto amendOf(Type<O>* = {}) & -> O& {
        static_assert(has_type<O>);
        return *indexed.template ptrOf<O>();
    }

    template<class F> void visitAll(F&& f) const& { (void)((f(*indexed.template ptrOf<Ts>()), ...)); }
    template<class F> void amendAll(F&& f) & { (void)((f(*indexed.template ptrOf<Ts>()), ...)); }
};

template<class... Ts> Tuple(Ts...)->Tuple<Ts...>;

#ifndef CPPBENCH_N
constexpr size_t CPPBENCH_N = 10;
#endif

constexpr auto n_pack = std::make_index_sequence<CPPBENCH_N>();

template<class T, T V> struct StrongConst { T v = V; };

template<size_t... Is> auto bench_tuple(std::index_sequence<Is...>) {
    ((void)StrongConst<size_t, Is>{}, ...);
#ifdef BASELINE
#else
    (void)Tuple<StrongConst<size_t, Is>...>{};
#endif
    return 0;
}

int main() { return bench_tuple(n_pack); }
