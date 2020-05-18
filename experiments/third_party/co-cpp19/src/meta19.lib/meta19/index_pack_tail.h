#pragma once
#include "index_pack.h"

namespace meta19 {

template<class> inline constexpr auto index_pack_tail = nullptr;

template<template<size_t...> class Tpl, size_t H, size_t... Vs>
inline constexpr auto& index_pack_tail<Tpl<H, Vs...>> = index_pack<Vs...>;

} // namespace meta19
