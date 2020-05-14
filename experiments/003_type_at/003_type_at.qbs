
Product {
    name: "003 - TypeAt"

    Depends { name: "cpp_bench" }
    cpp_bench.inputNumber: [5, 50, 100, 150] // , 200, 250, 300
    cpp_bench.showGroups: ['011_recursive_struct']

    Depends { name: "meta17" }
    Depends { name: "meta19" }
    Depends { name: "boost/hana" }
    Depends { name: "kvasir/mpl" }

    Group {
        name: "TypeAt Benchmarks"
        files: [
            "011_recursive_struct.cpp",
            "021_type_combiner.cpp",
            "022_type_combiner_opt.cpp",
            "031_boost_hana_basic_tuple.cpp",
            "032_kvasir_mpl_lookup.cpp",
            "033_kvasir_mpl_at.cpp",
            "041_fold_unwrap.cpp",
            "042_fold_struct.cpp",
            "043_fold_opt.cpp",
            "051_index_type_combo.cpp",
            "052_index_type_func.cpp",
        ]
    }
}
