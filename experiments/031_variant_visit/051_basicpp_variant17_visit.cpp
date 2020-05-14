#include <utility>
#include <variant17/Variant.h>

#ifndef CPPBENCH_N
constexpr std::size_t CPPBENCH_N = 10;
#endif

constexpr auto n_pack = std::make_index_sequence<CPPBENCH_N>();

template<class T, T V> struct StrongConst { T v = V; };

template<std::size_t... Is> //
auto bench_tuple(std::index_sequence<Is...>) {
    auto var = variant17::Variant<StrongConst<std::size_t, Is>...>{};
#ifdef BASELINE
#else
    var.visit([](const auto&) {});
#endif
    return 0;
}

int main() {
    return bench_tuple(n_pack); //
}
