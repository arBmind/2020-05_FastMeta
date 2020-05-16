#include "IndexPack.h"
#include "index_pack_to_array.h"

using namespace meta19;

void index_pack_to_array_test() {
    constexpr auto x = index_pack_to_array<IndexPack<0, 4, 8>>;
    static_assert(x.size() == 3);
    static_assert(x[0] == 0);
    static_assert(x[1] == 4);
    static_assert(x[2] == 8);
}
