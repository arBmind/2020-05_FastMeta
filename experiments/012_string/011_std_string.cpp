#include <string>
#include <utility>

#ifndef CPPBENCH_N
constexpr std::size_t CPPBENCH_N = 10;
#endif

constexpr auto n_pack = std::make_index_sequence<CPPBENCH_N>();

template<std::size_t... Is> //
auto bench_vec(std::index_sequence<Is...>) {
    (void)(std::integral_constant<std::size_t, Is>{} + ...);
#ifdef BASELINE
#else
    (void)(((std::string{}, std::integral_constant<std::size_t, Is>{}), ...));
#endif
    return 0;
}

int main() {
    return bench_vec(n_pack); //
}
