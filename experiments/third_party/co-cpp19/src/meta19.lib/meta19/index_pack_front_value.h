#pragma once
#include <stddef.h>

namespace meta19 {

template<class IP> inline constexpr //
    auto index_pack_front_value = nullptr;

template<template<size_t...> class Tpl, size_t H, size_t... Vs> inline constexpr //
    auto index_pack_front_value<Tpl<H, Vs...>> = H;

} // namespace meta19
