
Product {
    name: "030 - Variant"

    Depends { name: "cpp_bench" }
    cpp_bench.inputNumber: [10, 50, 100, 150, 200, 250, 300, 350, 400, 450]
    cpp_bench.showGroups: ['012_std_variant']

    Depends { name: "variant17" }
    Depends { name: "mpark/variant" }
    Depends { name: "mapbox/variant" }
    Depends { name: "EASTL" }
    Depends { name: "meta19" }
    Depends { name: "variant19" }

    // STL
    Group {
        name: "011 STL: using std::variant<T...>"
        files: "011_using_std_variant.cpp"
        cpp_bench.graphLabel: name
    }
    Group {
        name: "012 STL: std::variant<T...>{}"
        files: "012_std_variant.cpp"
        cpp_bench.graphLabel: name
    }

    // EASTL
    Group {
        name: "021 EASTL: eastl::variant<T...>{}"
        files: "021_eastl_variant.cpp"
        cpp_bench.graphLabel: name
    }

    // Mapbox
    Group {
        name: "031 Mapbox: mapbox::util::variant<T...>{}"
        files: "031_mapbox_variant.cpp"
        cpp_bench.graphLabel: name
    }

    // Michael Park
    Group {
        name: "041 MPark: mpark::variant<T...>{}"
        files: "041_mpark_variant.cpp"
        cpp_bench.graphLabel: name
    }

    // Basicpp17
    Group {
        name: "051 Basicpp: variant17::Variant<T...>{}"
        files: "051_basicpp_variant17.cpp"
        cpp_bench.graphLabel: name
    }

    // Co-Cpp19
    Group {
        name: "061 Co-Cpp19: variant19::Variant<T...>{}"
        files: "061_co-cpp_variant19.cpp"
        cpp_bench.graphLabel: name
    }
    Group {
        name: "062 Experimental: Variant<T...>{}"
        files: "062_experimental_variant.cpp"
        cpp_bench.graphLabel: name
    }
}
