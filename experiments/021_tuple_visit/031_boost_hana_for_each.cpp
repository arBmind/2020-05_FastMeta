#include <boost/hana/for_each.hpp>
#include <boost/hana/tuple.hpp>
#include <utility>

#ifndef CPPBENCH_N
constexpr std::size_t CPPBENCH_N = 10;
#endif

constexpr auto n_pack = std::make_index_sequence<CPPBENCH_N>();

template<class T, T V> struct StrongConst { T v = V; };

template<std::size_t... Is> //
auto bench_tuple(std::index_sequence<Is...>) {
    auto tpl = boost::hana::tuple<StrongConst<std::size_t, Is>...>{};
#ifdef BASELINE
#else
    (void)boost::hana::for_each(tpl, [](const auto& v) {});
#endif
    return 0;
}

int main() {
    return bench_tuple(n_pack); //
}
