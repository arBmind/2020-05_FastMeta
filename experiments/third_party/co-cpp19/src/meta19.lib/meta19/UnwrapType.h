#pragma once
#include "Type.h"
#include "nulltpr_to.h"

namespace meta19 {

template<class T> //
constexpr auto unwrapType(Type<T>*) -> T;

template<class T> //
using UnwrapType = decltype(unwrapType(nullptr_to<T>));

} // namespace meta19
