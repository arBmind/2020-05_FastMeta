
Product {
    name: "005 - ConstexprMax"

    Depends { name: "cpp_bench" }
    cpp_bench.inputNumber: [5, 50, 100, 150, 200, 250, 300, 350, 400, 450, 500]
    cpp_bench.showGroups: ['011_recursive_max']

    // Depends { name: "meta17" }
    // Depends { name: "meta19" }
    // Depends { name: "boost/hana" }
    // Depends { name: "kvasir/mpl" }

    Group {
        name: "ConstexprMax Benchmarks [hard limited]"
        files: [
            "011_recursive_max.cpp",
        ]
        cpp_bench.inputNumber: [5, 50, 100, 150, 200, 250, 300, 350, 400, 450]
    }
    Group {
        name: "ConstexprMax Benchmarks"
        files: [
            "012_initializer_max.cpp",
            "013_array_max.cpp",
            "014_slice_max.cpp",
            "021_foldexpr_max.cpp",
        ]
    }
}
