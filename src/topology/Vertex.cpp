#include "mnx/topology/Vertex.h"

namespace mnx::topology {

Vertex::Vertex(const Point3& point) : point_(point) {}

const Point3& Vertex::point() const {
    return point_;
}

} // namespace mnx::topology
