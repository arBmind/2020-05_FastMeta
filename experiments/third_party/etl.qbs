
Product {
    name: "etl"

    files: [
        "etl/include/etl/**/*.h"
    ]

    Export {
        Depends { name: "cpp" }
        cpp.includePaths: [
            'etl/include/',
            'etl/test/', // use test "etl_platform.h"
        ]
        // cpp.defines: [
        //     "ETL_CONSTEXPR",
        //     "ETL_CONSTEXPR14",
        //     "ETL_CONSTEXPR17",
        //     "ETL_IF_CONSTEXPR",
        //     "ETL_DELETE",
        //     "ETL_NOEXCEPT",
        //     "ETL_NOEXCEPT_EXPR",
        //     "ETL_NODISCARD",
        //     "ETL_OVERRIDE",
        //     "ETL_FINAL",
        //     "ETL_DEPRECATED",
        //     "ETL_ERROR_TEXT",
        //     "ETL_ASSERT",
        //     "ETL_ERROR",
        //     "ETL_VERBOSE_ERRORS",
        //     "ETL_FILE",
        //     "ETL_NULLPTR",
        //     "ETL_CPP11_SUPPORTED",
        //     "ETL_CPP14_SUPPORTED",
        //     "ETL_CPP17_SUPPORTED",
        // ]
    }
}
