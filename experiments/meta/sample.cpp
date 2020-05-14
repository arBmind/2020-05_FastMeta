#include <cstddef>
#include <new>
#include <utility>

namespace meta {

using std::size_t;

// nullptr to
template<class T> inline constexpr //
    T* nullptr_to = nullptr;
template<class T> inline constexpr //
    T* nullptr_to<T&> = nullptr;

// remove reference
template<class T> //
constexpr auto removePtr(T*) -> T;

template<class T> //
using RemoveReference = decltype(removePtr(nullptr_to<T>));

// type
template<class> //
struct Type;

template<class T> inline constexpr //
    auto& type = nullptr_to<Type<T>>;

// extract type from Type
template<class T> //
constexpr auto unwrapType(Type<T>*) -> T;
template<class T> //
using UnwrapType = decltype(unwrapType(nullptr_to<T>));

// type_pack
template<class...> //
struct TypePack;

template<class... Ts> inline constexpr //
    auto& type_pack = nullptr_to<TypePack<Ts...>>;

// index
template<size_t T> //
struct Index;

template<size_t V> inline constexpr //
    auto& _index = nullptr_to<Index<V>>;

// extract value from index
template<class C> inline constexpr //
    auto index_value = nullptr;

template<size_t V> inline constexpr //
    auto index_value<Index<V>> = V;

// index pack
template<size_t...> //
struct IndexPack;

template<size_t... Vs> inline constexpr //
    auto& index_pack = nullptr_to<IndexPack<Vs...>>;

// index pack template
template<template<size_t...> class> //
struct IndexPackTemplate;

template<template<size_t...> class Tpl> inline constexpr //
    auto& index_pack_template = nullptr_to<IndexPackTemplate<Tpl>>;

// const pack recursion
template<class IP> inline constexpr //
    auto index_pack_head = nullptr;

template<template<size_t...> class Tpl, size_t H, size_t... Vs> inline constexpr //
    auto& index_pack_head<Tpl<H, Vs...>> = _index<H>;

template<class IP> //
using IndexPackHead = RemoveReference<decltype(*index_pack_head<IP>)>;

template<class IP> inline constexpr //
    auto index_pack_front_value = nullptr;

template<template<size_t...> class Tpl, size_t H, size_t... Vs> inline constexpr //
    auto index_pack_front_value<Tpl<H, Vs...>> = H;

template<class IP> inline constexpr //
    auto index_pack_back_value = nullptr;

template<template<size_t...> class Tpl, size_t... Vs> inline constexpr //
    auto index_pack_back_value<Tpl<Vs...>> = (Vs, ...);

template<class IP> inline constexpr //
    auto index_pack_tail = nullptr;

template<template<size_t...> class Tpl, size_t H, size_t... Vs> inline constexpr //
    auto& index_pack_tail<Tpl<H, Vs...>> = index_pack<Vs...>;

template<class IP> //
using IndexPackTail = RemoveReference<decltype(*index_pack_tail<IP>)>;

// index pack join
template<template<size_t...> class Tpl, size_t... Ls, size_t... Rs> //
constexpr auto indexPackJoin(Tpl<Ls...>*, Tpl<Rs...>*) -> Tpl<Ls..., Rs...>* const& {
    return nullptr_to<Tpl<Ls..., Rs...>>;
}

template<class L, class R> //
using IndexPackJoin = RemoveReference<decltype(*indexPackJoin(nullptr_to<L>, nullptr_to<R>))>;

// change IndexPack template
template<template<size_t...> class ToTpl, template<size_t...> class FromTpl, size_t... Vs>
constexpr auto changeIndexPackTemplate(FromTpl<Vs...>*, IndexPackTemplate<ToTpl>* = {}) -> ToTpl<Vs...>;

template<template<size_t...> class ToTpl, class From> //
using ChangeIndexPackTemplate = decltype(changeIndexPackTemplate<ToTpl>(nullptr_to<From>));

// type value pack template
template<template<class T, T...> class> //
struct TypeValuePackTemplate;

template<template<class T, T...> class Tpl> inline constexpr //
    auto& type_value_pack_template = nullptr_to<TypeValuePackTemplate<Tpl>>;

// IndexPack For any IndexPackTemplate or TypeValuePackTemplate
template<template<size_t...> class FromTpl, size_t... Vs> //
constexpr auto indexPackFor(FromTpl<Vs...>*) -> IndexPack<Vs...>;

template<template<class T, T...> class FromTpl, class T, T... Vs> //
constexpr auto indexPackFor(FromTpl<T, Vs...>*) -> IndexPack<Vs...>;

template<class From> //
using IndexPackFor = decltype(indexPackFor(nullptr_to<From>));

// index pack for
template<std::size_t N> //
using IndexSequencePack = IndexPackFor<std::make_index_sequence<N>>;

template<class... Ts> //
using IndexSequencePackFor = IndexSequencePack<sizeof...(Ts)>;

template<class> constexpr auto index_pack_to_array = nullptr;
template<size_t... Is> constexpr auto index_pack_to_array<IndexPack<Is...>> = std::array{Is...};

// false for any given types
template<class...> inline constexpr //
    auto false_for = false;

// isSame
constexpr bool isSame(const void* a, const void* b) { return a == b; }

template<class T, class U> //
constexpr auto isSame(T*&, U*&) {
    static_assert(false_for<T, U>, "HINT: use &type<T>!");
}

// align of type pack
template<class TP> inline constexpr //
    auto align_of_pack = nullptr;

template<class... Ts> inline constexpr //
    auto& align_of_pack<TypePack<Ts...>> = index_pack<alignof(Ts)...>;

template<class TP> //
using AlignOfPack = RemoveReference<decltype(*align_of_pack<TP>)>;

// size of type pack
template<class TP> inline constexpr //
    auto size_of_pack = nullptr;

template<class... Ts> inline constexpr //
    auto& size_of_pack<TypePack<Ts...>> = index_pack<sizeof(Ts)...>;

template<class TP> //
using SizeOfPack = RemoveReference<decltype(*size_of_pack<TP>)>;

// align
template<std::size_t align, std::size_t offset> inline constexpr //
    auto align_offset = static_cast<std::size_t>(((offset + align - 1) / align) * align);

template<std::size_t offset, class SizePack, class AlignPack> inline constexpr //
    auto& layout_size_align_pack = []() -> decltype(auto) {
    constexpr auto aligned_head = align_offset<index_pack_front_value<AlignPack>, offset>;
    constexpr auto aligned_tail = layout_size_align_pack<
        aligned_head + index_pack_front_value<SizePack>,
        IndexPackTail<SizePack>,
        IndexPackTail<AlignPack>>;
    return indexPackJoin(index_pack<aligned_head>, aligned_tail);
}();

template<std::size_t offset> inline constexpr //
    auto& layout_size_align_pack<offset, IndexPack<>, IndexPack<>> = index_pack<>;

template<class TP, std::size_t offset = 0> inline constexpr //
    auto& layout_tuple_pack = layout_size_align_pack<offset, SizeOfPack<TP>, AlignOfPack<TP>>;

// typeAt

struct TypeCombiner;
auto operator||(TypeCombiner, TypeCombiner) -> TypeCombiner;
template<class T> auto operator||(TypeCombiner, T) -> T;
template<class T> auto operator||(T, TypeCombiner) -> T;

template<size_t I, class... Ts, size_t... Is> auto typeAt(TypePack<Ts...>*, IndexPack<Is...>*) {
    return (std::declval<std::conditional_t<Is == I, Ts, TypeCombiner>>() || ... || std::declval<TypeCombiner>());
}
template<size_t I, class TP, class IP> using TypeAt = decltype(typeAt<I>(nullptr_to<TP>, nullptr_to<IP>));

static_assert(type<char> == nullptr_to<TypeAt<0, TypePack<int, char>, IndexPack<0, 1>>>);

// struct TypeCombiner {};
// auto operator||(TypeCombiner, TypeCombiner) -> TypeCombiner;
// template<class T> auto operator||(TypeCombiner, T*) -> T*;
// template<class T> auto operator||(T*, TypeCombiner) -> T*;

// tuple
template<class... Ts> struct Tuple {
    static constexpr auto pack = type_pack<Ts...>;
    static constexpr auto pack_indices = IndexSequencePackFor<Ts...>{};
    static constexpr auto pack_offsets = layout_tuple_pack<pack>;
    static constexpr auto offsets_array = index_pack_to_array<decltype(pack_offsets)>;
    static constexpr auto storage_size =
        index_pack_back_value<pack_offsets> + index_pack_back_value<size_of_pack<pack>>;
    static constexpr auto npos = sizeof...(Ts);

private:
    template<class = decltype(pack_indices)> struct Helper;
    template<size_t... Is> struct Helper<IndexPack<Is...>> {
        alignas(Ts...) std::byte storage[storage_size];

        constexpr Helper() { (new (ptrAt(Is)) Ts(), ...); }
        ~Helper() { (at(Is).~Ts(), ...); }

        constexpr Helper(const Helper& o) { (new (rawPtrAt<Is>()) Ts(*o.ptrAt<Is>()), ...); }
        constexpr Helper(Helper&& o) { (new (rawPtrAt<Is>()) Ts(std::move(*o.ptrAt<Is>())), ...); }
        constexpr Helper& operator=(const Helper& o) { ((*ptrAt<Is>() = *o.ptrAt<Is>()), ...); }
        constexpr Helper& operator=(Helper&& o) { ((*ptrAt<Is>() = std::move(*o.ptrAt<Is>())), ...); }

        constexpr Helper(const Ts&... ts) { (new (rawPtrAt<Is>()) Ts(ts), ...); }
        constexpr Helper(Ts&&... ts) { (new (rawPtrAt<Is>()) Ts(std::move(ts)), ...); }

        template<size_t I> using PtrAt =
            decltype((std::conditional_t<Is == I, Ts*, TypeCombiner>{} || ... || TypeCombiner{}));
        template<size_t I> using ConstPtrAt =
            decltype((std::conditional_t<Is == I, const Ts*, TypeCombiner>{} || ... || TypeCombiner{}));

        template<class T> static constexpr auto index_of = ((std::is_same_v<T, Ts> ? Is : 0) + ...);

        template<size_t I> constexpr auto ptrAt() -> PtrAt<I> {
            return std::launder(reinterpret_cast<PtrAt<I>>(rawPtrAt<I>()));
        }
        template<size_t I> constexpr auto ptrAt() const -> ConstPtrAt<I> {
            return std::launder(reinterpret_cast<ConstPtrAt<I>>(rawPtrAt<I>()));
        }

        template<class T> constexpr auto ptrOf() -> T* {
            return std::launder(reinterpret_cast<T*>(storage + offsets_array[index_of<T>]));
        }
        template<class T> constexpr auto ptrOf() const -> const T* {
            return std::launder(reinterpret_cast<const T*>(storage + offsets_array[index_of<T>]));
        }

        template<size_t I> constexpr auto rawPtrAt() -> void* { return storage + offsets_array[I]; }
        template<size_t I> constexpr auto rawPtrAt() const -> const void* { return storage + offsets_array[I]; }
    };
    Helper<> helper;

public:
    constexpr Tuple() = default;
    constexpr Tuple(const Ts&... ts)
        : helper(ts...) {}
    constexpr Tuple(Ts&&... ts)
        : helper(std::move(ts)...) {}

    template<class... Os> static constexpr auto fromArgs(Os&&... os) -> Tuple {
        auto res = Tuple{};
        ((res.updateOf(type<std::remove_const_t<std::remove_reference_t<Os>>>) = std::forward<Os>(os)), ...);
        return res;
    }
    template<class... Os> static constexpr auto fromTuple(const Tuple<Os...>& o) -> Tuple {
        return fromArgs(o.template of<Os>()...);
    }
    template<class... Os> static constexpr auto fromTuple(Tuple<Os...>&& o) -> Tuple {
        return fromArgs(std::move(o.template updateOf<Os>())...);
    }

    template<class O> static constexpr auto has_type = ((std::is_same_v<O, Ts>) || ...);
    template<class O> static constexpr auto index_of = Helper<>::template index_of<O>;
    template<class O> static constexpr auto offset_of = offsets_array[index_of<O>];
    template<size_t I> static constexpr auto offset_at = offsets_array[I];
    template<size_t I> using PtrAt = typename Helper<>::template PtrAt<I>;
    template<size_t I> using ConstPtrAt = typename Helper<>::template ConstPtrAt<I>;

    template<size_t I> constexpr auto ptrAt(Index<I>* = {}) const& -> ConstPtrAt<I> {
        static_assert(I < npos);
        return helper.template ptrAt<I>();
    }
    template<size_t I> constexpr auto updatePtrAt(Index<I>* = {}) & -> PtrAt<I>& {
        static_assert(I < npos);
        return helper.template ptrAt<I>();
    }

    template<class O> constexpr auto of(Type<O>* = {}) const& -> const O& {
        static_assert(has_type<O>);
        return *helper.template ptrOf<O>();
    }
    template<class O> constexpr auto updateOf(Type<O>* = {}) & -> O& {
        static_assert(has_type<O>);
        return *helper.template ptrOf<O>();
    }

    template<class F> void visitAll(F&& f) const& { (void)(f(*helper.template ptrOf<Ts>()), ...); }
    template<class F> void updateAll(F&& f) & { (void)(f(*helper.template ptrOf<Ts>()), ...); }
};

template<class... Ts> Tuple(Ts...) -> Tuple<Ts...>;

} // namespace meta

namespace test {

using namespace meta;

auto test_same() { return isSame(&type<int>, &type<int>); }

static_assert(isSame(&type<IndexSequencePackFor<int, char, float, double>>, &type<IndexPack<0, 1, 2, 3>>));

static_assert(align_offset<3, 0> == 0);
static_assert(align_offset<3, 1> == 3);
static_assert(align_offset<3, 2> == 3);
static_assert(align_offset<3, 3> == 3);
static_assert(align_offset<3, 4> == 6);

static_assert(&type<IndexPackTail<IndexPack<1>>> == &type<IndexPack<>>);

static_assert(&index_pack<0> == &align_size_align_pack<0, IndexPack<1>, IndexPack<1>>);
static_assert(&index_pack<0, 1> == &align_size_align_pack<0, IndexPack<1, 1>, IndexPack<1, 1>>);
static_assert(&index_pack<0, 2> == &align_size_align_pack<0, IndexPack<1, 1>, IndexPack<1, 2>>);
static_assert(&index_pack<0, 8, 12> == &align_size_align_pack<0, IndexPack<2, 4, 4>, IndexPack<2, 8, 4>>);

} // namespace test

using namespace meta;

template<class... Ts> //
constexpr auto resolved(TypePack<Ts...>*) {
    return sizeof...(Ts);
}

auto test_res() {
    return resolved(type_pack<int, float, char>); //
}
