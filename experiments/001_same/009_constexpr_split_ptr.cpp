#include <utility>

#ifndef N
#    define N 10
#endif

namespace stx {
template<class T, T> struct Const;

template<class T, T V> constexpr Const<T, V>* const_ = {};
} // namespace stx

template<class T, size_t... Is> constexpr auto bench_impl(std::index_sequence<Is...>) {
    ((void)stx::const_<size_t, Is>, ...);
    return true;
}

template<size_t... Is> constexpr auto bench2_impl(std::index_sequence<Is...>) {
    return (bench_impl<stx::Const<size_t, Is>>(std::make_index_sequence<N>()) || ...);
}

int main() { return bench2_impl(std::make_index_sequence<N>()); }
