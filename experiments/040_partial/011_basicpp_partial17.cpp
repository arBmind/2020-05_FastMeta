#include <partial17/Partial.h>
#include <utility>

#ifndef CPPBENCH_N
constexpr size_t CPPBENCH_N = 10;
#endif

constexpr auto n_pack = std::make_index_sequence<CPPBENCH_N>();

template<class T, T V> struct StrongConst { T v = V; };

template<size_t I, size_t... Is> //
auto bench_tuple(std::index_sequence<I, Is...>) {
    ((void)StrongConst<size_t, I>{}, ((void)StrongConst<size_t, Is>{}, ...));
#ifdef BASELINE
#else
    (void)partial17::Partial<StrongConst<size_t, I>, StrongConst<size_t, Is>...>{StrongConst<size_t, I>{}};
#endif
    return 0;
}

int main() {
    return bench_tuple(n_pack); //
}
