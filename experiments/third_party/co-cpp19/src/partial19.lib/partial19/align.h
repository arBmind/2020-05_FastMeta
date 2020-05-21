#pragma once
#include <inttypes.h> // uint8_t
#include <stddef.h> // size_t

namespace partial19 {

/// returns the offset increased to be aligned for given align.
/// preconditions:
/// * align == 2^N - only supports 2^n alignments
/// * align >= 1
/// * offset < MAX-align
constexpr auto alignOffset(size_t align, size_t offset) -> size_t {
    return static_cast<size_t>((offset + align - 1) & ~(align - 1));
}

/// returns the ptr increased to be aligned for given align.
/// proconditons:
/// * align == 2^N - only supports 2^n alignments
/// * align >= 1
template<class T> auto alignPointer(size_t align, T* ptr) -> T* {
    auto offset = reinterpret_cast<intptr_t>(ptr);
    return reinterpret_cast<T*>(alignOffset(align, offset));
}

} // namespace partial19
