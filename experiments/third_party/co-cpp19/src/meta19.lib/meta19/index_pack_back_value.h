#pragma once
#include <stddef.h>

namespace meta19 {

template<class IP> inline constexpr //
    auto index_pack_back_value = nullptr;

template<template<size_t...> class Tpl, size_t V, size_t... Vs> inline constexpr //
    auto index_pack_back_value<Tpl<V, Vs...>> = (V, ..., Vs);

} // namespace meta19
