#if defined(MNX_HAS_GTEST)
#include <gtest/gtest.h>

TEST(Phase0SmokeTests, TestHarnessRuns) {
    SUCCEED();
}
#else
int main() {
    return 0;
}
#endif
