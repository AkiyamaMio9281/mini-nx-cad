#include "TestSupport.h"

#include "mnx/core/math/Tolerance.h"

using namespace mnx;

TEST(ToleranceTests, AbsoluteToleranceHandlesValuesNearZero) {
    EXPECT_TRUE(isZero(kDefaultTolerance * 0.5));
    EXPECT_FALSE(isZero(kLooseTolerance, kDefaultTolerance));
    EXPECT_TRUE(almostEqual(0.0, kDefaultTolerance * 0.5));
}

TEST(ToleranceTests, RelativeToleranceHandlesLargeValues) {
    EXPECT_TRUE(almostEqual(1.0e9, 1.0e9 + 0.5, kDefaultTolerance, kDefaultTolerance));
    EXPECT_FALSE(almostEqual(1.0, 1.1, kDefaultTolerance, kDefaultTolerance));
}
