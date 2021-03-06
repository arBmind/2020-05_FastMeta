#include "meta17/TypePack.wrap.h"
#include "meta17/align.h"
#include <utility>

#ifndef CPPBENCH_N
constexpr size_t CPPBENCH_N = 10;
#endif

constexpr auto n_pack = std::make_index_sequence<CPPBENCH_N>();

template<class T, T V> struct StrongConst { T v = V; };

template<size_t... Is> auto bench_tuple(std::index_sequence<Is...>) {
    ((void)sizeof(StrongConst<size_t, Is>), ...);
#ifdef BASELINE
#else
    constexpr auto pack = meta17::to_type_pack<StrongConst<size_t, Is>...>;
    constexpr auto offsets = meta17::alignTypePack(pack);
    (void)offsets;
    //(void)sizeof(std::tuple<StrongConst<size_t, Is>...>);
#endif
    return 0;
}

int main() { return bench_tuple(n_pack); }
