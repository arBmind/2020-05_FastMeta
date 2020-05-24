#include <utility>

namespace stx {

// after implementaton by Chiel Douwes (po2_metalib from 2017)
// see: https://github.com/chieltbest/po2_metalib/blob/master/src/lib.hpp#L33
template<bool> struct conditional;

template<> struct conditional<true> { template<class T, class> using type = T; };
template<> struct conditional<false> { template<class, class T> using type = T; };

template<bool C, class T, class F> using conditional_t = typename conditional<C>::template type<T, F>;

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
