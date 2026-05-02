#pragma once

#include <string>
#include <vector>

namespace mnx {

enum class SolverWarning {
    UnderConstrained,
    NotConverged,
    SingularSystem,
    InvalidConstraint,
};

class SolverDiagnostics {
public:
    void addWarning(SolverWarning warning, std::string message);

    [[nodiscard]] const std::vector<SolverWarning>& warnings() const {
        return warnings_;
    }

    [[nodiscard]] const std::vector<std::string>& messages() const {
        return messages_;
    }

    [[nodiscard]] bool hasWarnings() const {
        return !warnings_.empty();
    }

    [[nodiscard]] bool hasWarning(SolverWarning warning) const;

private:
    std::vector<SolverWarning> warnings_;
    std::vector<std::string> messages_;
};

} // namespace mnx
