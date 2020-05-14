
Product {
    name: "mpark/variant"

    files: [
        'mpark_variant/include/**/*.hpp',
    ]

    Export {
        Depends { name: "cpp" }
        cpp.includePaths: [
            'mpark_variant/include/',
        ]
    }
}
