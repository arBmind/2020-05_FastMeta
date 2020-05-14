#include <cstddef>
#include <utility>

template<class T, size_t S> struct Array {
    T m[S];

    constexpr auto begin() -> T* { return m; }
    constexpr auto begin() const -> const T* { return m; }

    constexpr auto end() -> T* { return m + S; }
    constexpr auto end() const -> const T* { return m + S; }

    constexpr auto size() const -> size_t { return S; }

    constexpr auto amendAt(size_t i) -> T& { return m[i]; }
    constexpr auto at(size_t i) const -> const T& { return m[i]; }
};

template<class T, class... Ts> Array(T, Ts...) -> Array<T, 1 + sizeof...(Ts)>;

constexpr auto alignOffset(size_t align, size_t offset) -> size_t {
    return ((offset + align - 1) / align) * align; //
}

template<class... Ts> //
constexpr auto tupleLayout() -> Array<size_t, sizeof...(Ts)> {

    auto r = Array<size_t, sizeof...(Ts)>{};
    constexpr auto sizes = Array{sizeof(Ts)...};
    constexpr auto aligns = Array{alignof(Ts)...};

    auto c = size_t{};
    for (auto i = 0u; i < sizeof...(Ts); i++) {
        c = alignOffset(aligns.at(i), c);
        r.amendAt(i) = c;
        c += sizes.at(i);
    }
    return r;
}

#ifndef CPPBENCH_N
constexpr std::size_t CPPBENCH_N = 10;
#endif

constexpr auto n_pack = std::make_index_sequence<CPPBENCH_N>();

template<class T, T V> struct StrongConst { T v = V; };

template<std::size_t... Is> //
auto bench_tuple(std::index_sequence<Is...>) {
    ((void)sizeof(StrongConst<std::size_t, Is>), ...);
#ifdef BASELINE
#else
    constexpr auto offsets = tupleLayout<StrongConst<std::size_t, Is>...>();
    (void)offsets;
    //(void)sizeof(std::tuple<StrongConst<std::size_t, Is>...>);
#endif
    return 0;
}

int main() {
    return bench_tuple(n_pack); //
}
