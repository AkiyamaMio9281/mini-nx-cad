#pragma once

#include "mnx/modeling/DisplayMesh.h"
#include "mnx/modeling/ProfileUtils.h"
#include "mnx/modeling/RevolveAxis2D.h"

namespace mnx {

class MeshBuilder {
public:
    [[nodiscard]] DisplayMesh buildExtrudeMesh(const Profile& profile, double distance) const;
    [[nodiscard]] DisplayMesh buildRevolveMesh(
        const Profile& profile,
        const RevolveAxis2D& axis,
        double angleRadians,
        int angularSegments = 32
    ) const;

private:
    [[nodiscard]] DisplayMesh buildPolylineExtrudeMesh(const std::vector<Point2>& boundary, double distance) const;
    [[nodiscard]] static Point3 mapRevolvePoint(const Point2& point, const RevolveAxis2D& axis, double angleRadians);
};

} // namespace mnx
