#include <EASTL/tuple.h>
#include <utility>

#ifndef CPPBENCH_N
constexpr size_t CPPBENCH_N = 10;
#endif

constexpr auto n_pack = std::make_index_sequence<CPPBENCH_N>();

template<class T, T V> struct StrongConst { T v = V; };

template<size_t... Is> auto bench_tuple(std::index_sequence<Is...>) {
    auto tpl = eastl::tuple<StrongConst<size_t, Is>...>{};
#ifdef BASELINE
#else
    (void)eastl::apply(
        [](const auto&... v) {
            auto l = [](const auto& v) {};
            (void)(((l(v), 0), ...));
        },
        tpl);
#endif
    return 0;
}

int main() { return bench_tuple(n_pack); }
