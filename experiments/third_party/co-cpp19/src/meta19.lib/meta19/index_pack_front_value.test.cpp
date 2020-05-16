#include "IndexPack.h"
#include "index_pack_front_value.h"

using namespace meta19;

void index_pack_front_value_test() {
    static_assert(index_pack_front_value<IndexPack<12, 13>> == 12); //
    static_assert(index_pack_front_value<IndexPack<>> == nullptr); //
}
