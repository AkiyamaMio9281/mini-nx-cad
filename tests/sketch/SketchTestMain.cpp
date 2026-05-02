#include "../core/TestSupport.h"

#if !defined(MNX_HAS_GTEST)
int main() {
    return mnx::test::runAllTests();
}
#endif
