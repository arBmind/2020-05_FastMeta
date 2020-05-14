
Product {
    name: "001 - Same"
    Depends { name: "cpp_bench" }

    cpp_bench.inputNumber: {
        if (qbs.toolchainType === 'msvc') {
            return [1000, 50000, 100000, 150000, 200000];
            // 250000 is maximum (500 types per pack), but requires >32gb RAM
            //return [1000, 50000, 100000, 150000, 200000, 250000];
        }
        if (qbs.toolchain.contains('clang')) {
            // clang seems fast and efficient
            return [1000, 50000, 100000, 150000, 200000, 250000, 300000, 350000];
        }
        if (qbs.toolchain.contains('gcc')) {
            // gcc might be very slow
            return [1000, 25000, 50000, 75000, 100000];
        }
        return [1000, 25000, 50000, 75000, 100000, 125000, 150000];
    }
    cpp_bench.showGroups: ['011_std_is_same_v']

    // STL
    Group {
        name: "011 STL: std::is_same_v<A, B>"
        files: "011_std_is_same_v.cpp"
        cpp_bench.graphLabel: name
    }
    Group {
        name: "012 STL: std::is_same<A, B>::value"
        files: "012_std_is_same.cpp"
        cpp_bench.graphLabel: name
    }
    Group {
        name: "021 STX: is_same_v = struct is_same<T, T>::value"
        files: "021_stx_is_same_v.cpp"
        cpp_bench.graphLabel: name
    }
    Group {
        name: "022 STX: struct is_same<T, T>"
        files: "022_stx_is_same.cpp"
        cpp_bench.graphLabel: name
    }

    // Variable
    Group {
        name: "031 Variable: auto is_same_v<T, T>"
        files: "031_specialized_constexpr.cpp"
        cpp_bench.graphLabel: name
    }

    // Wrapper
    Group {
        name: "041 Wrapper: Wrapper<T>::IsSame(Wrapper<T>*)"
        files: "041_wrapper.cpp"
        cpp_bench.graphLabel: name
    }

    // WrapperBase
    Group {
        name: "042 WrapperBase: struct Wrapper : WrapperBase"
        files: "042_wrapper_base.cpp"
        cpp_bench.graphLabel: name
    }
    Group {
        name: "043 WrapperBase: using is_same alias"
        files: "043_wrapper_base_alias.cpp"
        cpp_bench.graphLabel: name
    }
    Group {
        name: "044 Wrapper: decltype(Wrapper<T>::IsSame)"
        files: "044_wrapper_decltype.cpp"
        cpp_bench.graphLabel: name
    }

    // Pointer
    Group {
        name: "051 Pointer: is_same(T*, T*)"
        files: "051_ptr_is_same.cpp"
        cpp_bench.graphLabel: name
    }
    Group {
        name: "052 Pointer: is_same_v = is_same(T*, T*)"
        files: "052_ptr_is_same_v.cpp"
        cpp_bench.graphLabel: name
    }
    Group {
        name: "053 Pointer: is_same(T*, T*) / (...)"
        files: "053_ptr_is_same_varargs.cpp"
        cpp_bench.graphLabel: name
    }

    Group {
        name: "061 Type Pointer: same(&type<T>, &type<T>)"
        condition: !qbs.toolchain.contains('gcc') || qbs.toolchain.contains('clang')
        files: "061_void_ptr_same.cpp"
        cpp_bench.graphLabel: name
    }
    Group {
        name: "062 Type Pointer: same(type<T>, type<T>)"
        condition: !qbs.toolchain.contains('gcc') || qbs.toolchain.contains('clang')
        files: "062_void_ptr_ref_same.cpp"
        cpp_bench.graphLabel: name
    }
    Group {
        name: "063 Type Pointer: const type"
        condition: !qbs.toolchain.contains('gcc') || qbs.toolchain.contains('clang')
        files: "063_const_type.cpp"
        cpp_bench.graphLabel: name
    }
}
