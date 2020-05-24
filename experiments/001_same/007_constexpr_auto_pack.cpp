#include <utility>

#ifndef N
#    define N 10
#endif

namespace stx {
template<auto... V> constexpr auto value = {V...};
}

template<auto* T, size_t... Is> constexpr auto bench_impl(std::index_sequence<Is...>) {
    ((void)stx::value<Is>, ...);
    return true;
}

template<size_t... Is> constexpr auto bench2_impl(std::index_sequence<Is...>) {
    return (bench_impl<&stx::value<Is>>(std::make_index_sequence<N>()) || ...);
}

int main() { return bench2_impl(std::make_index_sequence<N>()); }
