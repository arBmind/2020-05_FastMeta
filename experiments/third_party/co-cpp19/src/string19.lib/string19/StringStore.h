#pragma once
#include "stddef.h" // size_t

namespace string19 {

template<size_t N> using CharArray = char[N];

/// constexpr wrapper around an array of characters
/// Use this to for the compilers to store the string in your binary.
///
/// note: the stored array is always 0 terminated.
/// also note: N is the number of actual characters (not including the trailing \0)
template<size_t N> struct StringStore {
    CharArray<N + 1> data{};
    static constexpr auto count = N;

    constexpr char operator[](size_t i) const noexcept { return data[i]; }
};

/// note: always use storeLiteral("Hello") - or you get a 2nd \0 into your string!
template<size_t N> StringStore(const char (&)[N]) -> StringStore<N>;

} // namespace string19
