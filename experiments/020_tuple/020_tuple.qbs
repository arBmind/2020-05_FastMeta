
Product {
    name: "020 - Tuple"

    Depends { name: "cpp_bench" }
    cpp_bench.inputNumber: [5, 50, 100, 150, 200, 250, 300, 350, 400, 450, 500]
    cpp_bench.showGroups: ['012_std_tuple']

    Depends { name: "tuple17" }
    Depends { name: "EASTL" }
    Depends { name: "boost/hana" }
    Depends { name: "tuple19" }

    // STL
    Group {
        name: "011 STL: using std::tuple<T...>"
        files: "011_using_std_tuple.cpp"
        cpp_bench.graphLabel: name
        cpp_bench.inputNumber: [5, 50, 100, 150, 200, 250, 300, 350, 400, 450]
    }
    Group {
        name: "012 STL: std::tuple<T...>{}"
        files: "012_std_tuple.cpp"
        cpp_bench.graphLabel: name
        cpp_bench.inputNumber: [5, 50, 100, 150, 200, 250, 300, 350, 400, 450]
    }

    // EASTL
    Group {
        name: "021 EASTL: eastl::tuple<T...>{}"
        files: "021_eastl_tuple.cpp"
        cpp_bench.graphLabel: name
        cpp_bench.inputNumber: [5, 50, 100, 150, 200, 250, 300, 350, 400, 450]
    }

    // Boost Hana
    Group {
        name: "031 Hana: boost::hana::tuple<T...>{}"
        files: "031_boost_hana_tuple.cpp"
        cpp_bench.graphLabel: name
    }
    Group {
        name: "032 Hana: boost::hana::basic_tuple<T...>{}"
        files: "032_boost_hana_basic_tuple.cpp"
        cpp_bench.graphLabel: name
    }

    // Basicpp17
    Group {
        name: "041 Basicpp: tuple17::Tuple<T...>{}"
        files: "041_basicpp_tuple17.cpp"
        cpp_bench.graphLabel: name
        cpp_bench.inputNumber: [5, 50, 100, 150]
    }
    Group {
        name: "042 FastLayout: Tuple<T...>{}"
        files: "042_fast_layout_tuple.cpp"
        cpp_bench.graphLabel: name
    }

    // Co-Cpp19
    Group {
        name: "051 Co-Cpp19: tuple19::Tuple<T...>{}"
        files: "051_co-cpp19_tuple.cpp"
        cpp_bench.graphLabel: name
    }
}
