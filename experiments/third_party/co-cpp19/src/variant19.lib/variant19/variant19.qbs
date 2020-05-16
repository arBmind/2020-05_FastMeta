
Product {
    Depends { name: "cpp" }
    Depends { name: "meta19" }

    files: [
        "Variant.h",
        "Variant.ops.h",
    ]

    Export {
        Depends { name: "cpp" }
        cpp.includePaths: [".."]
        Depends { name: "meta19" }
    }
}
