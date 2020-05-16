#pragma once
#include "false_for.h"

namespace meta19 {

constexpr bool isSame(const void* a, const void* b) { return a == b; }

template<class T, class U> //
constexpr auto isSame(T*&, U*&) {
    static_assert(false_for<T, U>, "HINT: use &type<T>!");
}

} // namespace meta19
