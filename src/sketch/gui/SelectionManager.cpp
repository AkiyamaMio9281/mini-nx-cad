#include "mnx/sketch/gui/SelectionManager.h"

#include "mnx/geometry/algorithms/Distances2D.h"
#include "mnx/sketch/SketchArc.h"
#include "mnx/sketch/SketchCircle.h"
#include "mnx/sketch/SketchLine.h"
#include "mnx/sketch/SketchPoint.h"
#include "mnx/sketch/SketchSpline.h"

#include <algorithm>
#include <cmath>
#include <limits>

namespace {

double entityDistance(const mnx::SketchEntity& entity, const mnx::Point2& position) {
    using namespace mnx;

    if(const auto* point = dynamic_cast<const SketchPoint*>(&entity)) {
        return distance(position, point->resolvePoint(GeometryRefKind::CenterPoint).value());
    }
    if(const auto* line = dynamic_cast<const SketchLine*>(&entity)) {
        const Point2 start = line->resolvePoint(GeometryRefKind::StartPoint).value();
        const Point2 end = line->resolvePoint(GeometryRefKind::EndPoint).value();
        return distance(position, Segment2D(start, end));
    }
    if(const auto* circle = dynamic_cast<const SketchCircle*>(&entity)) {
        const Point2 center = circle->resolvePoint(GeometryRefKind::CenterPoint).value();
        return std::abs(distance(position, center) - circle->radius());
    }
    if(const auto* arc = dynamic_cast<const SketchArc*>(&entity)) {
        const Point2 center = arc->resolvePoint(GeometryRefKind::CenterPoint).value();
        return std::abs(distance(position, center) - arc->radius());
    }
    if(const auto* spline = dynamic_cast<const SketchSpline*>(&entity)) {
        double best = std::numeric_limits<double>::max();
        auto points = spline->controlPoints();
        for(std::size_t index = 1; index < points.size(); ++index) {
            best = std::min(best, distance(position, Segment2D(points[index - 1], points[index])));
        }
        return best;
    }

    return std::numeric_limits<double>::max();
}

} // namespace

namespace mnx {

std::optional<HitTestResult> SelectionManager::hitTest(
    const Sketch& sketch,
    const Point2& position,
    double tolerance
) const {
    std::optional<HitTestResult> best;

    for(const SketchEntity* entity : sketch.entities()) {
        const double candidateDistance = entityDistance(*entity, position);
        if(candidateDistance <= tolerance && (!best || candidateDistance < best->distance)) {
            best = HitTestResult {GeometryRef::whole(entity->id()), candidateDistance};
        }
    }

    return best;
}

void SelectionManager::replaceSelection(const GeometryRef& ref) {
    selection_.clear();
    selection_.select(ref);
}

void SelectionManager::toggleSelection(const GeometryRef& ref) {
    if(selection_.contains(ref)) {
        selection_.deselect(ref);
        return;
    }
    selection_.select(ref);
}

void SelectionManager::clear() {
    selection_.clear();
}

} // namespace mnx
