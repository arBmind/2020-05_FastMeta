
Product {
    name: "021 - Tuple Visit"

    Depends { name: "cpp_bench" }
    cpp_bench.inputNumber: [5, 50, 100, 150, 200, 250, 300, 350, 400, 450, 500]
    cpp_bench.showGroups: ['011_std_apply_tuple']

    Depends { name: "tuple17" }
    Depends { name: "EASTL" }
    Depends { name: "boost/hana" }
    Depends { name: "tuple19" }

    // STL
    Group {
        name: "011 STL: std::apply([](...){}, tuple)"
        files: "011_std_apply_tuple.cpp"
        cpp_bench.inputNumber: [5, 50, 100, 150]
        cpp_bench.graphLabel: name
    }

    // EASTL
    Group {
        name: "021 EASTL: eastl::apply([](...){}, tuple)"
        files: "021_eastl_apply_tuple.cpp"
        cpp_bench.inputNumber: [5, 50, 100, 150]
        cpp_bench.graphLabel: name
    }

    // Boost Hana
    Group {
        name: "031 Hana: boost::hana::for_each(tuple, [](){})"
        files: "031_boost_hana_for_each.cpp"
        cpp_bench.graphLabel: name
    }
    Group {
        name: "032 Hana: boost::hana::for_each(basic_tuple, [](){})"
        files: "032_boost_hana_for_each_basic.cpp"
        cpp_bench.graphLabel: name
    }

    // Basicpp17
    Group {
        name: "041 Basicpp: tuple::visitAll([](){})"
        files: "041_basicpp_tuple17.cpp"
        cpp_bench.inputNumber: [5, 50, 100, 150]
        cpp_bench.graphLabel: name
    }

    // Co-Cpp19
    Group {
        name: "051 Co-Cpp19: tuple::visitAll([](){})"
        files: "051_co-cpp19_tuple.cpp"
        cpp_bench.graphLabel: name
    }
}
