#include <map>
#include <utility>

#ifndef CPPBENCH_N
constexpr size_t CPPBENCH_N = 10;
#endif

constexpr auto n_pack = std::make_index_sequence<CPPBENCH_N>();

template<size_t... Is> auto bench_vec(std::index_sequence<Is...>) {
    (void)(sizeof(std::integral_constant<size_t, Is>) + ...);
#ifdef BASELINE
#else
    (((void)sizeof(std::map<std::integral_constant<size_t, Is>, int>), ...));
#endif
    return 0;
}

int main() { return bench_vec(n_pack); }
