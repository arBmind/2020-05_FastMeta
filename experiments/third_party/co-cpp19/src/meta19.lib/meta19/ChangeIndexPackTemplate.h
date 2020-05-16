#pragma once
#include "IndexPackTemplate.h"
#include "nulltpr_to.h"

namespace meta19 {

template<template<size_t...> class ToTpl, template<size_t...> class FromTpl, size_t... Vs>
constexpr auto changeIndexPackTemplate(FromTpl<Vs...>*, IndexPackTemplate<ToTpl>* = {}) -> ToTpl<Vs...>;

template<template<size_t...> class ToTpl, class From> //
using ChangeIndexPackTemplate = decltype(changeIndexPackTemplate<ToTpl>(nullptr_to<From>));

} // namespace meta19
