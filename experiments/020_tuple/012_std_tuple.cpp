#include <tuple>
#include <utility>

#ifndef CPPBENCH_N
constexpr size_t CPPBENCH_N = 10;
#endif

constexpr auto n_pack = std::make_index_sequence<CPPBENCH_N>();

template<class T, T V> struct StrongConst { T v = V; };

template<size_t... Is> auto bench_tuple(std::index_sequence<Is...>) {
    ((void)StrongConst<size_t, Is>{}, ...);
#ifdef BASELINE
#else
    (void)std::tuple<StrongConst<size_t, Is>...>{};
#endif
    return 0;
}

int main() { return bench_tuple(n_pack); }
