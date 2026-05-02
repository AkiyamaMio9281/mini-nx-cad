#include "TestSupport.h"

#include "mnx/core/math/AABB2.h"

using namespace mnx;

TEST(AABB2Tests, EmptyBoxExpandsFromFirstPoint) {
    AABB2 box;
    EXPECT_FALSE(box.isValid());

    box.expand(Vec2(2.0, -1.0));
    box.expand(Vec2(-3.0, 4.0));

    EXPECT_TRUE(box.isValid());
    EXPECT_TRUE(box.min().isApprox(Vec2(-3.0, -1.0)));
    EXPECT_TRUE(box.max().isApprox(Vec2(2.0, 4.0)));
    EXPECT_TRUE(box.center().isApprox(Vec2(-0.5, 1.5)));
    EXPECT_TRUE(box.size().isApprox(Vec2(5.0, 5.0)));
}

TEST(AABB2Tests, ContainsIntersectsAndUnionHandleEmptyBoxes) {
    const AABB2 box(Vec2(0.0, 0.0), Vec2(2.0, 2.0));
    const AABB2 overlap(Vec2(1.0, 1.0), Vec2(3.0, 3.0));
    const AABB2 separate(Vec2(4.0, 4.0), Vec2(5.0, 5.0));

    EXPECT_TRUE(box.contains(Vec2(1.0, 1.0)));
    EXPECT_FALSE(box.contains(Vec2(3.0, 1.0)));
    EXPECT_TRUE(box.intersects(overlap));
    EXPECT_FALSE(box.intersects(separate));
    EXPECT_TRUE(unite(AABB2::empty(), box).center().isApprox(Vec2(1.0, 1.0)));
}
