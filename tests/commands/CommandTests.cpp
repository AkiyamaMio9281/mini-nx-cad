#include "../core/TestSupport.h"

#include "mnx/commands/AddConstraintCommand.h"
#include "mnx/commands/CommandManager.h"
#include "mnx/commands/CreateSketchEntityCommand.h"
#include "mnx/commands/DeleteSketchEntityCommand.h"
#include "mnx/commands/EditDimensionCommand.h"
#include "mnx/commands/MoveSketchHandleCommand.h"
#include "mnx/commands/RemoveConstraintCommand.h"
#include "mnx/constraints/Dimension.h"
#include "mnx/constraints/HorizontalConstraint.h"
#include "mnx/sketch/SketchLine.h"

#include <memory>
#include <vector>

using namespace mnx;

TEST(CommandTests, CreateLineUndoRedo) {
    Sketch sketch;
    CommandManager manager;
    auto line = std::make_unique<SketchLine>(EntityId(1), Point2(0.0, 0.0), Point2(1.0, 0.0));

    EXPECT_TRUE(manager.execute(std::make_unique<CreateSketchEntityCommand>(sketch, std::move(line))).isOk());
    EXPECT_EQ(sketch.entityCount(), 1U);
    EXPECT_TRUE(manager.canUndo());
    EXPECT_TRUE(manager.undo().isOk());
    EXPECT_EQ(sketch.entityCount(), 0U);
    EXPECT_TRUE(manager.canRedo());
    EXPECT_TRUE(manager.redo().isOk());
    EXPECT_EQ(sketch.entityCount(), 1U);
}

TEST(CommandTests, DeleteEntityUndoRedo) {
    Sketch sketch;
    CommandManager manager;
    SketchLine& line = sketch.addLine(Point2(0.0, 0.0), Point2(1.0, 0.0));
    const EntityId id = line.id();

    EXPECT_TRUE(manager.execute(std::make_unique<DeleteSketchEntityCommand>(sketch, id)).isOk());
    EXPECT_TRUE(sketch.findEntity(id) == nullptr);
    EXPECT_TRUE(manager.undo().isOk());
    EXPECT_TRUE(sketch.findEntity(id) != nullptr);
    EXPECT_TRUE(manager.redo().isOk());
    EXPECT_TRUE(sketch.findEntity(id) == nullptr);
}

TEST(CommandTests, MoveSketchHandleUndoRedo) {
    Sketch sketch;
    CommandManager manager;
    SketchLine& line = sketch.addLine(Point2(0.0, 0.0), Point2(1.0, 0.0));
    const EntityId id = line.id();

    EXPECT_TRUE(
        manager
            .execute(std::make_unique<MoveSketchHandleCommand>(
                sketch,
                GeometryRef::endPoint(id),
                Point2(1.0, 0.0),
                Point2(2.0, 3.0)
            ))
            .isOk()
    );
    EXPECT_TRUE(static_cast<const SketchLine*>(sketch.findEntity(id))->end().isApprox(Point2(2.0, 3.0)));
    EXPECT_TRUE(manager.undo().isOk());
    EXPECT_TRUE(static_cast<const SketchLine*>(sketch.findEntity(id))->end().isApprox(Point2(1.0, 0.0)));
    EXPECT_TRUE(manager.redo().isOk());
    EXPECT_TRUE(static_cast<const SketchLine*>(sketch.findEntity(id))->end().isApprox(Point2(2.0, 3.0)));
}

TEST(CommandTests, AddConstraintUndoRedo) {
    Sketch sketch;
    CommandManager manager;
    std::vector<std::unique_ptr<Constraint>> constraints;
    SketchLine& line = sketch.addLine(Point2(0.0, 0.0), Point2(1.0, 0.0));

    auto constraint = std::make_unique<HorizontalConstraint>(ConstraintId(1), GeometryRef::whole(line.id()));
    EXPECT_TRUE(
        manager.execute(std::make_unique<AddConstraintCommand>(sketch, constraints, std::move(constraint))).isOk()
    );
    EXPECT_EQ(constraints.size(), 1U);
    EXPECT_TRUE(sketch.hasConstraintId(ConstraintId(1)));
    EXPECT_TRUE(manager.undo().isOk());
    EXPECT_TRUE(constraints.empty());
    EXPECT_FALSE(sketch.hasConstraintId(ConstraintId(1)));
    EXPECT_TRUE(manager.redo().isOk());
    EXPECT_EQ(constraints.size(), 1U);
    EXPECT_TRUE(sketch.hasConstraintId(ConstraintId(1)));
}

TEST(CommandTests, RemoveConstraintUndoRedo) {
    Sketch sketch;
    CommandManager manager;
    std::vector<std::unique_ptr<Constraint>> constraints;
    SketchLine& line = sketch.addLine(Point2(0.0, 0.0), Point2(1.0, 0.0));
    constraints.push_back(std::make_unique<HorizontalConstraint>(ConstraintId(1), GeometryRef::whole(line.id())));
    sketch.addConstraintId(ConstraintId(1));

    EXPECT_TRUE(
        manager.execute(std::make_unique<RemoveConstraintCommand>(sketch, constraints, ConstraintId(1))).isOk()
    );
    EXPECT_TRUE(constraints.empty());
    EXPECT_FALSE(sketch.hasConstraintId(ConstraintId(1)));
    EXPECT_TRUE(manager.undo().isOk());
    EXPECT_EQ(constraints.size(), 1U);
    EXPECT_TRUE(sketch.hasConstraintId(ConstraintId(1)));
    EXPECT_TRUE(manager.redo().isOk());
    EXPECT_TRUE(constraints.empty());
}

TEST(CommandTests, EditDimensionUndoRedo) {
    CommandManager manager;
    Dimension dimension(ConstraintId(1), DimensionDisplayKind::Distance, Point2(0.0, 0.0), "old");
    Dimension edited(ConstraintId(1), DimensionDisplayKind::Distance, Point2(2.0, 3.0), "new");

    EXPECT_TRUE(manager.execute(std::make_unique<EditDimensionCommand>(dimension, dimension, edited)).isOk());
    EXPECT_EQ(dimension.label(), "new");
    EXPECT_TRUE(dimension.labelPosition().isApprox(Point2(2.0, 3.0)));
    EXPECT_TRUE(manager.undo().isOk());
    EXPECT_EQ(dimension.label(), "old");
    EXPECT_TRUE(dimension.labelPosition().isApprox(Point2(0.0, 0.0)));
    EXPECT_TRUE(manager.redo().isOk());
    EXPECT_EQ(dimension.label(), "new");
}
