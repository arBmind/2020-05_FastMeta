#pragma once
#include "meta19/Index.h"
#include "meta19/RemoveReference.h"
#include "meta19/Type.h"

#include <utility>

namespace tuple19 {

using meta19::Index;
using meta19::nullptr_to;
using meta19::StoredOf;
using meta19::Type;
using meta19::type;

namespace details {

template<size_t I, class T> struct TupleEntry { T v; };

template<size_t I, class T> constexpr auto entryAt(const TupleEntry<I, T>& te) -> const T& { return te.v; }
template<class T, size_t I> constexpr auto entryOf(const TupleEntry<I, T>& te) -> const T& { return te.v; }

template<size_t I, class T> constexpr auto amendEntryAt(TupleEntry<I, T>& te) -> T& { return te.v; }
template<class T, size_t I> constexpr auto amendEntryOf(TupleEntry<I, T>& te) -> T& { return te.v; }

template<size_t I, class T> constexpr auto typeAt(TupleEntry<I, T>*) -> T;
template<class T, size_t I> constexpr auto indexOf(TupleEntry<I, T>*) -> size_t { return I; }

} // namespace details

template<class... Ts> struct Tuple {

private:
    template<class Is> struct IndexedTuple;

    template<size_t... Is> struct IndexedTuple<std::index_sequence<Is...>> : details::TupleEntry<Is, Ts>... {

        constexpr IndexedTuple() = default;

        constexpr IndexedTuple(const Ts&... ts) : details::TupleEntry<Is, Ts>({ts})... {}

        constexpr IndexedTuple(Ts&&... ts) : details::TupleEntry<Is, Ts>({std::move(ts)})... {}

        template<class F> constexpr void visitAll(F&& f) const& {
            (void)((f(*static_cast<const details::TupleEntry<Is, Ts>*>(this)), ...));
        }
        template<class F> constexpr void amendAll(F&& f) & {
            (void)((f(*static_cast<details::TupleEntry<Is, Ts>*>(this)), ...));
        }
    };

    using Indexed = IndexedTuple<std::make_index_sequence<sizeof...(Ts)>>;
    enum : size_t { npos = sizeof...(Ts) };

private:
    Indexed indexed;

public:
    constexpr Tuple() = default;
    constexpr Tuple(const Ts&... ts) : indexed(ts...) {}
    constexpr Tuple(Ts&&... ts) : indexed(std::move(ts)...) {}

    template<class... Os> static constexpr auto fromArgs(Os&&... os) -> Tuple {
        auto res = Tuple{};
        ((res.amendOf(type<StoredOf<Os>>) = std::forward<Os>(os)), ...);
        return res;
    }
    template<class... Os> static constexpr auto fromTuple(const Tuple<Os...>& o) -> Tuple {
        return fromArgs(o.template of<Os>()...);
    }
    template<class... Os> static constexpr auto fromTuple(Tuple<Os...>&& o) -> Tuple {
        return fromArgs(std::move(o.template amendOf<Os>())...);
    }

    template<class O> static constexpr bool has_type = ((std::is_same_v<O, Ts>) || ...);
    template<class O> static constexpr size_t index_of = details::indexOf<O>(nullptr_to<Indexed>);
    template<size_t I> using At = decltype(details::typeAt<I>(nullptr_to<Indexed>));

    template<size_t I> constexpr auto at(Index<I>* = {}) const& -> decltype(auto) {
        static_assert(I < npos);
        return details::entryAt<I>(indexed);
    }
    template<class O> constexpr auto of(Type<O>* = {}) const& -> decltype(auto) {
        static_assert(has_type<O>);
        return details::entryOf<O>(indexed);
    }

    template<size_t I> constexpr auto amendAt(Index<I>* = {}) -> decltype(auto) {
        static_assert(I < npos);
        return details::amendEntryAt<I>(indexed);
    }
    template<class O> constexpr auto amendOf(Type<O>* = {}) -> decltype(auto) {
        static_assert(has_type<O>);
        return details::amendEntryOf<O>(indexed);
    }

    template<class F> constexpr void visitAll(F&& f) const& { indexed.visitAll(static_cast<F&&>(f)); }
    template<class F> constexpr void amendAll(F&& f) & { indexed.amendAll(static_cast<F&&>(f)); }
};

template<> struct Tuple<> {
    constexpr Tuple() = default;

    static constexpr auto fromArgs() -> Tuple { return {}; }
    static constexpr auto fromTuple(const Tuple&) -> Tuple { return {}; }

    template<class> static constexpr bool has_type = false;

    template<class F> void visitAll(F&&) const& {}
    template<class F> void amendAll(F&&) & {}
};

template<class... Ts> Tuple(Ts...) -> Tuple<Ts...>;

} // namespace tuple19
