#pragma once

namespace meta19 {

template<class T> inline constexpr //
    T* nullptr_to = nullptr;

template<class T> inline constexpr //
    T* nullptr_to<T&> = nullptr;

} // namespace meta19
