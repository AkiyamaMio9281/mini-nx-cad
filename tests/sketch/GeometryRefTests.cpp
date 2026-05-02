#include "../core/TestSupport.h"

#include "mnx/sketch/SelectionState.h"
#include "mnx/sketch/Sketch.h"

using namespace mnx;

TEST(GeometryRefTests, GeometryRefsResolveSupportedSubEntities) {
    Sketch sketch;
    const SketchLine& line = sketch.addLine(Point2(0.0, 0.0), Point2(4.0, 0.0));
    const SketchCircle& circle = sketch.addCircle(Point2(2.0, 3.0), 1.0);

    EXPECT_TRUE(sketch.resolvePoint(GeometryRef::startPoint(line.id())).value().isApprox(Point2(0.0, 0.0)));
    EXPECT_TRUE(sketch.resolvePoint(GeometryRef::endPoint(line.id())).value().isApprox(Point2(4.0, 0.0)));
    EXPECT_TRUE(sketch.resolvePoint(GeometryRef::midpoint(line.id())).value().isApprox(Point2(2.0, 0.0)));
    EXPECT_TRUE(sketch.resolvePoint(GeometryRef::centerPoint(circle.id())).value().isApprox(Point2(2.0, 3.0)));
}

TEST(GeometryRefTests, UnsupportedOrMissingRefsReturnFailures) {
    Sketch sketch;
    const SketchLine& line = sketch.addLine(Point2(0.0, 0.0), Point2(4.0, 0.0));

    EXPECT_FALSE(sketch.supportsRef(GeometryRef::centerPoint(line.id())));
    EXPECT_FALSE(sketch.resolvePoint(GeometryRef::centerPoint(line.id())).isOk());
    EXPECT_FALSE(sketch.resolvePoint(GeometryRef::startPoint(EntityId(99))).isOk());
}

TEST(GeometryRefTests, SelectionStateKeepsUniqueRefsInOrder) {
    SelectionState selection;
    const GeometryRef first = GeometryRef::whole(EntityId(1));
    const GeometryRef second = GeometryRef::startPoint(EntityId(2));

    selection.select(first);
    selection.select(first);
    selection.select(second);

    EXPECT_EQ(selection.refs().size(), 2U);
    EXPECT_TRUE(selection.contains(first));
    selection.deselect(first);
    EXPECT_FALSE(selection.contains(first));
    EXPECT_TRUE(selection.contains(second));
    selection.clear();
    EXPECT_TRUE(selection.empty());
}
