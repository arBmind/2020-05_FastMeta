#include <cstddef>
#include <utility>

template<size_t... _Vals> struct Maximum;

template<> struct Maximum<> : std::integral_constant<size_t, 0> {}; // maximum of nothing is 0

template<size_t _Val> struct Maximum<_Val> : std::integral_constant<size_t, _Val> {}; // maximum of _Val is _Val

template<size_t _First, size_t _Second, size_t... _Rest> struct Maximum<_First, _Second, _Rest...>
    : Maximum<(_First < _Second ? _Second : _First), _Rest...>::type {
    // find maximum value in _First, _Second, _Rest...
};

#ifndef CPPBENCH_N
constexpr std::size_t CPPBENCH_N = 10;
#endif

constexpr auto n_pack = std::make_index_sequence<CPPBENCH_N>();

template<class T, T V> struct StrongConst { T v = V; };

template<std::size_t... Is> //
auto bench_tuple(std::index_sequence<Is...>) {
    ((void)sizeof(StrongConst<std::size_t, Is>), ...);
#ifdef BASELINE
#else
    (void)Maximum<(10 + Is)...>::value;
    (void)Maximum<(2000 - Is)...>::value;
    (void)Maximum<(30 + Is)...>::value;
    (void)Maximum<(4000 - Is)...>::value;
    (void)Maximum<(50 + Is)...>::value;
    (void)Maximum<(6000 - Is)...>::value;
    (void)Maximum<(70 + Is)...>::value;
    (void)Maximum<(8000 - Is)...>::value;
    (void)Maximum<(90 + Is)...>::value;
    (void)Maximum<(10000 - Is)...>::value;
#endif
    return 0;
}

int main() {
    return bench_tuple(n_pack); //
}
