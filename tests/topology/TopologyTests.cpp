#include "TestSupport.h"

#include "mnx/modeling/ExtrudeFeature.h"
#include "mnx/modeling/FeatureGraph.h"
#include "mnx/modeling/RebuildEngine.h"
#include "mnx/modeling/SketchFeature.h"
#include "mnx/topology/TopologyQueries.h"
#include "mnx/topology/TopologyValidator.h"

#include <memory>
#include <vector>

using namespace mnx;

namespace {

[[nodiscard]] Sketch makeRectangleSketch() {
    Sketch sketch;
    (void)sketch.addLine(Point2(0.0, 0.0), Point2(2.0, 0.0));
    (void)sketch.addLine(Point2(2.0, 0.0), Point2(2.0, 1.0));
    (void)sketch.addLine(Point2(2.0, 1.0), Point2(0.0, 1.0));
    (void)sketch.addLine(Point2(0.0, 1.0), Point2(0.0, 0.0));
    return sketch;
}

[[nodiscard]] topology::Body makeBrokenLoopBody() {
    topology::Body body;
    const std::size_t v0 = body.addVertex(topology::Vertex(Point3(0.0, 0.0, 0.0)));
    const std::size_t v1 = body.addVertex(topology::Vertex(Point3(1.0, 0.0, 0.0)));
    const std::size_t edge = body.addEdge(topology::Edge(v0, v1));
    const std::size_t coedge = body.addCoEdge(topology::CoEdge(edge, false));
    const std::size_t loop = body.addLoop(topology::Loop({coedge}));
    const std::size_t face = body.addFace(topology::Face({loop}));
    (void)body.addShell(topology::Shell({face}));
    return body;
}

} // namespace

TEST(TopologyTests, SimpleExtrudedBodyHasValidTopology) {
    FeatureGraph graph;
    EXPECT_TRUE(graph.insertFeature(std::make_unique<SketchFeature>(FeatureId(1), "Rectangle", makeRectangleSketch())));
    EXPECT_TRUE(graph.insertFeature(std::make_unique<ExtrudeFeature>(FeatureId(2), "Extrude", FeatureId(1), 3.0)));

    const RebuildResult result = RebuildEngine().rebuildAll(graph);
    const auto* extrude = dynamic_cast<const ExtrudeFeature*>(graph.findFeature(FeatureId(2)));

    EXPECT_TRUE(result.isOk());
    EXPECT_TRUE(extrude != nullptr);
    EXPECT_TRUE(extrude->body().has_value());

    const topology::Body& topologyBody = extrude->body()->topology();
    EXPECT_EQ(topologyBody.vertices().size(), extrude->body()->displayMesh().vertexCount());
    EXPECT_EQ(topologyBody.faces().size(), extrude->body()->displayMesh().triangleCount());
    EXPECT_TRUE(topology::TopologyValidator().validate(topologyBody));
}

TEST(TopologyTests, FaceEdgeVertexTraversalWorks) {
    FeatureGraph graph;
    EXPECT_TRUE(graph.insertFeature(std::make_unique<SketchFeature>(FeatureId(1), "Rectangle", makeRectangleSketch())));
    EXPECT_TRUE(graph.insertFeature(std::make_unique<ExtrudeFeature>(FeatureId(2), "Extrude", FeatureId(1), 1.0)));
    EXPECT_TRUE(RebuildEngine().rebuildAll(graph).isOk());
    const auto* extrude = dynamic_cast<const ExtrudeFeature*>(graph.findFeature(FeatureId(2)));
    EXPECT_TRUE(extrude != nullptr);

    const topology::Body& topologyBody = extrude->body()->topology();
    const std::vector<std::size_t> faceEdges = topology::TopologyQueries::edgesOfFace(topologyBody, 0);
    EXPECT_EQ(faceEdges.size(), static_cast<std::size_t>(3));

    const std::vector<std::size_t> edgeVertices = topology::TopologyQueries::verticesOfEdge(topologyBody, faceEdges[0]);
    EXPECT_EQ(edgeVertices.size(), static_cast<std::size_t>(2));
    EXPECT_TRUE(topologyBody.vertex(edgeVertices[0]) != nullptr);
    EXPECT_TRUE(topologyBody.vertex(edgeVertices[1]) != nullptr);
}

TEST(TopologyTests, AdjacentFacesOfSharedEdgeAreQueryable) {
    FeatureGraph graph;
    EXPECT_TRUE(graph.insertFeature(std::make_unique<SketchFeature>(FeatureId(1), "Rectangle", makeRectangleSketch())));
    EXPECT_TRUE(graph.insertFeature(std::make_unique<ExtrudeFeature>(FeatureId(2), "Extrude", FeatureId(1), 1.0)));
    EXPECT_TRUE(RebuildEngine().rebuildAll(graph).isOk());
    const auto* extrude = dynamic_cast<const ExtrudeFeature*>(graph.findFeature(FeatureId(2)));
    EXPECT_TRUE(extrude != nullptr);

    const topology::Body& topologyBody = extrude->body()->topology();
    bool foundSharedEdge = false;
    for(std::size_t edgeIndex = 0; edgeIndex < topologyBody.edges().size(); ++edgeIndex) {
        const std::vector<std::size_t> adjacentFaces =
            topology::TopologyQueries::adjacentFacesOfEdge(topologyBody, edgeIndex);
        if(adjacentFaces.size() == 2) {
            foundSharedEdge = true;
            break;
        }
    }
    EXPECT_TRUE(foundSharedEdge);
}

TEST(TopologyTests, ValidatorCatchesBrokenLoop) {
    const topology::Body broken = makeBrokenLoopBody();
    const Status status = topology::TopologyValidator().validate(broken);

    EXPECT_FALSE(status.isOk());
    EXPECT_EQ(status.code(), StatusCode::FailedPrecondition);
}

TEST(TopologyTests, ValidatorCatchesUnreferencedFace) {
    topology::Body body;
    const std::size_t v0 = body.addVertex(topology::Vertex(Point3(0.0, 0.0, 0.0)));
    const std::size_t v1 = body.addVertex(topology::Vertex(Point3(1.0, 0.0, 0.0)));
    const std::size_t v2 = body.addVertex(topology::Vertex(Point3(0.0, 1.0, 0.0)));
    const std::size_t e0 = body.addEdge(topology::Edge(v0, v1));
    const std::size_t e1 = body.addEdge(topology::Edge(v1, v2));
    const std::size_t e2 = body.addEdge(topology::Edge(v2, v0));
    const std::size_t c0 = body.addCoEdge(topology::CoEdge(e0, false));
    const std::size_t c1 = body.addCoEdge(topology::CoEdge(e1, false));
    const std::size_t c2 = body.addCoEdge(topology::CoEdge(e2, false));
    const std::size_t loop = body.addLoop(topology::Loop({c0, c1, c2}));
    (void)body.addFace(topology::Face({loop}));
    (void)body.addShell(topology::Shell(std::vector<std::size_t> {}));

    const Status status = topology::TopologyValidator().validate(body);
    EXPECT_FALSE(status.isOk());
}
