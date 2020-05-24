#include <cstddef>
#include <utility>

template<size_t I, size_t... Is> constexpr auto maxOf() {
    auto r = I;
    (void)(((Is > r ? r = Is : 0), ...));
    return r;
}

#ifndef CPPBENCH_N
constexpr size_t CPPBENCH_N = 10;
#endif

constexpr auto n_pack = std::make_index_sequence<CPPBENCH_N>();

template<class T, T V> struct StrongConst { T v = V; };

template<size_t... Is> auto bench_tuple(std::index_sequence<Is...>) {
#ifdef BASELINE
#else
    constexpr auto max1 = maxOf<(10 + Is)...>();
    (void)max1;
    constexpr auto max2 = maxOf<(2000 - Is)...>();
    (void)max2;
    constexpr auto max3 = maxOf<(30 + Is)...>();
    (void)max3;
    constexpr auto max4 = maxOf<(4000 - Is)...>();
    (void)max4;
    constexpr auto max5 = maxOf<(50 + Is)...>();
    (void)max5;
    constexpr auto max6 = maxOf<(6000 - Is)...>();
    (void)max6;
    constexpr auto max7 = maxOf<(70 + Is)...>();
    (void)max7;
    constexpr auto max8 = maxOf<(8000 - Is)...>();
    (void)max8;
    constexpr auto max9 = maxOf<(90 + Is)...>();
    (void)max9;
    constexpr auto max10 = maxOf<(10000 - Is)...>();
    (void)max10;

#endif
    return 0;
}

int main() { return bench_tuple(n_pack); }
