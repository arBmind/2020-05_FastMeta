#include <cstddef>
#include <kvasir/mpl/sequence/lookup.hpp>
#include <kvasir/mpl/types/list.hpp>
#include <type_traits>
#include <utility>

template<size_t I, class... Ts> using TypeAt = kvasir::mpl::eager::lookup<kvasir::mpl::list<Ts...>, I>;

static_assert(std::is_same_v<int, TypeAt<1, char, int, float>>);

#ifndef CPPBENCH_N
constexpr size_t CPPBENCH_N = 10;
#endif

constexpr auto n_pack = std::make_index_sequence<CPPBENCH_N>();

template<size_t... Is> auto bench_vec(std::index_sequence<Is...>) {
    (void)(sizeof(std::integral_constant<size_t, Is>) + ...);
#ifdef BASELINE
#else
    using L = kvasir::mpl::list<std::integral_constant<size_t, Is>...>;
    (((void)sizeof(kvasir::mpl::eager::lookup<L, Is>), ...));
//    (((void)sizeof(TypeAt<Is, std::integral_constant<size_t, Is>...>), ...));
#endif
    return 0;
}

int main() { return bench_vec(n_pack); }
