
Product {
    name: "boost/hana"

    files: [
        'boost_hana/include/**/*.hpp',
    ]

    Export {
        Depends { name: "cpp" }
        cpp.includePaths: [
            'boost_hana/include/',
        ]
    }
}
