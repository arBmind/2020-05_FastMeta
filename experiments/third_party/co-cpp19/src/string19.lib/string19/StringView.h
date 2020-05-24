#pragma once
#include "stddef.h" // size_t

namespace string19 {

enum class ExtraZero : unsigned {
    Missing,
    Present,
};

struct StringView {
    const char* data{};
    unsigned count{};
    ExtraZero extraZero{};

    constexpr char operator[](size_t i) const noexcept { return data[i]; }
};

} // namespace string19
