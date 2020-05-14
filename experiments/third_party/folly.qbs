
Product {
    name: "folly"

    files: [
        'folly/folly/**/*.h',
        'folly_extras/folly/*.h',
    ]

    Export {
        Depends { name: "cpp" }
        cpp.includePaths: [
            'folly',
            'folly_extras',
        ]
    }
}