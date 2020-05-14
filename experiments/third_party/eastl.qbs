
Product {
    name: "EASTL"

    files: [
        "EASTL/include/EASTL/**/*.h"
    ]

    Export {
        Depends { name: "cpp" }
        cpp.includePaths: [
            'EASTL/include/',
            'EASTL/test/packages/EABase/include/Common/',
        ]
    }
}
