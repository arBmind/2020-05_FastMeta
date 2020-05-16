
Product {
    Depends { name: "cpp" }
    Depends { name: "meta19" }

    files: [
        "Tuple.h",
    ]

    Export {
        Depends { name: "cpp" }
        cpp.includePaths: [".."]
        Depends { name: "meta19" }
    }
}
