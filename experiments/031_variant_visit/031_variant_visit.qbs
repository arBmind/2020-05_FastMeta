
Product {
    name: "031 - Variant Visit"

    Depends { name: "cpp_bench" }
    cpp_bench.inputNumber: [10, 50, 100, 150, 200] //, 250, 300, 350, 400, 450]
    cpp_bench.showGroups: ['011_std_visit_variant']

    Depends { name: "variant17" }
    Depends { name: "mpark/variant" }
    Depends { name: "mapbox/variant" }
    Depends { name: "EASTL" }
    Depends { name: "variant19" }

    // STL
    Group {
        name: "011 STL: std::visit([](){}, variant)"
        files: "011_std_visit_variant.cpp"
        cpp_bench.graphLabel: name
    }

    // EASTL
    Group {
        name: "021 EASTL: eastl::visit([](){}, variant)"
        files: "021_eastl_visit_variant.cpp"
        cpp_bench.graphLabel: name
    }

    // Mapbox
    Group {
        name: "031 Mapbox: variant::visit(variant, [](){})"
        files: "031_mapbox_variant_visit.cpp"
        cpp_bench.graphLabel: name
    }

    // Michael Park
    Group {
        name: "041 MPark: mpark::visit([](){}, variant)"
        files: "041_mpark_visit_variant.cpp"
        cpp_bench.graphLabel: name
    }

    // Basicpp17
    Group {
        name: "051 Basicpp: variant.visit([](){})"
        files: "051_basicpp_variant17_visit.cpp"
        cpp_bench.graphLabel: name
    }

    // Co-Cpp19
    Group {
        name: "061 Co-Cpp19: variant.visit([](){})"
        files: "061_co-cpp_variant19_visit.cpp"
        cpp_bench.graphLabel: name
    }
}
