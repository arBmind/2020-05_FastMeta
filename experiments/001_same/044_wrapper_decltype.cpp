#include <utility>

namespace stx {

// use return type only
template<class T> struct Wrapper {
    static auto IsSame(Wrapper<T>*) -> std::true_type;
    static auto IsSame(void*) -> std::false_type;
};

template<class A, class B> constexpr auto is_same_v = decltype(Wrapper<A>::IsSame((Wrapper<B>*)(nullptr))){};

// make sure it works
static_assert(is_same_v<int, int>);
static_assert(!is_same_v<int, float>);
static_assert(!is_same_v<int, const int>);
static_assert(!is_same_v<const int, int>);
static_assert(is_same_v<const int, const int>);

} // namespace stx

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

template<class T, size_t... Is> constexpr auto bench_d(std::index_sequence<Is...>) {
    (void)(sizeof(std::integral_constant<size_t, Is>) + ...);
#ifdef BASELINE
    return true;
#else
    return (stx::is_same_v<std::integral_constant<size_t, Is>, T> || ...);
#endif
}

template<size_t... Is> constexpr auto bench_t(std::index_sequence<Is...>) {
    return (bench_d<std::integral_constant<int, Is>>(d_pack) || ...);
}

int main() { return bench_t(t_pack); }
