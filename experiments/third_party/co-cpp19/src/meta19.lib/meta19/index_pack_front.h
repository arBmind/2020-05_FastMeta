#pragma once
#include "Index.h"

namespace meta19 {

template<class IP> inline constexpr auto index_pack_front = nullptr;

template<template<size_t...> class Tpl, size_t H, size_t... Vs>
inline constexpr auto& index_pack_front<Tpl<H, Vs...>> = _index<H>;

} // namespace meta19
