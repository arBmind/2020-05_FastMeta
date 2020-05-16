#pragma once
#include "IndexPack.h"

namespace meta19 {

// IndexPack For any IndexPackTemplate or TypeValuePackTemplate
template<template<size_t...> class FromTpl, size_t... Vs> //
constexpr auto indexPackFor(FromTpl<Vs...>*) -> IndexPack<Vs...>;

template<template<class T, T...> class FromTpl, class T, T... Vs> //
constexpr auto indexPackFor(FromTpl<T, Vs...>*) -> IndexPack<Vs...>;

template<class From> //
using IndexPackFor = decltype(indexPackFor(nullptr_to<From>));

} // namespace meta19
