#pragma once

#include "mnx/core/utils/Result.h"
#include "mnx/geometry/Point2.h"
#include "mnx/sketch/Sketch.h"

#include <vector>

namespace mnx {

enum class ProfileKind {
    Polyline,
    Circle,
};

class Profile {
public:
    [[nodiscard]] static Profile polyline(std::vector<Point2> vertices);
    [[nodiscard]] static Profile circle(const Point2& center, double radius, int segmentCount = 32);

    [[nodiscard]] ProfileKind kind() const;
    [[nodiscard]] const std::vector<Point2>& vertices() const;
    [[nodiscard]] const Point2& center() const;
    [[nodiscard]] double radius() const;
    [[nodiscard]] int segmentCount() const;
    [[nodiscard]] double area() const;
    [[nodiscard]] std::vector<Point2> tessellatedBoundary() const;

private:
    ProfileKind kind_ = ProfileKind::Polyline;
    std::vector<Point2> vertices_;
    Point2 center_;
    double radius_ = 0.0;
    int segmentCount_ = 32;
};

class ProfileUtils {
public:
    [[nodiscard]] static Result<Profile> extractClosedProfile(const Sketch& sketch);
    [[nodiscard]] static bool isClosedPolyline(const std::vector<Point2>& vertices);
    [[nodiscard]] static bool isConvexPolyline(const std::vector<Point2>& vertices);
    [[nodiscard]] static double signedArea(const std::vector<Point2>& vertices);
};

} // namespace mnx
