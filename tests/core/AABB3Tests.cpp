#include "TestSupport.h"

#include "mnx/core/math/AABB3.h"

using namespace mnx;

TEST(AABB3Tests, EmptyBoxExpandsFromFirstPoint) {
    AABB3 box;
    box.expand(Vec3(2.0, -1.0, 5.0));
    box.expand(Vec3(-3.0, 4.0, -2.0));

    EXPECT_TRUE(box.isValid());
    EXPECT_TRUE(box.min().isApprox(Vec3(-3.0, -1.0, -2.0)));
    EXPECT_TRUE(box.max().isApprox(Vec3(2.0, 4.0, 5.0)));
    EXPECT_TRUE(box.center().isApprox(Vec3(-0.5, 1.5, 1.5)));
    EXPECT_TRUE(box.size().isApprox(Vec3(5.0, 5.0, 7.0)));
}

TEST(AABB3Tests, ContainsIntersectsAndUnionHandleEmptyBoxes) {
    const AABB3 box(Vec3(0.0, 0.0, 0.0), Vec3(2.0, 2.0, 2.0));
    const AABB3 overlap(Vec3(1.0, 1.0, 1.0), Vec3(3.0, 3.0, 3.0));
    const AABB3 separate(Vec3(4.0, 4.0, 4.0), Vec3(5.0, 5.0, 5.0));

    EXPECT_TRUE(box.contains(Vec3(1.0, 1.0, 1.0)));
    EXPECT_FALSE(box.contains(Vec3(1.0, 1.0, 3.0)));
    EXPECT_TRUE(box.intersects(overlap));
    EXPECT_FALSE(box.intersects(separate));
    EXPECT_TRUE(unite(AABB3::empty(), box).center().isApprox(Vec3(1.0, 1.0, 1.0)));
}
