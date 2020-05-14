#include <cstddef>
#include <utility>

template<class T> inline constexpr //
    T* nullptr_to = nullptr;
template<class T> inline constexpr //
    T* nullptr_to<T&> = nullptr;

template<class...> struct TypePack;

template<class... Ts> inline constexpr //
    auto type_pack = nullptr_to<TypePack<Ts...>>;

struct NoType;
template<class... Ps> struct FoldRec;
template<class... Ps> struct FoldRec<NoType, Ps...> : FoldRec<Ps...> {};
template<class... Ps> struct FoldRec<NoType, NoType, Ps...> : FoldRec<Ps...> {};
template<class... Ps> struct FoldRec<NoType, NoType, NoType, NoType, Ps...> : FoldRec<Ps...> {};
template<class... Ps> struct FoldRec<NoType, NoType, NoType, NoType, NoType, NoType, NoType, NoType, Ps...>
    : FoldRec<Ps...> {};
template<class... Ps> struct FoldRec<
    NoType,
    NoType,
    NoType,
    NoType,
    NoType,
    NoType,
    NoType,
    NoType,
    NoType,
    NoType,
    NoType,
    NoType,
    NoType,
    NoType,
    NoType,
    NoType,
    Ps...> : FoldRec<Ps...> {};
template<class T, class... Ps> struct FoldRec<T, Ps...> { using type = T; };

template<size_t I, class... Ts, size_t... Is> //
auto typeAt(TypePack<Ts...>*, std::index_sequence<Is...>*) ->
    typename FoldRec<std::conditional_t<Is == I, Ts, NoType>...>::type;

template<size_t I, class... Ts> //
using TypeAt = decltype(typeAt<I>(nullptr_to<TypePack<Ts...>>, nullptr_to<std::make_index_sequence<sizeof...(Ts)>>));

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
    (((void)sizeof(TypeAt<Is, std::integral_constant<std::size_t, Is>...>), ...));
#endif
    return 0;
}

int main() {
    return bench_vec(n_pack); //
}
