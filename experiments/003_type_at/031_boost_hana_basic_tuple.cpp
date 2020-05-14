#include <boost/hana/basic_tuple.hpp>
#include <utility>

template<size_t I, class... Ts> //
using TypeAt = std::decay_t<decltype(boost::hana::at_c<I>(std::declval<boost::hana::basic_tuple<Ts...>>()))>;

static_assert(std::is_same_v<int, TypeAt<1, char, int, float>>);

#ifndef CPPBENCH_N
constexpr std::size_t CPPBENCH_N = 10;
#endif

constexpr auto n_pack = std::make_index_sequence<CPPBENCH_N>();

template<std::size_t... Is> //
auto bench_vec(std::index_sequence<Is...>) {
    (void)(sizeof(std::integral_constant<std::size_t, Is>) + ...);
#ifdef BASELINE
#else
    using T = boost::hana::basic_tuple<std::integral_constant<std::size_t, Is>...>;
    (((void)sizeof(decltype(boost::hana::at_c<Is>(std::declval<T>()))), ...));
    // (((void)sizeof(TypeAt<Is, std::integral_constant<std::size_t, Is>...>), ...));
#endif
    return 0;
}

int main() {
    return bench_vec(n_pack); //
}
