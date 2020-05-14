#include <cstddef>
#include <utility>

template<class T> inline constexpr //
    T* nullptr_to = nullptr;
template<class T> inline constexpr //
    T* nullptr_to<T&> = nullptr;

template<size_t, class> struct IndexType {};

template<class IP, class... Ts> struct IndexTypeCombo;
template<size_t... Is, class... Ts> struct IndexTypeCombo<std::index_sequence<Is...>, Ts...> : IndexType<Is, Ts>... {};

template<size_t I, class T> T typeAtImpl(IndexType<I, T>*);

template<size_t I, class... Ts> //
using TypeAt = decltype(typeAtImpl<I>(nullptr_to<IndexTypeCombo<std::make_index_sequence<sizeof...(Ts)>, Ts...>>));

template<size_t I, class TC> //
using TypeComboAt = decltype(typeAtImpl<I>(nullptr_to<TC>));

static_assert(std::is_same_v<int, TypeAt<1, char, int, float>>);

#ifndef CPPBENCH_N
constexpr std::size_t CPPBENCH_N = 10;
#endif

constexpr auto n_pack = std::make_index_sequence<CPPBENCH_N>();

template<std::size_t... Is> //
auto bench_vec(std::index_sequence<Is...>) {
    (void)(sizeof(std::integral_constant<std::size_t, Is>) + ...);
#ifdef BASELINE
#else
    using Combo = IndexTypeCombo<std::make_index_sequence<sizeof...(Is)>, std::integral_constant<std::size_t, Is>...>;
    (((void)sizeof(TypeComboAt<Is, Combo>), ...));
    // (((void)sizeof(TypeAt<Is, std::integral_constant<std::size_t, Is>...>), ...));
#endif
    return 0;
}

int main() {
    return bench_vec(n_pack); //
}
