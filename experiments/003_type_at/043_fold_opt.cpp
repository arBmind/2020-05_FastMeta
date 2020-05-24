#include <cstddef>
#include <utility>

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

template<bool> struct conditional { template<class T, class> using type = T; };
template<> struct conditional<false> { template<class, class T> using type = T; };

template<size_t I, class Is, class... Ts> struct TypeAtImpl;
template<size_t I, size_t... Is, class... Ts> struct TypeAtImpl<I, std::index_sequence<Is...>, Ts...>
    : FoldRec<typename conditional<Is == I>::template type<Ts, NoType>...> {};

template<size_t I, class... Ts> using TypeAt =
    typename TypeAtImpl<I, std::make_index_sequence<sizeof...(Ts)>, Ts...>::type;

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
