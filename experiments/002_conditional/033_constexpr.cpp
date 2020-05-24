#include <utility>

namespace stx {

template<class T> auto val() -> T;

template<bool B, class T, class F> constexpr auto conditional_val = &val<T>;
template<class T, class F> constexpr auto conditional_val<false, T, F> = &val<F>;

template<bool B, class T, class F> using conditional_t = decltype(conditional_val<B, T, F>());

} // namespace stx

template<class T, class... Ds> constexpr auto bench_d() {
    (void)((T{} || Ds{}) || ...);
#ifdef BASELINE
    return true;
#else
    return ((stx::conditional_t<false, Ds, T>{} || stx::conditional_t<true, Ds, T>{}) || ...);
#endif
}

template<size_t... TIs, size_t... DIs>
constexpr auto bench_t(std::index_sequence<TIs...>, std::index_sequence<DIs...>) {
    return (bench_d<std::integral_constant<int, TIs>, std::integral_constant<size_t, DIs>...>() || ...);
}

#ifndef CPPBENCH_N
constexpr size_t CPPBENCH_N = 10;
#endif

constexpr auto D = []() -> size_t {
    if (CPPBENCH_N < 500) return 1;
    if (CPPBENCH_N < 10000) return CPPBENCH_N / 100;
    return CPPBENCH_N / 500;
}();
constexpr auto T = CPPBENCH_N / D;
static_assert(T * D == CPPBENCH_N);

int main() { return bench_t(std::make_index_sequence<T>(), std::make_index_sequence<D>()); }
