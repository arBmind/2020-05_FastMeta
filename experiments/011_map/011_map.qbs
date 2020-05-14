
Product {
    name: "011 - Map"

    Depends { name: "cpp_bench" }
    cpp_bench.inputNumber: [5, 50, 100, 150]
    cpp_bench.showGroups: ['012_std_map']

    Depends { name: "etl" }
    Depends { name: "folly" }
    Depends { name: "EASTL" }

    Group {
        name: "011 STL: using std::map<K, V>"
        files: "011_using_std_map.cpp"
        cpp_bench.graphLabel: name
    }
    Group {
        name: "012 STL: std::map<K, V>{}"
        files: "012_std_map.cpp"
        cpp_bench.graphLabel: name
    }
    Group {
        name: "021 ETL: etl::flat_map<K, V, 1024>{}"
        files: "021_etl_flat_map.cpp"
        cpp_bench.graphLabel: name
    }
    Group {
        name: "031 Folly: folly::sorted_vector_map<K, V>{}"
        files: "031_folly_sorted_vector_map.cpp"
        cpp_bench.graphLabel: name
    }
    Group {
        name: "041 EASTL: eastl::vector_map<K, V>{}"
        files: "041_eastl_vector_map.cpp"
        cpp_bench.graphLabel: name
    }
}
