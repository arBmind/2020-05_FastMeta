
Product {
    name: "041 - Partial Visit"

    Depends { name: "cpp_bench" }
    cpp_bench.inputNumber: [10, 50, 100, 150, 200] //, 250, 300, 350, 400, 450]
    // cpp_bench.showGroups: []

    Depends { name: "partial17" }
    Depends { name: "partial19" }

    // Basicpp17
    Group {
        name: "011 Basicpp: partial.visit([](){})"
        files: "011_basicpp_partial17_visit.cpp"
        cpp_bench.graphLabel: name
    }

    // Co-Cpp19
    Group {
        name: "012 Co-Cpp19: partial.visit([](){})"
        files: "012_co-cpp_partial19_visit.cpp"
        cpp_bench.graphLabel: name
    }
}
