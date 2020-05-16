#pragma once
#include "SliceOf.h"

#include <new> // launder

namespace array19 {

namespace details {

template<class ElemStorage> //
struct DynamicStorage final {
    using ElementStorage = ElemStorage;
    using Slice = SliceOf<ElemStorage>;
    using Capacity = size_t;
    using Index = size_t;

    DynamicStorage() = default;
    ~DynamicStorage() noexcept { delete[](begin()); }

    DynamicStorage(DynamicStorage&& o) noexcept
        : _beg(o._beg)
        , _capacity(o._capacity) {
        o._beg = 0;
    }
    DynamicStorage& operator=(DynamicStorage&& o) noexcept {
        delete[](begin());
        _beg = o._beg;
        _capacity = o._capacity;
        o._beg = 0;
    }

    [[nodiscard]] static auto create(Capacity capacity) -> DynamicStorage {
        auto result = DynamicStorage{};
        result._beg = new ElementStorage[capacity];
        result._capacity = capacity;
        return result;
    }

    [[nodiscard]] auto begin() const noexcept -> ElementStorage* { return _beg; }
    [[nodiscard]] auto end() const noexcept -> ElementStorage* { return _beg + _capacity; }

    [[nodiscard]] auto ptr_at(Index index) const noexcept -> ElementStorage* { return _beg + index; }

    [[nodiscard]] auto capacity() const noexcept -> Capacity { return _capacity; }

private:
    ElementStorage* _beg{};
    Capacity _capacity{};
};

} // namespace details

template<class T> //
struct DynamicArrayOf final {
    using Element = T;
    using Size = size_t;
    using Index = size_t;

    using Reference = Element&;
    using ConstReference = const Element&;
    using Iterator = Element*;
    using ConstIterator = const Element*;
    using Slice = SliceOf<Element>;
    using ConstSlice = SliceOf<const Element>;

    DynamicArrayOf() = default;
    ~DynamicArrayOf() noexcept(std::is_nothrow_destructible_v<Element>) { destructSlice(amendSlice()); }

    DynamicArrayOf(const DynamicArrayOf& o) noexcept(std::is_nothrow_copy_constructible_v<Element>)
        : m_storage(Storage::create(o._count))
        , m_count(o._count) {
        copyConstructSlice(m_storage.begin(), o.slice());
    }
    DynamicArrayOf& operator=(const DynamicArrayOf& o) noexcept(std::is_nothrow_copy_assignable_v<Element>) {
        if (m_storage.capacity() < o.count()) {
            *this = DynamicArrayOf(o);
        }
        else {
            auto countDiff = m_count - o._count;
            if (countDiff > 0) {
                copyAssignSlice(amendBegin(), ConstSlice{o.begin(), o.begin() + m_count});
                copyConstructSlice(storageEnd(), ConstSlice{o.begin() + m_count, o.end()});
            }
            else {
                copyAssignSlice(amendBegin(), o.slice());
                destructSlice(Slice{amendBegin() + o._count, amendEnd()});
            }
            m_count = o.count();
        }
    }

    DynamicArrayOf(DynamicArrayOf&& o) noexcept
        : m_storage(std::move(o))
        , m_count(o._count) {
        o._count = 0;
    }
    DynamicArrayOf& operator=(DynamicArrayOf&& o) noexcept {
        destructSlice(amendSlice());
        m_storage = std::move(o);
        m_count = o._count;
        o._count = 0;
    }

    [[nodiscard]] auto count() const -> Size { return m_count; }
    [[nodiscard]] auto totalCapacity() const -> Size { return m_storage.capacity(); }
    [[nodiscard]] auto unusedCapacity() const -> Size { return m_storage.capacity() - m_count; }

    [[nodiscard]] auto begin() const & noexcept -> ConstIterator {
        return std::launder(reinterpret_cast<const T*>(m_storage.begin()));
    }
    // NOTE: end() is not laundered!
    [[nodiscard]] auto end() const & noexcept -> ConstIterator {
        return reinterpret_cast<const T*>(m_storage.begin() + m_count);
    }

    [[nodiscard]] auto at(Size index) const & noexcept -> ConstReference {
        return *std::launder(reinterpret_cast<const T*>(m_storage.ptr_at(index)));
    }
    [[nodiscard]] auto front() const & noexcept -> ConstReference { return at(0); }
    [[nodiscard]] auto back() const & noexcept -> ConstReference { return at(m_count - 1); }
    [[nodiscard]] auto slice() const -> ConstSlice { return ConstSlice{begin(), end()}; }

    [[nodiscard]] auto amendBegin() & noexcept -> Iterator {
        return std::launder(reinterpret_cast<T*>(m_storage.begin()));
    }
    // NOTE: end() is not laundered!
    [[nodiscard]] auto amendEnd() & noexcept -> Iterator {
        return amendBegin() + m_count; //
    }

    [[nodiscard]] auto amendAt(Size index) & noexcept -> Reference {
        return *std::launder(reinterpret_cast<T*>(m_storage.ptr_at(index)));
    }
    [[nodiscard]] auto amendFront() & noexcept -> Reference { return amendAt(0); }
    [[nodiscard]] auto amendBack() & noexcept -> Reference { return amendAt(m_count - 1); }
    [[nodiscard]] auto amendSlice() -> Slice { return Slice{amendBegin(), amendEnd()}; }

    void appendSlice(ConstSlice elems) {
        if (unusedCapacity() < elems.count()) grow(elems.count());
        copyConstructSlice(storageEnd(), elems);
        m_count += elems.count();
    }

    void pop() noexcept(std::is_nothrow_destructible_v<Element>) {
        destructSlice(Slice{amendBegin() + m_count - 1, amendEnd()});
        m_count--;
    }

    void splice(Iterator it, Size removeCount, ConstSlice insertSlice) {
        auto offset = it - amendBegin();
        auto insertCount = insertSlice.count();
        auto remainCount = m_count - offset - removeCount;
        auto remainSlice = Slice{it + removeCount, amendEnd()};
        // old: [ ..(offset)..,[it] ..(removeCount).., ..(remainCount)... ]
        // new: [ ..(offset)..,[it] ..(insertCount).., ..(remainCount)... ]

        auto countDiff = insertCount - removeCount;
        if (unusedCapacity() < countDiff) { // not enough storage => arrange everything in new storage
            auto newStorage = grownStorage(countDiff);
            moveConstructSlice(newStorage.begin(), Slice{amendBegin(), it});
            copyConstructSlice(newStorage.begin() + offset, insertSlice);
            moveConstructSlice(newStorage.begin() + offset + insertCount, remainSlice);
            destructSlice(amendSlice());
            m_storage = newStorage;
        }
        else if (countDiff <= 0) { // shrinking
            copyAssignSlice(it, insertSlice);
            forwardMoveConstructSlice(it + insertCount, remainSlice);
            destructSlice(Slice{amendEnd() + countDiff, amendEnd()});
        }
        else if (offset + insertCount <= m_count) { // parts of remainSlice is moved beyond end()
            moveConstructSlice(storageEnd(), remainSlice.slice(remainCount - countDiff, countDiff));
            reverseMoveAssignSlice(it + insertCount, remainSlice.slice(0, remainCount - countDiff));
            copyAssignSlice(it, insertSlice);
        }
        else { // remainSlice is moved beyond end()
            moveConstructSlice(m_storage.begin() + offset + insertCount, remainSlice);
            auto assignElems = m_count - offset;
            copyAssignSlice(it, insertSlice.slice(0, assignElems));
            copyConstructSlice(storageEnd(), insertSlice.slice(assignElems, insertCount - assignElems));
        }
        m_count += countDiff;
    }

private:
    struct ElementStorage final {
        alignas(T) unsigned char data[sizeof(T)];
    };
    using Storage = details::DynamicStorage<ElementStorage>;
    using StorageSlice = SliceOf<ElementStorage>;
    using StorageIterator = ElementStorage*;
    constexpr static auto element_size = sizeof(ElementStorage);

    auto byteSize() const -> Size { return m_count * element_size; }
    auto storageEnd() -> StorageIterator { return m_storage.begin() + m_count; }

    static void destructSlice(Slice slice) {
        for (auto& elem : slice) elem.~Element();
    }
    static void copyAssignSlice(Iterator to, ConstSlice fromSlice) {
        if constexpr (std::is_trivially_copy_assignable_v<Element>) {
            memcpy(to, fromSlice.begin(), fromSlice.count() * element_size);
        }
        else {
            for (const auto& e : fromSlice) *to++ = e;
        }
    }
    static void moveAssignSlice(Iterator to, Slice fromSlice) {
        if constexpr (std::is_trivially_move_assignable_v<Element>) {
            memcpy(to, fromSlice.amendBegin(), fromSlice.count() * element_size);
        }
        else {
            for (const auto& e : fromSlice) *to++ = std::move(e);
        }
    }
    static void reverseMoveAssignSlice(Iterator to, Slice fromSlice) {
        if constexpr (std::is_trivially_move_assignable_v<Element>) {
            memmove(to, fromSlice.amendBegin(), fromSlice.count() * element_size);
        }
        else {
            auto rTo = to + fromSlice.count();
            auto rFrom = fromSlice.amendEnd();
            auto rFromEnd = fromSlice.amendBegin();
            while (rFrom != rFromEnd) *(--rTo) = std::move(*(--rFrom));
        }
    }
    static void copyConstructSlice(StorageIterator to, ConstSlice fromSlice) {
        if constexpr (std::is_trivially_copy_constructible_v<Element>) {
            memcpy(to, fromSlice.begin(), fromSlice.count() * element_size);
        }
        else {
            for (auto& from : fromSlice) new (to++) Element(from);
        }
    }
    static void moveConstructSlice(StorageIterator to, Slice fromSlice) {
        if constexpr (std::is_trivially_move_constructible_v<Element>) {
            memcpy(to, fromSlice.amendBegin(), fromSlice.count() * element_size);
        }
        else {
            for (auto& from : fromSlice) new (to++) Element(std::move(from));
        }
    }
    static void forwardMoveConstructSlice(StorageIterator to, Slice fromSlice) {
        if constexpr (std::is_trivially_move_constructible_v<Element>) {
            memmove(to, fromSlice.amendBegin(), fromSlice.count() * element_size);
        }
        else {
            for (auto& from : fromSlice) new (to++) Element(std::move(from));
        }
    }
    auto grownStorage(int growBy) const -> Size {
        auto cur = totalCapacity();
        auto res = (cur << 1) - (cur >> 1) + (cur >> 4); // * 1.563
        if (res < 5) res = 5;
        if (res < totalCapacity() + growBy) res = totalCapacity() + growBy;
        return Storage::create(res);
    }
    void grow(int by) {
        auto newStorage = grownStorage(by);
        moveConstructSlice(newStorage.begin(), amendSlice());
        destructSlice(amendSlice());
        m_storage = newStorage;
    }

private:
    Storage m_storage;
    Size m_count;
};

} // namespace array19
