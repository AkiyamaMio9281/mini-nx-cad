#include "TestSupport.h"

#include "mnx/core/utils/Result.h"

#include <stdexcept>

using namespace mnx;

TEST(StatusResultTests, StatusCarriesCodeAndMessage) {
    const Status ok = Status::ok();
    const Status error = Status::invalidArgument("bad input");

    EXPECT_TRUE(ok.isOk());
    EXPECT_FALSE(error.isOk());
    EXPECT_EQ(error.code(), StatusCode::InvalidArgument);
    EXPECT_EQ(error.message(), "bad input");
}

TEST(StatusResultTests, ResultCarriesValueOrFailureStatus) {
    const Result<int> value(12);
    const Result<int> failure = Result<int>::failure(Status::notFound("missing"));

    EXPECT_TRUE(value.isOk());
    EXPECT_EQ(value.value(), 12);
    EXPECT_FALSE(failure.isOk());
    EXPECT_EQ(failure.status().code(), StatusCode::NotFound);
    EXPECT_THROW(failure.value(), std::logic_error);
}
