#include <utility>

#ifndef N
#    define N 10
#endif

template<class T, std::size_t... Is> constexpr auto bench_impl(std::index_sequence<Is...>) {
    ((void)std::integral_constant<std::size_t, Is>{}, ...);
    return true;
}

template<std::size_t... Is> constexpr auto bench2_impl(std::index_sequence<Is...>) {
    return (bench_impl<std::integral_constant<std::size_t, Is>>(std::make_index_sequence<N>()) || ...);
}

int main() { return bench2_impl(std::make_index_sequence<N>()); }
