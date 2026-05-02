#include "mnx/modeling/MeshBuilder.h"

#include "mnx/core/math/Tolerance.h"

#include <cmath>
#include <numbers>

namespace mnx {

DisplayMesh MeshBuilder::buildExtrudeMesh(const Profile& profile, double distance) const {
    return buildPolylineExtrudeMesh(profile.tessellatedBoundary(), distance);
}

DisplayMesh MeshBuilder::buildRevolveMesh(
    const Profile& profile,
    const RevolveAxis2D& axis,
    double angleRadians,
    int angularSegments
) const {
    DisplayMesh mesh;
    const std::vector<Point2> boundary = profile.tessellatedBoundary();
    if(boundary.size() < 3 || !axis.isValid() || angularSegments < 3) {
        return mesh;
    }

    const bool fullTurn = std::abs(angleRadians - 2.0 * std::numbers::pi) <= kDefaultTolerance;
    const std::size_t profileCount = boundary.size();
    const std::size_t ringCount = static_cast<std::size_t>(angularSegments) + 1;

    for(std::size_t ring = 0; ring < ringCount; ++ring) {
        const double t = static_cast<double>(ring) / static_cast<double>(angularSegments);
        const double angle = angleRadians * t;
        for(const Point2& point : boundary) {
            (void)mesh.addVertex(mapRevolvePoint(point, axis, angle));
        }
    }

    for(std::size_t ring = 0; ring + 1 < ringCount; ++ring) {
        const std::size_t currentOffset = ring * profileCount;
        const std::size_t nextOffset = (ring + 1) * profileCount;
        for(std::size_t index = 0; index < profileCount; ++index) {
            const std::size_t next = (index + 1) % profileCount;
            mesh.addTriangle(currentOffset + index, currentOffset + next, nextOffset + index);
            mesh.addTriangle(currentOffset + next, nextOffset + next, nextOffset + index);
        }
    }

    if(!fullTurn) {
        const std::size_t lastOffset = (ringCount - 1) * profileCount;
        for(std::size_t index = 1; index + 1 < profileCount; ++index) {
            mesh.addTriangle(0, index + 1, index);
            mesh.addTriangle(lastOffset, lastOffset + index, lastOffset + index + 1);
        }
    }

    return mesh;
}

DisplayMesh MeshBuilder::buildPolylineExtrudeMesh(const std::vector<Point2>& boundary, double distance) const {
    DisplayMesh mesh;
    if(boundary.size() < 3) {
        return mesh;
    }

    const bool clockwise = ProfileUtils::signedArea(boundary) < 0.0;
    const std::size_t count = boundary.size();
    for(const Point2& point : boundary) {
        (void)mesh.addVertex(Point3(point.x(), point.y(), 0.0));
    }
    for(const Point2& point : boundary) {
        (void)mesh.addVertex(Point3(point.x(), point.y(), distance));
    }

    for(std::size_t index = 1; index + 1 < count; ++index) {
        if(clockwise) {
            mesh.addTriangle(0, index + 1, index);
            mesh.addTriangle(count, count + index, count + index + 1);
        } else {
            mesh.addTriangle(0, index, index + 1);
            mesh.addTriangle(count, count + index + 1, count + index);
        }
    }

    for(std::size_t index = 0; index < count; ++index) {
        const std::size_t next = (index + 1) % count;
        const std::size_t bottomA = index;
        const std::size_t bottomB = next;
        const std::size_t topA = count + index;
        const std::size_t topB = count + next;

        if(clockwise) {
            mesh.addTriangle(bottomA, topA, bottomB);
            mesh.addTriangle(bottomB, topA, topB);
        } else {
            mesh.addTriangle(bottomA, bottomB, topA);
            mesh.addTriangle(bottomB, topB, topA);
        }
    }

    return mesh;
}

Point3 MeshBuilder::mapRevolvePoint(const Point2& point, const RevolveAxis2D& axis, double angleRadians) {
    const double axial = axis.axialCoordinate(point);
    const double radius = std::abs(axis.signedDistance(point));
    return Point3(axial, radius * std::cos(angleRadians), radius * std::sin(angleRadians));
}

} // namespace mnx
