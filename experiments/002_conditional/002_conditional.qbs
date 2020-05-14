
Product {
    name: "002 - Conditional"
    Depends { name: "cpp_bench" }

    cpp_bench.inputNumber: {
        if (qbs.toolchainType === 'msvc') {
            return [1000, 25000, 50000, 75000, 10000];
            // 250000 is maximum (500 types per pack)
            //return [1000, 50000, 100000, 150000, 200000, 250000];
        }
        if (qbs.toolchain.contains('clang')) {
            // clang seems fast and efficient
            return [1000, 25000, 50000, 75000, 100000, 125000, 150000, 175000];
        }
        if (qbs.toolchain.contains('gcc')) {
            // gcc might be very slow
            return [1000, 25000, 50000, 75000, 100000];
        }
        return [1000, 25000, 50000, 75000, 100000, 125000, 150000];
    }
    cpp_bench.showGroups: ['011_std_conditional_t']

    // STL
    Group {
        name: "011 STL: std::conditional_t<C, T, F>"
        files: "011_std_conditional_t.cpp"
        cpp_bench.graphLabel: name
    }
    Group {
        name: "012 STL: std::conditional<C, T, F>::value"
        files: "012_std_conditional.cpp"
        cpp_bench.graphLabel: name
    }

    // Nested
    Group {
        name: "021 Nested: conditional_t = conditional<C>::type<T, F>"
        files: "021_nested_using_t.cpp"
        cpp_bench.graphLabel: name
    }
    Group {
        name: "022 Nested: typename conditional<C>::template type<T, F>"
        condition: !qbs.toolchain.contains('gcc') || qbs.toolchain.contains('clang') // bug in gcc
        files: "022_nested_using.cpp"
        cpp_bench.graphLabel: name
    }

    // Other Experiments
    Group {
        name: "031 Function: if constexpr (C)"
        files: "031_signature.cpp"
        cpp_bench.graphLabel: name
    }
    Group {
        name: "032 Function: tag overloads"
        files: "032_tag_signature.cpp"
        cpp_bench.graphLabel: name
    }
    Group {
        name: "033 Constexpr: decltype(conditional<C, T, F>)"
        files: "033_constexpr.cpp"
        cpp_bench.graphLabel: name
    }
}
