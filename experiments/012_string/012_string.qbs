
Product {
    name: "012 - String"

    Depends { name: "cpp_bench" }
    cpp_bench.inputNumber: [5, 50, 100, 150]
    // cpp_bench.showGroups: ['011_std_vector']

    // Depends { name: "etl" }
    // Depends { name: "folly" }
    // Depends { name: "EASTL" }
    // Depends { name: "array19" }

    Group {
        name: "String Benchmarks"
        files: [
            "011_std_string.cpp",
        ]
    }
}
