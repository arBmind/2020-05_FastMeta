#include <utility>

#ifndef N
#   define N 10
#endif

namespace stx {
    template<auto V>
    struct Const;

    template<auto V>
    constexpr Const<V>* const_ = {};
}

template<class T, std::size_t... Is>
constexpr auto bench_impl(std::index_sequence<Is...>) {
    ((void)stx::const_<Is>, ...);
    return true;
}

template<std::size_t... Is>
constexpr auto bench2_impl(std::index_sequence<Is...>) {
    return (bench_impl<stx::Const<Is>>(std::make_index_sequence<N>()) || ...);
}

int main() {
    return bench2_impl(std::make_index_sequence<N>());
}
