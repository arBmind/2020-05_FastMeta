#pragma once
#include "nulltpr_to.h"

namespace meta19 {

template<class> //
struct Type;

template<class T> inline constexpr //
    auto& type = nullptr_to<Type<T>>;

} // namespace meta19
