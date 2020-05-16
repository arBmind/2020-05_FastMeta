#pragma once
#include "RemoveReference.h"
#include "nulltpr_to.h"
#include <stddef.h>

namespace meta19 {

template<template<size_t...> class Tpl, size_t... Ls, size_t... Rs> //
constexpr auto indexPackJoin(Tpl<Ls...>*, Tpl<Rs...>*) -> Tpl<Ls..., Rs...>* const& {
    return nullptr_to<Tpl<Ls..., Rs...>>;
}

template<class L, class R> //
using IndexPackJoin = RemoveReference<decltype(*indexPackJoin(nullptr_to<L>, nullptr_to<R>))>;

} // namespace meta19
