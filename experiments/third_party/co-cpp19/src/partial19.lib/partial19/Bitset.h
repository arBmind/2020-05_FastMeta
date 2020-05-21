#pragma once
#include <stddef.h> // size_t

namespace partial19 {

template<class T, size_t Len> struct BitsetArray {
    T m[Len] = {};

    [[nodiscard]] constexpr bool at(size_t index) const noexcept {
        return 0 != (m[index / sizeof(T)] & (1u << (index % sizeof(T))));
    }

    constexpr void reset() noexcept {
        for (auto& v : m) v = {};
    }

    constexpr void setAt(size_t index) noexcept { m[index / sizeof(T)] |= (1u << (index % sizeof(T))); }
    constexpr void resetAt(size_t index) noexcept {
        m[index / sizeof(T)] &= ~static_cast<T>(1u << (index % sizeof(T)));
    }
};

/// For Len == 1 we can skip the division and modulo
/// should be easier to parse and to optimize
template<class T> struct BitsetArray<T, 1u> {
    T m{};

    [[nodiscard]] constexpr bool at(size_t index) const noexcept { return 0 != (m & (1u << index)); }

    constexpr void reset() noexcept { m = {}; }

    constexpr void setAt(size_t index) noexcept { m |= (1u << index); }
    constexpr void resetAt(size_t index) noexcept { m &= ~static_cast<T>(1u << index); }
};

template<size_t Count, size_t Len = (Count + (sizeof(unsigned) - 1)) / sizeof(unsigned)>
struct Bitset : BitsetArray<unsigned, Len> {
    constexpr static auto count = Count;
};

} // namespace partial19
