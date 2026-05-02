#pragma once

#include "mnx/core/math/AABB3.h"
#include "mnx/topology/CoEdge.h"
#include "mnx/topology/Edge.h"
#include "mnx/topology/Face.h"
#include "mnx/topology/Loop.h"
#include "mnx/topology/Shell.h"
#include "mnx/topology/Vertex.h"

#include <span>
#include <vector>

namespace mnx::topology {

class Body {
public:
    [[nodiscard]] std::size_t addVertex(const Vertex& vertex);
    [[nodiscard]] std::size_t addEdge(const Edge& edge);
    [[nodiscard]] std::size_t addCoEdge(const CoEdge& coEdge);
    [[nodiscard]] std::size_t addLoop(const Loop& loop);
    [[nodiscard]] std::size_t addFace(const Face& face);
    [[nodiscard]] std::size_t addShell(const Shell& shell);

    [[nodiscard]] std::span<const Vertex> vertices() const;
    [[nodiscard]] std::span<const Edge> edges() const;
    [[nodiscard]] std::span<const CoEdge> coEdges() const;
    [[nodiscard]] std::span<const Loop> loops() const;
    [[nodiscard]] std::span<const Face> faces() const;
    [[nodiscard]] std::span<const Shell> shells() const;

    [[nodiscard]] const Vertex* vertex(std::size_t index) const;
    [[nodiscard]] const Edge* edge(std::size_t index) const;
    [[nodiscard]] const CoEdge* coEdge(std::size_t index) const;
    [[nodiscard]] const Loop* loop(std::size_t index) const;
    [[nodiscard]] const Face* face(std::size_t index) const;
    [[nodiscard]] const Shell* shell(std::size_t index) const;

    [[nodiscard]] AABB3 bounds() const;

private:
    std::vector<Vertex> vertices_;
    std::vector<Edge> edges_;
    std::vector<CoEdge> coEdges_;
    std::vector<Loop> loops_;
    std::vector<Face> faces_;
    std::vector<Shell> shells_;
};

} // namespace mnx::topology
