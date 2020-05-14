#include <cstddef>
#include <utility>

template<class T> using ArrayOf = T[];

template<class T> struct Slice {
    T* b{};
    T* e{};

    template<size_t N> constexpr Slice(T (&v)[N])
        : b(v)
        , e(v + N) {}

    constexpr auto begin() const { return b; }
    constexpr auto end() const { return e; }
};

constexpr auto maxOf(Slice<const size_t> s) {
    auto r = size_t{};
    for (auto v : s)
        if (v > r) r = v;
    return r;
}

#ifndef CPPBENCH_N
constexpr std::size_t CPPBENCH_N = 10;
#endif

constexpr auto n_pack = std::make_index_sequence<CPPBENCH_N>();

template<class T, T V> struct StrongConst { T v = V; };

template<std::size_t... Is> //
auto bench_tuple(std::index_sequence<Is...>) {
#ifdef BASELINE
#else
    constexpr size_t a1[] = {(10 + Is)...};
    constexpr size_t a2[] = {(2000 - Is)...};
    constexpr size_t a3[] = {(30 + Is)...};
    constexpr size_t a4[] = {(4000 - Is)...};
    constexpr size_t a5[] = {(50 + Is)...};
    constexpr size_t a6[] = {(6000 - Is)...};
    constexpr size_t a7[] = {(70 + Is)...};
    constexpr size_t a8[] = {(8000 - Is)...};
    constexpr size_t a9[] = {(90 + Is)...};
    constexpr size_t a10[] = {(10000 - Is)...};

    constexpr auto max1 = maxOf(a1);
    (void)max1;
    constexpr auto max2 = maxOf(a2);
    (void)max2;
    constexpr auto max3 = maxOf(a3);
    (void)max3;
    constexpr auto max4 = maxOf(a4);
    (void)max4;
    constexpr auto max5 = maxOf(a5);
    (void)max5;
    constexpr auto max6 = maxOf(a6);
    (void)max6;
    constexpr auto max7 = maxOf(a7);
    (void)max7;
    constexpr auto max8 = maxOf(a8);
    (void)max8;
    constexpr auto max9 = maxOf(a9);
    (void)max9;
    constexpr auto max10 = maxOf(a10);
    (void)max10;
#endif
    return 0;
}

int main() {
    return bench_tuple(n_pack); //
}
