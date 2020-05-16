#pragma once
#include "Index.h"
#include "nulltpr_to.h"
#include <utility>

namespace meta19 {

template<size_t, class> struct IndexType {};

template<class IP, class... Ts> struct IndexTypeOverloads;
template<size_t... Is, class... Ts> struct IndexTypeOverloads<std::index_sequence<Is...>, Ts...>
    : IndexType<Is, Ts>... {};

template<class... Ts> using IndexTypeMap = IndexTypeOverloads<std::make_index_sequence<sizeof...(Ts)>, Ts...>;

template<size_t I, class T> T typeAtSig(IndexType<I, T>*);

template<size_t I, class... Ts> //
using TypeAt = decltype(typeAtSig<I>(nullptr_to<IndexTypeMap<Ts...>>));

template<size_t I, class ITM> //
using MapTypeAt = decltype(typeAtSig<I>(nullptr_to<ITM>));

template<class T, size_t I> Index<I> indexOfSig(IndexType<I, T>*);
template<class T, size_t I> constexpr auto indexOf(IndexType<I, T>*) -> size_t { return I; }

template<class T, class... Ts> //
using IndexOf = decltype(indexOfSig<T>(nullptr_to<IndexTypeMap<Ts...>>));

template<class T, class ITM> //
using MapIndexOf = decltype(indexOfSig<T>(nullptr_to<ITM>));

template<class T, class ITM> inline constexpr //
    auto map_index_of = indexOf<T>(nullptr_to<ITM>);

} // namespace meta19
