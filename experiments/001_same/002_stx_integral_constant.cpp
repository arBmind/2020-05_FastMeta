#include <utility>

#ifndef N
#   define N 10
#endif

namespace stx {

template<class _Tp, _Tp _v>
struct integral_constant {
    static constexpr const _Tp value = _v;
    typedef _Tp value_type;
    typedef integral_constant type;

    constexpr operator value_type() const noexcept { return value; }
    constexpr value_type operator() () const noexcept { return value; }
};

template <class _Tp, _Tp _v>
constexpr const _Tp integral_constant<_Tp, _v>::value;

template <bool _b>
using bool_constant = integral_constant<bool, _b>;

typedef bool_constant<true> true_type;
typedef bool_constant<false> false_type;

}

template<class T, std::size_t... Is>
constexpr auto bench_impl(std::index_sequence<Is...>) {
    ((void)stx::integral_constant<std::size_t, Is>{}, ...);
    return true;
}

template<std::size_t... Is>
constexpr auto bench2_impl(std::index_sequence<Is...>) {
    return (bench_impl<stx::integral_constant<std::size_t, Is>>(std::make_index_sequence<N>()) || ...);
}

int main() {
    return bench2_impl(std::make_index_sequence<N>());
}
