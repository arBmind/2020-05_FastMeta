
Product {
    Depends { name: "array19" }

    Group {
        name: "Index"
        files: [
            "Index.h",
            "index_value.h",
        ]
    }
    Group {
        name: "IndexPack"
        files: [
            "IndexPack.Join.h",
            "IndexPack.h",
            "IndexPackTemplate.Change.h",
            "IndexPackTemplate.h",
            "index_pack.back.h",
            "index_pack.back.test.cpp",
            "index_pack.front.h",
            "index_pack.front.test.cpp",
            "index_pack.h",
            "index_pack.tail.h",
            "index_pack.to_array.h",
            "index_pack.to_array.test.cpp",
        ]
    }
    Group {
        name: "Type"
        files: [
            "RemoveReference.h",
            "Type.Unwrap.h",
            "Type.h",
            "TypeAt.h",
        ]
    }
    Group {
        name: "TypePack"
        files: [
            "TypePack.Front.h",
            "TypePack.h",
            "TypePackTemplate.Change.h",
            "TypePackTemplate.h",
        ]
    }

    files: [
        "Unreachable.h",
        "isSame.h",
        "nullptr_to.h",
    ]

    Export {
        Depends { name: "cpp" }
        cpp.includePaths: [".."]
        Depends { name: "array19" }
    }
}
