#include "../core/TestSupport.h"

#include "mnx/constraints/CoincidentConstraint.h"
#include "mnx/constraints/DistanceConstraint.h"
#include "mnx/constraints/HorizontalConstraint.h"
#include "mnx/constraints/ParallelConstraint.h"
#include "mnx/constraints/PerpendicularConstraint.h"
#include "mnx/constraints/RadiusConstraint.h"
#include "mnx/constraints/TangentConstraint.h"
#include "mnx/constraints/VerticalConstraint.h"
#include "mnx/solver/SketchSolver.h"

#include <memory>
#include <vector>

using namespace mnx;

namespace {

std::vector<const Constraint*> rawConstraints(const std::vector<std::unique_ptr<Constraint>>& constraints) {
    std::vector<const Constraint*> result;
    result.reserve(constraints.size());
    for(const auto& constraint : constraints) {
        result.push_back(constraint.get());
    }
    return result;
}

double lineLength(const SketchLine& line) {
    return (line.end() - line.start()).norm();
}

} // namespace

TEST(SketchSolverTests, SolvesConstrainedRectangleDimensions) {
    Sketch sketch;
    SketchLine& bottom = sketch.addLine(Point2(0.2, 0.1), Point2(4.8, -0.2));
    SketchLine& right = sketch.addLine(Point2(4.6, 0.2), Point2(5.1, 3.5));
    SketchLine& top = sketch.addLine(Point2(4.7, 3.2), Point2(-0.3, 2.9));
    SketchLine& left = sketch.addLine(Point2(-0.1, 3.1), Point2(0.3, -0.4));

    std::vector<std::unique_ptr<Constraint>> constraints;
    constraints.push_back(std::make_unique<CoincidentConstraint>(
        ConstraintId(1), GeometryRef::endPoint(bottom.id()), GeometryRef::startPoint(right.id())));
    constraints.push_back(std::make_unique<CoincidentConstraint>(
        ConstraintId(2), GeometryRef::endPoint(right.id()), GeometryRef::startPoint(top.id())));
    constraints.push_back(std::make_unique<CoincidentConstraint>(
        ConstraintId(3), GeometryRef::endPoint(top.id()), GeometryRef::startPoint(left.id())));
    constraints.push_back(std::make_unique<CoincidentConstraint>(
        ConstraintId(4), GeometryRef::endPoint(left.id()), GeometryRef::startPoint(bottom.id())));
    constraints.push_back(std::make_unique<HorizontalConstraint>(ConstraintId(5), GeometryRef::whole(bottom.id())));
    constraints.push_back(std::make_unique<HorizontalConstraint>(ConstraintId(6), GeometryRef::whole(top.id())));
    constraints.push_back(std::make_unique<VerticalConstraint>(ConstraintId(7), GeometryRef::whole(left.id())));
    constraints.push_back(std::make_unique<VerticalConstraint>(ConstraintId(8), GeometryRef::whole(right.id())));
    constraints.push_back(std::make_unique<DistanceConstraint>(
        ConstraintId(9), GeometryRef::startPoint(bottom.id()), GeometryRef::endPoint(bottom.id()), 4.0));
    constraints.push_back(std::make_unique<DistanceConstraint>(
        ConstraintId(10), GeometryRef::startPoint(left.id()), GeometryRef::endPoint(left.id()), 3.0));

    auto raw = rawConstraints(constraints);
    const SolveResult result = SketchSolver().solve(sketch, raw);

    EXPECT_TRUE(result.converged());
    EXPECT_NEAR(lineLength(bottom), 4.0, 1.0e-5);
    EXPECT_NEAR(lineLength(left), 3.0, 1.0e-5);
    EXPECT_NEAR(bottom.start().y(), bottom.end().y(), 1.0e-5);
    EXPECT_NEAR(left.start().x(), left.end().x(), 1.0e-5);
}

TEST(SketchSolverTests, SolvesTrianglePointDistances) {
    Sketch sketch;
    SketchPoint& a = sketch.addPoint(Point2(0.0, 0.0));
    SketchPoint& b = sketch.addPoint(Point2(3.2, 0.2));
    SketchPoint& c = sketch.addPoint(Point2(0.2, 4.2));

    std::vector<std::unique_ptr<Constraint>> constraints;
    constraints.push_back(std::make_unique<DistanceConstraint>(
        ConstraintId(1), GeometryRef::centerPoint(a.id()), GeometryRef::centerPoint(b.id()), 3.0));
    constraints.push_back(std::make_unique<DistanceConstraint>(
        ConstraintId(2), GeometryRef::centerPoint(a.id()), GeometryRef::centerPoint(c.id()), 4.0));
    constraints.push_back(std::make_unique<DistanceConstraint>(
        ConstraintId(3), GeometryRef::centerPoint(b.id()), GeometryRef::centerPoint(c.id()), 5.0));

    auto raw = rawConstraints(constraints);
    const SolveResult result = SketchSolver().solve(sketch, raw);

    EXPECT_TRUE(result.converged());
    EXPECT_TRUE(result.diagnostics().hasWarning(SolverWarning::UnderConstrained));
    EXPECT_NEAR((a.point() - b.point()).norm(), 3.0, 1.0e-5);
    EXPECT_NEAR((a.point() - c.point()).norm(), 4.0, 1.0e-5);
    EXPECT_NEAR((b.point() - c.point()).norm(), 5.0, 1.0e-5);
}

TEST(SketchSolverTests, SolvesTangentLineCircleCase) {
    Sketch sketch;
    SketchLine& line = sketch.addLine(Point2(-2.0, 0.4), Point2(2.0, 0.4));
    SketchCircle& circle = sketch.addCircle(Point2(0.0, 0.0), 0.8);

    std::vector<std::unique_ptr<Constraint>> constraints;
    constraints.push_back(std::make_unique<HorizontalConstraint>(ConstraintId(1), GeometryRef::whole(line.id())));
    constraints.push_back(std::make_unique<RadiusConstraint>(ConstraintId(2), GeometryRef::whole(circle.id()), 1.0));
    constraints.push_back(std::make_unique<TangentConstraint>(
        ConstraintId(3), GeometryRef::whole(line.id()), GeometryRef::whole(circle.id())));

    auto raw = rawConstraints(constraints);
    const SolveResult result = SketchSolver().solve(sketch, raw);

    EXPECT_TRUE(result.converged());
    EXPECT_NEAR(circle.radius(), 1.0, 1.0e-5);
    EXPECT_NEAR(line.start().y() - circle.center().y(), 1.0, 1.0e-5);
    EXPECT_NEAR(line.start().y(), line.end().y(), 1.0e-5);
}

TEST(SketchSolverTests, ReportsInconsistentConstraints) {
    Sketch sketch;
    SketchPoint& a = sketch.addPoint(Point2(0.0, 0.0));
    SketchPoint& b = sketch.addPoint(Point2(1.0, 0.0));

    std::vector<std::unique_ptr<Constraint>> constraints;
    constraints.push_back(std::make_unique<DistanceConstraint>(
        ConstraintId(1), GeometryRef::centerPoint(a.id()), GeometryRef::centerPoint(b.id()), 1.0));
    constraints.push_back(std::make_unique<DistanceConstraint>(
        ConstraintId(2), GeometryRef::centerPoint(a.id()), GeometryRef::centerPoint(b.id()), 2.0));

    auto raw = rawConstraints(constraints);
    const SolveResult result = SketchSolver().solve(sketch, raw);

    EXPECT_FALSE(result.converged());
    EXPECT_TRUE(result.diagnostics().hasWarning(SolverWarning::NotConverged));
    EXPECT_TRUE(result.finalResidual() > 0.1);
}

TEST(SketchSolverTests, ReportsUnderConstrainedScenario) {
    Sketch sketch;
    SketchLine& line = sketch.addLine(Point2(0.0, 1.0), Point2(2.0, 2.0));
    std::vector<std::unique_ptr<Constraint>> constraints;
    constraints.push_back(std::make_unique<HorizontalConstraint>(ConstraintId(1), GeometryRef::whole(line.id())));

    auto raw = rawConstraints(constraints);
    const SolveResult result = SketchSolver().solve(sketch, raw);

    EXPECT_TRUE(result.converged());
    EXPECT_TRUE(result.diagnostics().hasWarning(SolverWarning::UnderConstrained));
    EXPECT_NEAR(line.start().y(), line.end().y(), 1.0e-5);
}

TEST(SketchSolverTests, SolvesParallelAndPerpendicularLines) {
    Sketch sketch;
    SketchLine& base = sketch.addLine(Point2(0.0, 0.0), Point2(3.0, 0.2));
    SketchLine& parallel = sketch.addLine(Point2(0.0, 1.0), Point2(2.0, 1.4));
    SketchLine& perpendicular = sketch.addLine(Point2(1.0, 0.0), Point2(1.5, 3.0));

    std::vector<std::unique_ptr<Constraint>> constraints;
    constraints.push_back(std::make_unique<HorizontalConstraint>(ConstraintId(1), GeometryRef::whole(base.id())));
    constraints.push_back(std::make_unique<ParallelConstraint>(
        ConstraintId(2), GeometryRef::whole(base.id()), GeometryRef::whole(parallel.id())));
    constraints.push_back(std::make_unique<PerpendicularConstraint>(
        ConstraintId(3), GeometryRef::whole(base.id()), GeometryRef::whole(perpendicular.id())));

    auto raw = rawConstraints(constraints);
    const SolveResult result = SketchSolver().solve(sketch, raw);

    EXPECT_TRUE(result.converged());
    EXPECT_NEAR((base.end() - base.start()).cross(parallel.end() - parallel.start()), 0.0, 1.0e-4);
    EXPECT_NEAR((base.end() - base.start()).dot(perpendicular.end() - perpendicular.start()), 0.0, 1.0e-4);
}
