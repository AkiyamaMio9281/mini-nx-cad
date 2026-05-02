#include "mnx/topology/Body.h"

namespace mnx::topology {

std::size_t Body::addVertex(const Vertex& vertex) {
    vertices_.push_back(vertex);
    return vertices_.size() - 1;
}

std::size_t Body::addEdge(const Edge& edge) {
    edges_.push_back(edge);
    return edges_.size() - 1;
}

std::size_t Body::addCoEdge(const CoEdge& coEdge) {
    coEdges_.push_back(coEdge);
    return coEdges_.size() - 1;
}

std::size_t Body::addLoop(const Loop& loop) {
    loops_.push_back(loop);
    return loops_.size() - 1;
}

std::size_t Body::addFace(const Face& face) {
    faces_.push_back(face);
    return faces_.size() - 1;
}

std::size_t Body::addShell(const Shell& shell) {
    shells_.push_back(shell);
    return shells_.size() - 1;
}

std::span<const Vertex> Body::vertices() const {
    return vertices_;
}

std::span<const Edge> Body::edges() const {
    return edges_;
}

std::span<const CoEdge> Body::coEdges() const {
    return coEdges_;
}

std::span<const Loop> Body::loops() const {
    return loops_;
}

std::span<const Face> Body::faces() const {
    return faces_;
}

std::span<const Shell> Body::shells() const {
    return shells_;
}

const Vertex* Body::vertex(std::size_t index) const {
    return index < vertices_.size() ? &vertices_[index] : nullptr;
}

const Edge* Body::edge(std::size_t index) const {
    return index < edges_.size() ? &edges_[index] : nullptr;
}

const CoEdge* Body::coEdge(std::size_t index) const {
    return index < coEdges_.size() ? &coEdges_[index] : nullptr;
}

const Loop* Body::loop(std::size_t index) const {
    return index < loops_.size() ? &loops_[index] : nullptr;
}

const Face* Body::face(std::size_t index) const {
    return index < faces_.size() ? &faces_[index] : nullptr;
}

const Shell* Body::shell(std::size_t index) const {
    return index < shells_.size() ? &shells_[index] : nullptr;
}

AABB3 Body::bounds() const {
    AABB3 box;
    for(const Vertex& item : vertices_) {
        box.expand(item.point().vector());
    }
    return box;
}

} // namespace mnx::topology
