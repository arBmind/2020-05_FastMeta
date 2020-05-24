#include "Array.h"

#include <ostream>

namespace array19 {

template<class Chr, class Traits, class T, size_t C>
auto operator<<(std::basic_ostream<Chr, Traits>& out, const Array<T, C>& a) -> decltype(out)& {
    out << "[";
    bool first = true;
    for (auto& v : a) {
        if (first)
            first = false;
        else
            out << ", ";
        out << v;
    }
    return out << "]";
}

} // namespace array19