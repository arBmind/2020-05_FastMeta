#include <utility>

namespace stx {

template<class T, T> struct Const;
template<class T, T V> constexpr Const<T, V>* const_ = {};

template<class T> struct Type;
template<class T> constexpr Type<T>* type = {};

constexpr bool same(const void* a, const void* b) { return a == b; }

} // namespace stx

using stx::Const;
using stx::type;

// make sure it works
static_assert(stx::same(&type<int>, &type<int>));
static_assert(!stx::same(&type<int>, &type<float>));
static_assert(!stx::same(&type<int>, &type<const int>));
static_assert(!stx::same(&type<const int>, &type<int>));
static_assert(stx::same(&type<const int>, &type<const int>));

#ifndef CPPBENCH_N
constexpr size_t CPPBENCH_N = 10;
#endif

constexpr auto D = []() -> size_t {
    if (CPPBENCH_N < 500) return 1;
    if (CPPBENCH_N < 10000) return CPPBENCH_N / 100;
    return CPPBENCH_N / 500;
}();
constexpr size_t T = CPPBENCH_N / D;
static_assert(T * D == CPPBENCH_N);

constexpr auto d_pack = std::make_index_sequence<D>();
constexpr auto t_pack = std::make_index_sequence<T>();

template<size_t... Is> constexpr auto bench_d(const void* a, std::index_sequence<Is...>) {
    (void)(sizeof(type<Const<size_t, Is>>) + ...);
#ifdef BASELINE
    return true;
#else
    return (stx::same(&type<Const<size_t, Is>>, a) || ...);
#endif
}

template<size_t... Is> constexpr auto bench_t(std::index_sequence<Is...>) {
    return (bench_d(&type<Const<int, Is>>, d_pack) || ...);
}

int main() { return bench_t(t_pack); }
