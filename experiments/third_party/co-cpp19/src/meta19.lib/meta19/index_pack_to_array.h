#pragma once
#include "array19/Array.h"

namespace meta19 {

template<class> inline constexpr auto index_pack_to_array = nullptr;

template<template<size_t...> class Tpl, size_t... Is>
inline constexpr auto index_pack_to_array<Tpl<Is...>> = array19::Array{Is...};

} // namespace meta19
