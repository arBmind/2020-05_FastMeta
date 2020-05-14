#include <utility>

namespace stx {
// after Jorg Brown @CppCon2019 (Slide 44/61)
template<class T> struct Wrapper {
    static constexpr bool IsSame(Wrapper<T>*) { return true; }
    static constexpr bool IsSame(void*) { return false; }
};

// make sure it works
static_assert(Wrapper<int>::IsSame((Wrapper<int>*)(nullptr)));
static_assert(!Wrapper<int>::IsSame((Wrapper<float>*)(nullptr)));
static_assert(!Wrapper<int>::IsSame((Wrapper<const int>*)(nullptr)));
static_assert(!Wrapper<const int>::IsSame((Wrapper<int>*)(nullptr)));
static_assert(Wrapper<const int>::IsSame((Wrapper<const int>*)(nullptr)));

} // namespace stx

#ifndef CPPBENCH_N
constexpr std::size_t CPPBENCH_N = 10;
#endif

constexpr auto D = []() -> std::size_t {
    if (CPPBENCH_N < 500) return 1;
    if (CPPBENCH_N < 10000) return CPPBENCH_N / 100;
    return CPPBENCH_N / 500;
}();
constexpr std::size_t T = CPPBENCH_N / D;
static_assert(T * D == CPPBENCH_N);

constexpr auto d_pack = std::make_index_sequence<D>();
constexpr auto t_pack = std::make_index_sequence<T>();

template<class T, std::size_t... Is> //
constexpr auto bench_d(std::index_sequence<Is...>) {
    (void)(sizeof(std::integral_constant<std::size_t, Is>) + ...);
#ifdef BASELINE
    return true;
#else
    return (stx::Wrapper<std::integral_constant<std::size_t, Is>>::IsSame((stx::Wrapper<T>*)(nullptr)) || ...);
#endif
}

template<std::size_t... Is> //
constexpr auto bench_t(std::index_sequence<Is...>) {
    return (bench_d<std::integral_constant<int, Is>>(d_pack) || ...);
}

int main() {
    return bench_t(t_pack); //
}
