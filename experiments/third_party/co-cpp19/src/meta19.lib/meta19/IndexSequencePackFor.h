#pragma once
#include "IndexSequencePack.h"

namespace meta19 {

template<class... Ts> //
using IndexSequencePackFor = IndexSequencePack<sizeof...(Ts)>;

} // namespace meta19
