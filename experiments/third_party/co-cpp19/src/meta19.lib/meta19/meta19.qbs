
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
            "IndexPack.h",
            "IndexPackFront.h",
            "IndexPackJoin.h",
            "IndexPackTail.h",
            "IndexPackTemplate.h",
            "ChangeIndexPackTemplate.h",
            "index_pack.h",
            "index_pack_back_value.h",
            "index_pack_front.h",
            "index_pack_front_value.h",
            "index_pack_tail.h",
            "index_pack_to_array.h",
        ]
    }
    Group {
        name: "Type"
        files: [
            "RemoveReference.h",
            "Type.h",
            "TypeAt.h",
            "UnwrapType.h",
        ]
    }
    Group {
        name: "TypePack"
        files: [
            "ChangeTypePackTemplate.h",
            "TypePack.h",
            "TypePackTemplate.h",
            "type_pack.h",
            "type_pack_front_type.h",
        ]
    }

    files: [
        "Unreachable.h",
        "isSame.h",
        "nulltpr_to.h",
    ]

    Export {
        Depends { name: "cpp" }
        cpp.includePaths: [".."]
        Depends { name: "array19" }
    }
}
