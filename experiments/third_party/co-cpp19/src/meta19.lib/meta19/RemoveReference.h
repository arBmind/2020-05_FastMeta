#pragma once
#include "nulltpr_to.h"

namespace meta19 {

template<class T> //
constexpr auto removePtr(T*) -> T;

template<class T> //
using RemoveReference = decltype(removePtr(nullptr_to<T>));

} // namespace meta19