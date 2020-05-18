#pragma once
#include "RemoveReference.h"
#include "index_pack_front.h"

namespace meta19 {

template<class IP> using IndexPackFront = PointeeOf<decltype(index_pack_front<IP>)>;

} // namespace meta19
