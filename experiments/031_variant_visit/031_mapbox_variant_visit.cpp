#include <mapbox/variant.hpp>
#include <utility>

#ifndef CPPBENCH_N
constexpr std::size_t CPPBENCH_N = 10;
#endif

constexpr auto n_pack = std::make_index_sequence<CPPBENCH_N>();

template<class T, T V> struct StrongConst { T v = V; };

template<std::size_t... Is> //
auto bench_tuple(std::index_sequence<Is...>) {
    auto var = mapbox::util::variant<StrongConst<std::size_t, Is>...>{};
#ifdef BASELINE
#else
    var.visit(var, [](const auto&) {});
#endif
    return 0;
}

int main() {
    return bench_tuple(n_pack); //
}
