#pragma once
#include "nulltpr_to.h"
#include <stddef.h>

namespace meta19 {

template<template<size_t...> class> //
struct IndexPackTemplate;

template<template<size_t...> class Tpl> inline constexpr //
    auto& index_pack_template = nullptr_to<IndexPackTemplate<Tpl>>;

} // namespace meta19
