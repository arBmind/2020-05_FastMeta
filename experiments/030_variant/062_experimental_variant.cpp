#include "meta19/Type.h"
#include "meta19/TypeAt.h"
#include "meta19/index_pack.h"
#include "meta19/type_pack_front_type.h"

#include <memory>
#include <new>
#include <utility>

using meta19::FrontType;
using meta19::index_pack;
using meta19::IndexPack;
using meta19::IndexTypeMap;
using meta19::map_index_of;
using meta19::Type;

template<size_t... Is> //
constexpr auto constexpr_max_of() {
    auto r = size_t{};
    for (auto v : {Is...}) {
        if (v > r) r = v;
    }
    return r;
}

template<size_t N> //
constexpr auto selectVariantWhichValue() {
    if constexpr (N <= std::numeric_limits<uint8_t>::max()) {
        return uint8_t{};
    }
    else if constexpr (N <= std::numeric_limits<uint16_t>::max()) {
        return uint16_t{};
    }
    else if constexpr (N <= std::numeric_limits<uint32_t>::max()) {
        return uint32_t{};
    }
    else if constexpr (N <= std::numeric_limits<uint64_t>::max()) {
        return uint64_t{};
    }
    // fail …
}

namespace details {

template<class T, class... Ts, class V, class F, size_t I, size_t... Is>
constexpr auto visitRecursive(V&& v, F&& f, IndexPack<I, Is...>*) -> decltype(auto) {
    if (I == v.which) {
        return f(*v.template asPtr<T>());
    }
    if constexpr (0 != sizeof...(Ts) && 0 != sizeof...(Is)) {
        return visitRecursive<Ts...>(std::forward<V>(v), std::forward<F>(f), index_pack<Is...>);
    }
    else {
        // UNREACHABLE();
    }
}

template<class T, class... Ts, class V, class F, size_t I, size_t... Is>
constexpr auto amendVisitRecursive(V&& v, F&& f, IndexPack<I, Is...>*) -> decltype(auto) {
    if (I == v.which) {
        return f(*v.template amendAsPtr<T>());
    }
    if constexpr (0 != sizeof...(Ts) && 0 != sizeof...(Is)) {
        return visitRecursive<Ts...>(std::forward<V>(v), std::forward<F>(f), index_pack<Is...>);
    }
    else {
        // UNREACHABLE();
    }
}

} // namespace details

template<class... Ts> //
struct VariantWhich {
    using Value = decltype(selectVariantWhichValue<sizeof...(Ts)>());
    using Map = IndexTypeMap<Ts...>;

    constexpr VariantWhich() = default;
    explicit constexpr VariantWhich(Value v)
        : m(v) {}

    constexpr operator Value() const { return m; }

    constexpr bool operator==(VariantWhich o) const { return o.m == m; }
    constexpr bool operator!=(VariantWhich o) const { return o.m != m; }

    template<class T> //
    static constexpr auto of(Type<T> = {}) -> VariantWhich {
        return VariantWhich{static_cast<Value>(map_index_of<T, Map>)};
    }

    template<class T> constexpr bool operator==(Type<T>) const { return map_index_of<T, Map> == m; }
    template<class T> constexpr bool operator!=(Type<T>) const { return map_index_of<T, Map> != m; }

private:
    Value m{};
};

template<class... Ts> //
struct Variant {
    using Map = IndexTypeMap<Ts...>;
    using Which = VariantWhich<Ts...>;
    enum { npos = sizeof...(Ts) }; // invalid state after exception - only destruction checks!

private:
    template<class> struct IndexedVariant;

    template<size_t... Is> struct IndexedVariant<std::index_sequence<Is...>> {
        enum : size_t {
            storage_size = constexpr_max_of<sizeof(Ts)...>(),
        };
        using First = FrontType<Ts...>;
        using WhichValue = typename Which::Value;

        WhichValue which{npos};
        alignas(Ts...) std::byte storage[storage_size];

        constexpr IndexedVariant() {
            constructAs<First>();
            which = 0; // only initialize once constuction was successful
        }
        ~IndexedVariant() {
            if (which == npos) return;
            destruct();
        }

        constexpr IndexedVariant(const IndexedVariant& o) {
            (((Is == o.which ? (constructAs<Ts>(*o.asPtr<Ts>()), 0) : 0), ...));
            which = o.which;
        }
        constexpr auto operator=(const IndexedVariant& o) -> IndexedVariant& {
            if (which == o.which) {
                (((Is == o.which ? (*amendAsPtr<Ts>() = *o.asPtr<Ts>(), 0) : 0), ...));
            }
            else {
                destruct();
                which = npos;
                (((Is == o.which ? (constructAs<Ts>(*o.asPtr<Ts>()), 0) : 0), ...));
                which = o.which;
            }
            return *this;
        }

        constexpr IndexedVariant(IndexedVariant&& o) {
            (((Is == o.which ? (constructAs<Ts>(std::move(*o.amendAsPtr<Ts>())), 0) : 0), ...));
            which = o.which; // o.which is needed for destruction!
        }
        constexpr auto operator=(IndexedVariant&& o) -> IndexedVariant& {
            if (which == o.which) {
                (((Is == o.which ? (*amendAsPtr<Ts>() = std::move(*o.amendAsPtr<Ts>()), 0) : 0), ...));
            }
            else {
                destruct();
                which = npos;
                (((Is == o.which ? (constructAs<Ts>(std::move(*o.amendAsPtr<Ts>())), 0) : 0), ...));
                which = o.which; // o.which is needed for destruction!
            }
            return *this;
        }

        template<class T> constexpr auto asPtr() const -> const T* {
            return std::launder(reinterpret_cast<const T*>(&storage));
        }
        template<class T> constexpr auto amendAsPtr() -> T* {
            return std::launder(reinterpret_cast<T*>(&storage)); //
        }

        template<class T, class... Args> //
        constexpr auto constructAs(Args&&... args) {
            new (&storage) T(std::forward<Args>(args)...);
        }
        template<class... Args> //
        constexpr auto constructWhich(WhichValue w, Args&&... args) {
            (((Is == w ? (constructAs<Ts>(std::forward<Args>(args)...), 0) : 0), ...));
        }

        constexpr void destruct() {
            (void)((Is == which ? (std::destroy_at(amendAsPtr<Ts>()), true) : false) || ...); //
        }

        template<class F> //
        constexpr auto visitImpl(F&& f) const -> decltype(auto) {
            if constexpr (std::is_same_v<void, decltype(f(first()))>) {
                (void)(((Is == which ? (f(*asPtr<Ts>()), true) : false) || ...));
            }
            else {
                return details::visitRecursive<Ts...>(*this, std::forward<F>(f), index_pack<Is...>);
            }
        }

        template<class F> //
        constexpr auto amendVisitImpl(F&& f) -> decltype(auto) {
            if constexpr (std::is_same_v<void, decltype(f(first()))>) {
                (void)(((Is == which ? (f(*amendAsPtr<Ts>()), true) : false) || ...));
            }
            else {
                return details::amendVisitRecursive<Ts...>(*this, std::forward<F>(f), index_pack<Is...>);
            }
        }

        auto first() -> First&;
        auto first() const -> const First&;
    };
    using Indexed = IndexedVariant<std::make_index_sequence<sizeof...(Ts)>>;

private:
    Indexed indexed;

public:
    constexpr Variant() = default;

    /// construct from move/copy
    template<
        class T,
        class BT = std::remove_cv_t<std::remove_reference_t<T>>,
        class = std::enable_if_t<(!std::is_base_of_v<Variant, BT>)>>
    Variant(T&& t) {
        //        static_assert(containsOf<BT>(pack), "type not part of variant");
        indexed.template constructAs<BT>(std::forward<T>(t));
        indexed.which = map_index_of<BT, Map>;
    }

    constexpr auto which() const -> Which { return Which{indexed.which}; }

    template<class F> constexpr auto visit(F&& f) const -> decltype(auto) {
        return indexed.visitImpl(std::forward<F>(f));
    }
    template<class F> constexpr auto amendVisit(F&& f) -> decltype(auto) {
        return indexed.amendVisitImpl(std::forward<F>(f));
    }
};

#ifndef CPPBENCH_N
constexpr std::size_t CPPBENCH_N = 10;
#endif

constexpr auto n_pack = std::make_index_sequence<CPPBENCH_N>();

template<class T, T V> struct StrongConst { T v = V; };

template<std::size_t... Is> //
auto bench_tuple(std::index_sequence<Is...>) {
    ((void)StrongConst<std::size_t, Is>{}, ...);
#ifdef BASELINE
#else
    (void)Variant<StrongConst<std::size_t, Is>...>{};
#endif
    return 0;
}

int main() {
    return bench_tuple(n_pack); //
}
