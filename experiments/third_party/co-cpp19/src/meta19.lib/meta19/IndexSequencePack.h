#pragma once
#include "IndexPackFor.h"
#include <utility>

namespace meta19 {

template<std::size_t N> //
using IndexSequencePack = IndexPackFor<std::make_index_sequence<N>>;

} // namespace meta19
