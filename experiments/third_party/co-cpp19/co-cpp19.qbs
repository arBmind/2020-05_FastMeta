import qbs

Project {
    name: "Co-Cpp19"

    references: [
        "third_party/third_party.qbs",
        "src/array19.lib",
        "src/meta19.lib",
        "src/tuple19.lib",
        "src/variant19.lib",
    ]

    AutotestRunner {}

    // note: if do not use this .qbs project you need a similar setup
    Product {
        name: "cpp17"

        Export {
            Depends { name: "cpp" }
            cpp.cxxLanguageVersion: "c++17"
            // cpp.treatWarningsAsErrors: true

            Properties {
                condition: qbs.toolchain.contains('msvc')
                cpp.cxxFlags: base.concat(
                    "/std:c++latest", "/permissive-", "/Zc:__cplusplus", // best C++ compatibility
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
        name: "[Extra Files]"
        files: [
            ".clang-format",
        ]
    }
}
