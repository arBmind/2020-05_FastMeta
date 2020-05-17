Project {
    qbsSearchPaths: 'qbs'
    references: [
        "001_same",
        "002_conditional",
        "003_type_at",
        "004_tuple_layout",
        "005_constexpr_max",
        "010_vector",
        "011_map",
        "012_string",
        "020_tuple",
        "021_tuple_visit",
        "030_variant",
        "031_variant_visit",
        "third_party/third_party.qbs",
    ]

    // note: if do not use this .qbs project you need a similar setup
    Product {
        name: "cpp17"

        Export {
            Depends { name: "cpp" }
            cpp.cxxLanguageVersion: "c++2a"
            // cpp.treatWarningsAsErrors: true

            Properties {
                condition: qbs.toolchain.contains('msvc')
                cpp.cxxFlags: base.concat(
                    "/permissive-", "/Zc:__cplusplus", // best C++ compatibility
                    "/diagnostics:caret", // better errors
                    "/wd4068", // ignore unknown pragmas
                    "/D_ENABLE_EXTENDED_ALIGNED_STORAGE" // use real alignments
                )
            }
            Properties {
                condition: qbs.toolchain.contains('clang')
                cpp.cxxFlags: base.concat(
                    "--pedantic", // best C++ compatibility
                    "-Wall", "-Wextra", // enable more warnings
                    "-ftemplate-backtrace-limit=0", // do not cut template backtraces
                    "-Wno-gnu-zero-variadic-macro-arguments" // accept this extensions for opaque strong types
                )
                cpp.cxxStandardLibrary: "libc++"
                cpp.staticLibraries: ["c++", "c++abi"]
            }
        }
    }

    Product {
        files: [
            ".clang-format",
        ]
        name: "[extra files]"
        Group {
            name: "qbs"
            files: ["**/*.*"]
            prefix: "qbs/"
        }
    }
}
