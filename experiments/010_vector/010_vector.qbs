
Product {
    name: "010 - Vector"

    Depends { name: "cpp_bench" }
    cpp_bench.inputNumber: [5, 50, 100, 150]
    cpp_bench.showGroups: ['012_std_vector']

    Depends { name: "etl" }
    Depends { name: "folly" }
    Depends { name: "EASTL" }
    Depends { name: "array19" }

    Group {
        name: "011 STL: using std::vector<T>"
        files: "011_using_std_vector.cpp"
        cpp_bench.graphLabel: name
    }
    Group {
        name: "012 STL: std::vector<T>{}"
        files: "012_std_vector.cpp"
        cpp_bench.graphLabel: name
    }
    Group {
        name: "021 ETL: etl::vector<T, 1024>{}"
        files: "021_etl_vector.cpp"
        cpp_bench.graphLabel: name
    }
    Group {
        name: "031 Folly: folly::fbvector<T>{}"
        files: "031_folly_fbvector.cpp"
        cpp_bench.graphLabel: name
    }
    Group {
        name: "041 EASTL: eastl::vector<T>{}"
        files: "041_eastl_vector.cpp"
        cpp_bench.graphLabel: name
    }
    Group {
        name: "051 Co-Cpp19: array19::DynamicArrayOf<T>{}"
        files: "051_co-cpp19_dynamic_array.cpp"
        cpp_bench.graphLabel: name
    }
}
