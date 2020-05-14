#include <utility>

namespace stx {

template<class, class> struct is_same : std::false_type {};

template<class T> struct is_same<T, T> : std::true_type {};

template<typename T, typename U> constexpr bool is_same_v = is_same<T, U>::value;

} // namespace stx

#ifndef CPPBENCH_N
constexpr std::size_t CPPBENCH_N = 10;
#endif

constexpr auto D = []() -> std::size_t {
    if (CPPBENCH_N < 500) return 1;
    if (CPPBENCH_N < 10000) return CPPBENCH_N / 100;
    return CPPBENCH_N / 500;
}();
constexpr std::size_t T = CPPBENCH_N / D;
static_assert(T * D == CPPBENCH_N);

constexpr auto d_pack = std::make_index_sequence<D>();
constexpr auto t_pack = std::make_index_sequence<T>();

template<class T, std::size_t... Is> //
constexpr auto bench_d(std::index_sequence<Is...>) {
    (void)(sizeof(std::integral_constant<std::size_t, Is>) + ...);
#ifdef BASELINE
    return true;
#else
    return (stx::is_same_v<std::integral_constant<std::size_t, Is>, T> || ...);
#endif
}

template<std::size_t... Is> //
constexpr auto bench_t(std::index_sequence<Is...>) {
    return (bench_d<std::integral_constant<int, Is>>(d_pack) || ...);
}

int main() {
    return bench_t(t_pack); //
}
