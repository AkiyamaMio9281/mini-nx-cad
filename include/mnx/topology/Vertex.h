#pragma once

#include "mnx/geometry/Point3.h"

namespace mnx::topology {

class Vertex {
public:
    Vertex() = default;
    explicit Vertex(const Point3& point);

    [[nodiscard]] const Point3& point() const;

private:
    Point3 point_;
};

} // namespace mnx::topology
