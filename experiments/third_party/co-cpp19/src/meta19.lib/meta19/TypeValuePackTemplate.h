#pragma once
#include "nulltpr_to.h"

namespace meta19 {

template<template<class T, T...> class> //
struct TypeValuePackTemplate;

template<template<class T, T...> class Tpl> inline constexpr //
    auto& type_value_pack_template = nullptr_to<TypeValuePackTemplate<Tpl>>;

} // namespace meta19
