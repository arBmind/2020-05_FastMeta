
Product {
    name: "004 - TupleLayout"

    Depends { name: "cpp_bench" }
    cpp_bench.inputNumber: [5, 50, 100, 150, 200, 250, 300, 350, 400, 450, 500]
    // cpp_bench.showGroups: ['011_recursive_struct']

    Depends { name: "meta17" }
    // Depends { name: "meta19" }
    // Depends { name: "boost/hana" }
    // Depends { name: "kvasir/mpl" }

    Group {
        name: "TupleLayout Benchmarks [limited]"
        cpp_bench.inputNumber: [5, 50, 100, 150]
        files: [
            "011_basicpp17_alignment.cpp",
        ]
    }

    Group {
        name: "TupleLayout Benchmarks"
        files: [
            "021_constexpr_array.cpp",
        ]
    }
}
