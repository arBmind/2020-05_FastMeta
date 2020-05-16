#pragma once
#include <cstddef> // size_t
#include <utility>

namespace array19 {

template<class T> //
struct SliceOf {
    using Element = T;
    using Count = size_t;
    using Index = size_t;
    using Slice = SliceOf;

    constexpr SliceOf() = default;
    constexpr SliceOf(const SliceOf&) = default;
    constexpr SliceOf& operator=(const SliceOf&) = default;
    constexpr SliceOf(SliceOf&&) = default;
    constexpr SliceOf& operator=(SliceOf&&) = default;

    constexpr explicit //
        SliceOf(Element* data, Element* end) noexcept
        : _data(data)
        , _count(end - data) {}

    constexpr explicit //
        SliceOf(Element* data, Count count) noexcept
        : _data(data)
        , _count(count) {}

    template<size_t N> constexpr explicit //
        SliceOf(T (&array)[N]) noexcept
        : _data(N > 0 ? array : nullptr)
        , _count(N) {}

    [[nodiscard]] constexpr auto count() const -> Count { return _count; }
    [[nodiscard]] constexpr auto slice(Index offset, Count count) const -> Slice {
        return Slice{_data + offset, count};
    }

    [[nodiscard]] constexpr auto begin() const & noexcept -> Element* { return _data; }
    [[nodiscard]] constexpr auto end() const& -> Element* { return _data + _count; }
    [[nodiscard]] constexpr auto at(Index index) const & noexcept -> Element& { return *(_data + index); }

private:
    Element* _data{};
    Count _count{};
};

template<class T, size_t N> SliceOf(T (&)[N])->SliceOf<T>;

} // namespace array19
