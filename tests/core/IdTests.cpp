#include "TestSupport.h"

#include "mnx/core/ids/BodyId.h"
#include "mnx/core/ids/ComponentId.h"
#include "mnx/core/ids/ConstraintId.h"
#include "mnx/core/ids/EntityId.h"
#include "mnx/core/ids/FeatureId.h"

#include <type_traits>
#include <unordered_map>

using namespace mnx;

TEST(IdTests, DefaultsAreInvalidAndExplicitValuesAreValid) {
    EntityId invalid;
    EntityId entity(42);

    EXPECT_FALSE(invalid.isValid());
    EXPECT_TRUE(entity.isValid());
    EXPECT_EQ(entity.value(), 42U);
}

TEST(IdTests, IdsAreComparableAndHashable) {
    std::unordered_map<EntityId, int> values;
    values.emplace(EntityId(7), 11);

    EXPECT_EQ(EntityId(7), EntityId(7));
    EXPECT_NE(EntityId(7), EntityId(8));
    EXPECT_EQ(values.at(EntityId(7)), 11);
}

TEST(IdTests, IdTypesRemainDistinctAtCompileTime) {
    static_assert(!std::is_same_v<EntityId, FeatureId>);
    static_assert(!std::is_same_v<ConstraintId, BodyId>);
    static_assert(!std::is_same_v<ComponentId, BodyId>);
}
