
Product {
    Depends { name: "cpp17" }

    files: [
        "*.h",
    ]

    Export {
        Depends { name: "cpp" }
        cpp.includePaths: [".."]
        Depends { name: "cpp17" }
    }
}
