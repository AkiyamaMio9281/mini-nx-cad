#include "mnx/scripting/PythonBindingsPart.h"

#include "mnx/constraints/DistanceConstraint.h"
#include "mnx/modeling/ExtrudeFeature.h"
#include "mnx/modeling/RebuildEngine.h"
#include "mnx/modeling/RevolveFeature.h"
#include "mnx/modeling/SketchFeature.h"
#include "mnx/persistence/PersistenceUtils.h"
#include "mnx/solver/SketchSolver.h"

#include <fstream>
#include <sstream>
#include <utility>

#if defined(MNX_HAS_PYBIND11)
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#endif

namespace mnx::scripting {

namespace {

[[nodiscard]] ConstraintId nextConstraintId(EntityId::ValueType& next) {
    return ConstraintId(next++);
}

[[nodiscard]] std::unique_ptr<SketchEntity> cloneEntity(const SketchEntity& entity) {
    switch(entity.type()) {
    case SketchEntityType::Point: {
        const auto& point = static_cast<const SketchPoint&>(entity);
        return std::make_unique<SketchPoint>(point.id(), point.point());
    }
    case SketchEntityType::Line: {
        const auto& line = static_cast<const SketchLine&>(entity);
        return std::make_unique<SketchLine>(line.id(), line.start(), line.end());
    }
    case SketchEntityType::Circle: {
        const auto& circle = static_cast<const SketchCircle&>(entity);
        return std::make_unique<SketchCircle>(circle.id(), circle.center(), circle.radius());
    }
    case SketchEntityType::Arc: {
        const auto& arc = static_cast<const SketchArc&>(entity);
        return std::make_unique<SketchArc>(arc.id(), arc.center(), arc.radius(), arc.startAngle(), arc.endAngle());
    }
    case SketchEntityType::Spline: {
        const auto& spline = static_cast<const SketchSpline&>(entity);
        return std::make_unique<SketchSpline>(spline.id(), std::vector<Point2>(spline.controlPoints().begin(), spline.controlPoints().end()), spline.degree());
    }
    }
    return nullptr;
}

[[nodiscard]] Sketch cloneSketch(const Sketch& sketch) {
    Sketch result(sketch.plane());
    for(const SketchEntity* entity : sketch.entities()) {
        if(entity != nullptr) {
            (void)result.insertEntity(cloneEntity(*entity));
        }
    }
    for(ConstraintId id : sketch.constraintIds()) {
        result.addConstraintId(id);
    }
    return result;
}

[[nodiscard]] const Body* latestBody(const PartDocument& document) {
    const Body* body = nullptr;
    for(const Feature* feature : document.featureGraph().features()) {
        if(const auto* extrude = dynamic_cast<const ExtrudeFeature*>(feature);
           extrude != nullptr && extrude->body().has_value()) {
            body = &*extrude->body();
        } else if(const auto* revolve = dynamic_cast<const RevolveFeature*>(feature);
                  revolve != nullptr && revolve->body().has_value()) {
            body = &*revolve->body();
        }
    }
    return body;
}

[[maybe_unused]] [[nodiscard]] std::string statusToString(const Status& status) {
    return status.isOk() ? std::string() : status.message();
}

void writeStlFacet(std::ostream& stream, const Point3& a, const Point3& b, const Point3& c) {
    const Vec3 normal = ((b - a).cross(c - a)).normalized();
    stream << "  facet normal " << normal.x() << ' ' << normal.y() << ' ' << normal.z() << '\n';
    stream << "    outer loop\n";
    stream << "      vertex " << a.x() << ' ' << a.y() << ' ' << a.z() << '\n';
    stream << "      vertex " << b.x() << ' ' << b.y() << ' ' << b.z() << '\n';
    stream << "      vertex " << c.x() << ' ' << c.y() << ' ' << c.z() << '\n';
    stream << "    endloop\n";
    stream << "  endfacet\n";
}

} // namespace

ScriptPart::ScriptPart(std::string name) : name_(std::move(name)), document_(name_) {}

const std::string& ScriptPart::name() const {
    return name_;
}

void ScriptPart::newSketch() {
    sketch_ = Sketch();
    constraints_.clear();
    nextConstraintId_ = 1;
    extrudeRequest_.reset();
    revolveRequest_.reset();
    document_ = PartDocument(name_);
}

EntityId ScriptPart::addLine(double x1, double y1, double x2, double y2) {
    return sketch_.addLine(Point2(x1, y1), Point2(x2, y2)).id();
}

EntityId ScriptPart::addCircle(double cx, double cy, double radius) {
    return sketch_.addCircle(Point2(cx, cy), radius).id();
}

ConstraintId ScriptPart::addDistanceConstraint(EntityId first, EntityId second, double distance) {
    const ConstraintId id = nextConstraintId(nextConstraintId_);
    auto constraint = std::make_unique<DistanceConstraint>(
        id,
        GeometryRef::startPoint(first),
        GeometryRef::startPoint(second),
        distance
    );
    constraints_.push_back(std::move(constraint));
    sketch_.addConstraintId(id);
    return id;
}

ScriptSolveSummary ScriptPart::solveSketch() {
    std::vector<const Constraint*> constraints;
    constraints.reserve(constraints_.size());
    for(const auto& constraint : constraints_) {
        constraints.push_back(constraint.get());
    }

    const SolveResult result = SketchSolver().solve(sketch_, constraints);
    ScriptSolveSummary summary;
    summary.converged = result.converged();
    summary.iterationCount = result.iterationCount();
    summary.finalResidual = result.finalResidual();
    summary.diagnostics = result.diagnostics().messages();
    return summary;
}

Status ScriptPart::extrude(double distance) {
    extrudeRequest_ = ExtrudeRequest {distance};
    revolveRequest_.reset();
    return rebuildDocument();
}

Status ScriptPart::revolve(
    double axisOriginX,
    double axisOriginY,
    double axisDirectionX,
    double axisDirectionY,
    double angleRadians
) {
    revolveRequest_ = RevolveRequest {axisOriginX, axisOriginY, axisDirectionX, axisDirectionY, angleRadians};
    extrudeRequest_.reset();
    return rebuildDocument();
}

Status ScriptPart::saveProject(const std::filesystem::path& path) {
    Status rebuildStatus = rebuildDocument();
    if(!rebuildStatus) {
        return rebuildStatus;
    }
    return persistence::PersistenceUtils::saveProject(document_, path);
}

Status ScriptPart::exportStl(const std::filesystem::path& path) {
    Status rebuildStatus = rebuildDocument();
    if(!rebuildStatus) {
        return rebuildStatus;
    }
    const Body* body = latestBody(document_);
    if(body == nullptr) {
        return Status::failedPrecondition("No rebuilt body is available for STL export");
    }

    std::ofstream file(path);
    if(!file) {
        return Status::failedPrecondition("Could not open STL file for writing");
    }
    file << "solid " << name_ << '\n';
    const DisplayMesh& mesh = body->displayMesh();
    for(const MeshTriangle& triangle : mesh.triangles()) {
        const Point3& a = mesh.vertices()[triangle.vertexIndices[0]];
        const Point3& b = mesh.vertices()[triangle.vertexIndices[1]];
        const Point3& c = mesh.vertices()[triangle.vertexIndices[2]];
        writeStlFacet(file, a, b, c);
    }
    file << "endsolid " << name_ << '\n';
    return Status::ok();
}

const PartDocument& ScriptPart::document() {
    (void)rebuildDocument();
    return document_;
}

Status ScriptPart::rebuildDocument() {
    document_ = PartDocument(name_);
    for(const auto& constraint : constraints_) {
        if(constraint) {
            document_.addConstraint(constraint->clone());
        }
    }

    auto sketchFeature = std::make_unique<SketchFeature>(FeatureId(1), "Sketch 1", cloneSketch(sketch_));
    Result<FeatureId> sketchId = document_.featureGraph().insertFeature(std::move(sketchFeature));
    if(!sketchId) {
        return sketchId.status();
    }

    if(extrudeRequest_.has_value()) {
        auto extrudeFeature = std::make_unique<ExtrudeFeature>(
            FeatureId(2),
            "Extrude 1",
            sketchId.value(),
            extrudeRequest_->distance
        );
        Result<FeatureId> extrudeId = document_.featureGraph().insertFeature(std::move(extrudeFeature));
        if(!extrudeId) {
            return extrudeId.status();
        }
    } else if(revolveRequest_.has_value()) {
        const RevolveAxis2D axis(
            Point2(revolveRequest_->axisOriginX, revolveRequest_->axisOriginY),
            Vec2(revolveRequest_->axisDirectionX, revolveRequest_->axisDirectionY)
        );
        auto revolveFeature = std::make_unique<RevolveFeature>(
            FeatureId(2),
            "Revolve 1",
            sketchId.value(),
            axis,
            revolveRequest_->angleRadians
        );
        Result<FeatureId> revolveId = document_.featureGraph().insertFeature(std::move(revolveFeature));
        if(!revolveId) {
            return revolveId.status();
        }
    }

    const RebuildResult rebuild = RebuildEngine().rebuildAll(document_.featureGraph());
    return rebuild.status;
}

ScriptPart createPart(const std::string& name) {
    return ScriptPart(name);
}

#if defined(MNX_HAS_PYBIND11)
void bindPart(pybind11::module_& module) {
    namespace py = pybind11;

    py::class_<ScriptSolveSummary>(module, "SolveSummary")
        .def_readonly("converged", &ScriptSolveSummary::converged)
        .def_readonly("iteration_count", &ScriptSolveSummary::iterationCount)
        .def_readonly("final_residual", &ScriptSolveSummary::finalResidual)
        .def_readonly("diagnostics", &ScriptSolveSummary::diagnostics);

    py::class_<ScriptPart>(module, "Part")
        .def(py::init<std::string>(), py::arg("name") = "ScriptPart")
        .def_property_readonly("name", &ScriptPart::name)
        .def("new_sketch", &ScriptPart::newSketch)
        .def("add_line", [](ScriptPart& part, double x1, double y1, double x2, double y2) {
            return part.addLine(x1, y1, x2, y2).value();
        })
        .def("add_circle", [](ScriptPart& part, double cx, double cy, double radius) {
            return part.addCircle(cx, cy, radius).value();
        })
        .def("add_distance_constraint", [](ScriptPart& part, EntityId::ValueType first, EntityId::ValueType second, double distance) {
            return part.addDistanceConstraint(EntityId(first), EntityId(second), distance).value();
        })
        .def("solve_sketch", &ScriptPart::solveSketch)
        .def("extrude", [](ScriptPart& part, double distance) {
            Status status = part.extrude(distance);
            if(!status) {
                throw std::runtime_error(status.message());
            }
        })
        .def("revolve", [](ScriptPart& part, double ox, double oy, double dx, double dy, double angle) {
            Status status = part.revolve(ox, oy, dx, dy, angle);
            if(!status) {
                throw std::runtime_error(status.message());
            }
        })
        .def("save_project", [](ScriptPart& part, const std::string& path) {
            Status status = part.saveProject(path);
            if(!status) {
                throw std::runtime_error(status.message());
            }
        })
        .def("export_stl", [](ScriptPart& part, const std::string& path) {
            Status status = part.exportStl(path);
            if(!status) {
                throw std::runtime_error(status.message());
            }
        });

    module.def("create_part", &createPart, py::arg("name") = "ScriptPart");
    module.def("status_to_string", &statusToString);
}
#endif

} // namespace mnx::scripting
