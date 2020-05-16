#pragma once
#include "TypePack.h"
#include "nulltpr_to.h"

namespace meta19 {

template<template<class...> class Tpl, class T, class... Ts> //
auto typePackFrontType(Tpl<T, Ts...>*) -> T;

template<class... Ts> //
using FrontType = decltype(typePackFrontType(nullptr_to<TypePack<Ts...>>));

template<class T> //
using TemplateFrontType = decltype(typePackFrontType(nullptr_to<T>));

} // namespace meta19
