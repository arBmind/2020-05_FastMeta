#pragma once
#include "RemoveReference.h"
#include "index_pack_tail.h"

namespace meta19 {

template<class IP> //
using IndexPackTail = RemoveReference<decltype(*index_pack_tail<IP>)>;

} // namespace meta19