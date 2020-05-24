#include <utility>

namespace stx {
// after Jorg Brown @CppCon2019 (Slide 46/61)
struct WrapperBase {
    // avoid pointer cast
    static constexpr bool IsSame(...) { return false; }
};
template<class T> struct Wrapper : WrapperBase {
    using WrapperBase::IsSame;
    static constexpr bool IsSame(Wrapper<T>*) { return true; }
};

// support with wrapper_to… but no is_same
template<class T> constexpr Wrapper<T>* wrapper_to = nullptr;

// make sure it works
static_assert(Wrapper<int>::IsSame(wrapper_to<int>));
static_assert(!Wrapper<int>::IsSame(wrapper_to<float>));
static_assert(!Wrapper<int>::IsSame(wrapper_to<const int>));
static_assert(!Wrapper<const int>::IsSame(wrapper_to<int>));
static_assert(Wrapper<const int>::IsSame(wrapper_to<const int>));

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
    return (stx::Wrapper<std::integral_constant<size_t, Is>>::IsSame(stx::wrapper_to<T>) || ...);
#endif
}

template<size_t... Is> constexpr auto bench_t(std::index_sequence<Is...>) {
    return (bench_d<std::integral_constant<int, Is>>(d_pack) || ...);
}

int main() { return bench_t(t_pack); }
