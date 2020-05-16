#pragma once
#include "nulltpr_to.h"
#include <stddef.h>

namespace meta19 {

template<size_t T> //
struct Index;

template<size_t V> inline constexpr //
    auto& _index = nullptr_to<Index<V>>;

} // namespace meta19
