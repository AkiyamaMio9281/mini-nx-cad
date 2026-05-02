#include "../core/TestSupport.h"

#include "mnx/constraints/Dimension.h"
#include "mnx/constraints/DistanceConstraint.h"

using namespace mnx;

TEST(ConstraintMetadataTests, BaseMetadataIsInspectable) {
    DistanceConstraint constraint(
        ConstraintId(7),
        GeometryRef::startPoint(EntityId(1)),
        GeometryRef::endPoint(EntityId(2)),
        12.5,
        false
    );

    EXPECT_EQ(constraint.id(), ConstraintId(7));
    EXPECT_EQ(constraint.type(), ConstraintType::Distance);
    EXPECT_FALSE(constraint.isDriving());
    EXPECT_EQ(constraint.refs().size(), 2U);
    EXPECT_EQ(constraint.distance(), 12.5);
    EXPECT_FALSE(constraint.debugDescription().empty());

    constraint.setLabel("slot width");
    EXPECT_EQ(constraint.label(), "slot width");
}

TEST(ConstraintMetadataTests, DimensionStoresDisplayMetadataOnly) {
    Dimension dimension(ConstraintId(7), DimensionDisplayKind::Distance, Point2(1.0, 2.0), "12.5");

    EXPECT_EQ(dimension.constraintId(), ConstraintId(7));
    EXPECT_EQ(dimension.displayKind(), DimensionDisplayKind::Distance);
    EXPECT_TRUE(dimension.labelPosition().isApprox(Point2(1.0, 2.0)));
    EXPECT_EQ(dimension.label(), "12.5");

    dimension.setLabelPosition(Point2(3.0, 4.0));
    dimension.setLabel("width");
    EXPECT_TRUE(dimension.labelPosition().isApprox(Point2(3.0, 4.0)));
    EXPECT_EQ(dimension.label(), "width");
}
