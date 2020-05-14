
Product {
    name: "kvasir/mpl"

    files: [
        'kvasir_mpl/src/**/*.hpp',
    ]

    Export {
        Depends { name: "cpp" }
        cpp.includePaths: [
            'kvasir_mpl/src/',
        ]
    }
}
