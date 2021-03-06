#include <cstddef>
#include <utility>

template<class T> inline constexpr T* nullptr_to = nullptr;
template<class T> inline constexpr T* nullptr_to<T&> = nullptr;

struct TypeCombiner {};
auto operator||(TypeCombiner, TypeCombiner) -> TypeCombiner;
template<class T> auto operator||(TypeCombiner, T) -> T;
template<class T> auto operator||(T, TypeCombiner) -> T;

template<size_t I, class... Ts, size_t... Is> auto typeAt(std::index_sequence<Is...>)
    -> decltype((std::declval<std::conditional_t<Is == I, Ts, TypeCombiner>>() || ... || std::declval<TypeCombiner>()));

template<size_t I, class... Ts> using TypeAt =
    decltype(typeAt<I, Ts...>(std::declval<std::make_index_sequence<sizeof...(Ts)>>()));

static_assert(std::is_same_v<int, TypeAt<1, char, int, float>>);

#ifndef CPPBENCH_N
constexpr size_t CPPBENCH_N = 10;
#endif

constexpr auto n_pack = std::make_index_sequence<CPPBENCH_N>();

template<size_t... Is> auto bench_vec(std::index_sequence<Is...>) {
    (void)(sizeof(std::integral_constant<size_t, Is>) + ...);
#ifdef BASELINE
#else
    (((void)sizeof(TypeAt<Is, std::integral_constant<size_t, Is>...>), ...));
#endif
    return 0;
}

int main() { return bench_vec(n_pack); }
