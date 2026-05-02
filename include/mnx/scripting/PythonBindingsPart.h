#pragma once

#include "mnx/core/ids/ConstraintId.h"
#include "mnx/core/ids/EntityId.h"
#include "mnx/core/utils/Result.h"
#include "mnx/constraints/Constraint.h"
#include "mnx/modeling/PartDocument.h"
#include "mnx/sketch/Sketch.h"
#include "mnx/solver/SolveResult.h"

#include <filesystem>
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace mnx::scripting {

struct ScriptSolveSummary {
    bool converged = false;
    int iterationCount = 0;
    double finalResidual = 0.0;
    std::vector<std::string> diagnostics;
};

class ScriptPart {
public:
    explicit ScriptPart(std::string name = "ScriptPart");

    [[nodiscard]] const std::string& name() const;
    void newSketch();
    [[nodiscard]] EntityId addLine(double x1, double y1, double x2, double y2);
    [[nodiscard]] EntityId addCircle(double cx, double cy, double radius);
    [[nodiscard]] ConstraintId addDistanceConstraint(EntityId first, EntityId second, double distance);
    [[nodiscard]] ScriptSolveSummary solveSketch();
    [[nodiscard]] Status extrude(double distance);
    [[nodiscard]] Status revolve(
        double axisOriginX,
        double axisOriginY,
        double axisDirectionX,
        double axisDirectionY,
        double angleRadians
    );
    [[nodiscard]] Status saveProject(const std::filesystem::path& path);
    [[nodiscard]] Status exportStl(const std::filesystem::path& path);
    [[nodiscard]] const PartDocument& document();

private:
    struct ExtrudeRequest {
        double distance = 0.0;
    };

    struct RevolveRequest {
        double axisOriginX = 0.0;
        double axisOriginY = 0.0;
        double axisDirectionX = 0.0;
        double axisDirectionY = 1.0;
        double angleRadians = 0.0;
    };

    [[nodiscard]] Status rebuildDocument();

    std::string name_;
    Sketch sketch_;
    std::vector<std::unique_ptr<Constraint>> constraints_;
    PartDocument document_;
    EntityId::ValueType nextConstraintId_ = 1;
    std::optional<ExtrudeRequest> extrudeRequest_;
    std::optional<RevolveRequest> revolveRequest_;
};

[[nodiscard]] ScriptPart createPart(const std::string& name);

} // namespace mnx::scripting

#if defined(MNX_HAS_PYBIND11)
namespace pybind11 {
class module_;
}

namespace mnx::scripting {
void bindPart(pybind11::module_& module);
}
#endif
