#include "IndexPack.h"
#include "index_pack_back_value.h"

using namespace meta19;

void index_pack_back_value_test() {
    static_assert(index_pack_back_value<IndexPack<12, 13>> == 13); //
    static_assert(index_pack_back_value<IndexPack<>> == nullptr); //
}
