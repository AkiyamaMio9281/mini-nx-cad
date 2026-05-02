#include "mnx/sketch/gui/SnapManager.h"

#include "mnx/geometry/algorithms/Distances2D.h"

#include <array>

namespace mnx {

std::optional<SnapResult> SnapManager::snap(
    const Sketch& sketch,
    const Point2& position,
    double tolerance
) const {
    std::optional<SnapResult> best;

    for(const SketchEntity* entity : sketch.entities()) {
        const std::array<std::pair<GeometryRefKind, SnapKind>, 4> candidates {{
            {GeometryRefKind::StartPoint, SnapKind::Endpoint},
            {GeometryRefKind::EndPoint, SnapKind::Endpoint},
            {GeometryRefKind::Midpoint, SnapKind::Midpoint},
            {GeometryRefKind::CenterPoint, SnapKind::Center},
        }};

        for(const auto& [refKind, snapKind] : candidates) {
            const GeometryRef ref(entity->id(), refKind);
            if(!sketch.supportsRef(ref)) {
                continue;
            }

            Result<Point2> resolved = sketch.resolvePoint(ref);
            if(!resolved.isOk()) {
                continue;
            }

            const double candidateDistance = distance(position, resolved.value());
            if(candidateDistance <= tolerance && (!best || candidateDistance < best->distance)) {
                best = SnapResult {resolved.value(), ref, snapKind, candidateDistance};
            }
        }
    }

    return best;
}

} // namespace mnx
