
Product {
    name: "mapbox/variant"

    files: [
        'mapbox_variant/include/**/*.hpp',
    ]

    Export {
        Depends { name: "cpp" }
        cpp.includePaths: [
            'mapbox_variant/include/',
        ]
    }
}
