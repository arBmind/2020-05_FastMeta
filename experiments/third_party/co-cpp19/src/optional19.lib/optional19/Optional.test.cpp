#include "Optional.equals.h"
#include "Optional.h"
#include "Optional.ostream.h"

#include <gtest/gtest.h>

using namespace optional19;

TEST(Optional, int) {
    using OptInt = Optional<int>;

    static_assert(sizeof(OptInt) > sizeof(int));
    {
        constexpr auto optInt = OptInt{};
        ASSERT_FALSE(optInt);
        ASSERT_EQ(optInt, OptInt{});

        ASSERT_EQ((optInt || 42), 42);

        constexpr const auto constOpt = optInt;
        ASSERT_FALSE(constOpt);
        ASSERT_EQ(constOpt || 42, 42);

        ASSERT_FALSE(constOpt && [](int v) { return v > 42; });
    }
    {
        constexpr auto optInt = OptInt{23};
        constexpr const auto constOpt = optInt;
        ASSERT_TRUE(optInt);
        //        ASSERT_EQ(constOpt, 23);

        ASSERT_EQ(optInt.value(), 23);
        ASSERT_EQ(optInt || 42, 23);

        auto l = constOpt.map([](int v) { return v * 2; });
        ASSERT_EQ(l, 23 * 2);

        ASSERT_FALSE(constOpt && [](int v) { return v > 42; });
        ASSERT_TRUE(constOpt && [](int v) { return v > 0; });
    }

    auto optInt = OptInt{};
    //    ASSERT_NE(optInt, 23);
    optInt = 23;
    //    ASSERT_EQ(optInt, 23);
}
