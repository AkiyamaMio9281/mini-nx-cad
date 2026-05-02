#include "../core/TestSupport.h"

#include "mnx/constraints/AngleConstraint.h"
#include "mnx/constraints/CoincidentConstraint.h"
#include "mnx/constraints/DistanceConstraint.h"
#include "mnx/constraints/EqualLengthConstraint.h"
#include "mnx/constraints/HorizontalConstraint.h"
#include "mnx/constraints/ParallelConstraint.h"
#include "mnx/constraints/PerpendicularConstraint.h"
#include "mnx/constraints/RadiusConstraint.h"
#include "mnx/constraints/TangentConstraint.h"
#include "mnx/constraints/VerticalConstraint.h"

#include <stdexcept>

using namespace mnx;

TEST(ConstraintValidationTests, CoincidentRequiresExistingPointLikeRefs) {
    Sketch sketch;
    const auto& first = sketch.addLine(Point2(0.0, 0.0), Point2(1.0, 0.0));
    const auto& second = sketch.addCircle(Point2(2.0, 0.0), 1.0);

    const CoincidentConstraint valid(
        ConstraintId(1),
        GeometryRef::endPoint(first.id()),
        GeometryRef::centerPoint(second.id())
    );
    const CoincidentConstraint unsupported(
        ConstraintId(2),
        GeometryRef::whole(first.id()),
        GeometryRef::centerPoint(second.id())
    );
    const CoincidentConstraint missing(
        ConstraintId(3),
        GeometryRef::startPoint(EntityId(999)),
        GeometryRef::centerPoint(second.id())
    );

    EXPECT_TRUE(valid.validate(sketch).isOk());
    EXPECT_EQ(unsupported.validate(sketch).code(), StatusCode::InvalidArgument);
    EXPECT_EQ(missing.validate(sketch).code(), StatusCode::NotFound);
}

TEST(ConstraintValidationTests, DirectionConstraintsRequireWholeLineRefs) {
    Sketch sketch;
    const auto& line = sketch.addLine(Point2(0.0, 0.0), Point2(1.0, 0.0));
    const auto& circle = sketch.addCircle(Point2(0.0, 0.0), 1.0);

    EXPECT_TRUE(HorizontalConstraint(ConstraintId(1), GeometryRef::whole(line.id())).validate(sketch).isOk());
    EXPECT_TRUE(VerticalConstraint(ConstraintId(2), GeometryRef::whole(line.id())).validate(sketch).isOk());
    EXPECT_EQ(
        HorizontalConstraint(ConstraintId(3), GeometryRef::centerPoint(circle.id())).validate(sketch).code(),
        StatusCode::InvalidArgument
    );
    EXPECT_EQ(
        VerticalConstraint(ConstraintId(4), GeometryRef::whole(circle.id())).validate(sketch).code(),
        StatusCode::InvalidArgument
    );
}

TEST(ConstraintValidationTests, LinePairConstraintsRequireWholeLines) {
    Sketch sketch;
    const auto& first = sketch.addLine(Point2(0.0, 0.0), Point2(1.0, 0.0));
    const auto& second = sketch.addLine(Point2(0.0, 1.0), Point2(1.0, 1.0));
    const auto& circle = sketch.addCircle(Point2(2.0, 0.0), 1.0);

    EXPECT_TRUE(ParallelConstraint(ConstraintId(1), GeometryRef::whole(first.id()), GeometryRef::whole(second.id()))
                    .validate(sketch)
                    .isOk());
    EXPECT_TRUE(
        PerpendicularConstraint(ConstraintId(2), GeometryRef::whole(first.id()), GeometryRef::whole(second.id()))
            .validate(sketch)
            .isOk()
    );
    EXPECT_TRUE(EqualLengthConstraint(ConstraintId(3), GeometryRef::whole(first.id()), GeometryRef::whole(second.id()))
                    .validate(sketch)
                    .isOk());
    EXPECT_EQ(
        AngleConstraint(ConstraintId(4), GeometryRef::whole(first.id()), GeometryRef::whole(circle.id()), 0.5)
            .validate(sketch)
            .code(),
        StatusCode::InvalidArgument
    );
}

TEST(ConstraintValidationTests, DistanceRadiusAndTangentValidateExpectedRefs) {
    Sketch sketch;
    const auto& line = sketch.addLine(Point2(0.0, 0.0), Point2(1.0, 0.0));
    const auto& circle = sketch.addCircle(Point2(2.0, 0.0), 1.0);
    const auto& arc = sketch.addArc(Point2(4.0, 0.0), 1.0, 0.0, 1.0);
    const auto& point = sketch.addPoint(Point2(6.0, 0.0));

    EXPECT_TRUE(
        DistanceConstraint(ConstraintId(1), GeometryRef::startPoint(line.id()), GeometryRef::centerPoint(circle.id()), 2.0)
            .validate(sketch)
            .isOk()
    );
    EXPECT_TRUE(RadiusConstraint(ConstraintId(2), GeometryRef::whole(circle.id()), 1.0).validate(sketch).isOk());
    EXPECT_TRUE(RadiusConstraint(ConstraintId(3), GeometryRef::whole(arc.id()), 1.0).validate(sketch).isOk());
    EXPECT_TRUE(TangentConstraint(ConstraintId(4), GeometryRef::whole(line.id()), GeometryRef::whole(circle.id()))
                    .validate(sketch)
                    .isOk());
    EXPECT_EQ(
        RadiusConstraint(ConstraintId(5), GeometryRef::whole(line.id()), 1.0).validate(sketch).code(),
        StatusCode::InvalidArgument
    );
    EXPECT_EQ(
        TangentConstraint(ConstraintId(6), GeometryRef::whole(point.id()), GeometryRef::whole(circle.id()))
            .validate(sketch)
            .code(),
        StatusCode::InvalidArgument
    );
}

TEST(ConstraintValidationTests, NumericConstraintsRejectNegativeValues) {
    EXPECT_THROW(
        DistanceConstraint(ConstraintId(1), GeometryRef::startPoint(EntityId(1)), GeometryRef::endPoint(EntityId(1)), -1.0),
        std::invalid_argument
    );
    EXPECT_THROW(RadiusConstraint(ConstraintId(2), GeometryRef::whole(EntityId(1)), -1.0), std::invalid_argument);
}
