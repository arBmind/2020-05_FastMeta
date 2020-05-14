#include <type_traits>
#include <utility>

template<class T, class... Ds> //
constexpr auto bench_d() {
    (void)((T{} || Ds{}) || ...);
#ifdef BASELINE
    return true;
#else
    return ((typename std::conditional<false, Ds, T>::type{} || typename std::conditional<true, Ds, T>::type{}) || ...);
#endif
}

template<std::size_t... TIs, std::size_t... DIs> //
constexpr auto bench_t(std::index_sequence<TIs...>, std::index_sequence<DIs...>) {
    return (bench_d<std::integral_constant<int, TIs>, std::integral_constant<std::size_t, DIs>...>() || ...);
}

#ifndef CPPBENCH_N
constexpr std::size_t CPPBENCH_N = 10;
#endif

constexpr auto D = []() -> std::size_t {
    if (CPPBENCH_N < 500) return 1;
    if (CPPBENCH_N < 10000) return CPPBENCH_N / 100;
    return CPPBENCH_N / 500;
}();
constexpr auto T = CPPBENCH_N / D;
static_assert(T * D == CPPBENCH_N);

int main() {
    return bench_t(std::make_index_sequence<T>(), std::make_index_sequence<D>()); //
}
