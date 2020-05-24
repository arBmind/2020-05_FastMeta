#include <utility>

namespace stx {
// after Jorg Brown @CppCon2019 (Slide 46/61)
struct WrapperBase {
    static constexpr bool IsSame(void*) { return false; }
};
template<class T> struct Wrapper : WrapperBase {
    using WrapperBase::IsSame;
    static constexpr bool IsSame(Wrapper<T>*) { return true; }
};

template<class A, class B> using is_same = std::integral_constant<bool, Wrapper<A>::IsSame((Wrapper<B>*)(nullptr))>;

// make sure it works
static_assert(is_same<int, int>::value);
static_assert(!is_same<int, float>::value);
static_assert(!is_same<int, const int>::value);
static_assert(!is_same<const int, int>::value);
static_assert(is_same<const int, const int>::value);

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
    return (stx::is_same<std::integral_constant<size_t, Is>, T>::value || ...);
#endif
}

template<size_t... Is> constexpr auto bench_t(std::index_sequence<Is...>) {
    return (bench_d<std::integral_constant<int, Is>>(d_pack) || ...);
}

int main() { return bench_t(t_pack); }
