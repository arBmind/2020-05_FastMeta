#pragma once
#include "Array.h"
#include "Zip.h"

namespace array19 {

template<class A, class B, size_t AN, size_t BN> //
constexpr bool operator==(const Array<A, AN>& a, const Array<B, BN>& b) {
    if constexpr (AN != BN)
        return false;
    else {
        for (auto [av, bv] : Zip(a, b)) {
            if (!(av == bv)) return false;
        }
        return true;
    }
}

template<class A, class B, size_t AN, size_t BN> //
constexpr bool operator!=(const Array<A, AN>& a, const Array<B, BN>& b) {
    return !(a == b);
}

} // namespace array19
