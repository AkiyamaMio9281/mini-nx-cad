#include "mnx/solver/SketchSolver.h"

#include <algorithm>
#include <cmath>
#include <numeric>

namespace {

double stepNorm(std::span<const double> step) {
    double sum = 0.0;
    for(const double value : step) {
        sum += value * value;
    }
    return std::sqrt(sum);
}

} // namespace

namespace mnx {

SketchSolver::SketchSolver(SketchSolverOptions options) : options_(options) {}

SolveResult SketchSolver::solve(Sketch& sketch, std::span<const Constraint*> constraints) const {
    SolverDiagnostics diagnostics;
    VariableRegistry registry;
    registry.extractFromSketch(sketch);

    std::vector<ConstraintEquation> equations = equationBuilder_.build(sketch, registry, constraints, diagnostics);
    std::vector<double> values = registry.initialValues();

    if(equations.empty()) {
        diagnostics.addWarning(SolverWarning::UnderConstrained, "No equations were generated");
        return SolveResult(true, 0, 0.0, diagnostics);
    }
    if(equations.size() < values.size()) {
        diagnostics.addWarning(SolverWarning::UnderConstrained, "There are fewer equations than variables");
    }

    std::vector<double> residuals = evaluateResiduals(equations, values);
    double finalResidual = residualNorm(residuals);
    bool converged = finalResidual <= options_.residualTolerance;
    int iterations = 0;

    for(; iterations < options_.maxIterations && !converged; ++iterations) {
        const Matrix jacobian = jacobianBuilder_.build(equations, values);
        const std::vector<double> delta =
            solveNormalEquations(jacobian, residuals, options_.damping, diagnostics);

        if(delta.empty()) {
            break;
        }

        std::vector<double> candidate = values;
        for(std::size_t index = 0; index < candidate.size(); ++index) {
            candidate[index] += delta[index];
        }

        std::vector<double> candidateResiduals = evaluateResiduals(equations, candidate);
        double candidateNorm = residualNorm(candidateResiduals);

        double scale = 0.5;
        while(candidateNorm > finalResidual && scale > 1.0e-4) {
            candidate = values;
            for(std::size_t index = 0; index < candidate.size(); ++index) {
                candidate[index] += delta[index] * scale;
            }
            candidateResiduals = evaluateResiduals(equations, candidate);
            candidateNorm = residualNorm(candidateResiduals);
            scale *= 0.5;
        }

        values = std::move(candidate);
        residuals = std::move(candidateResiduals);
        finalResidual = candidateNorm;

        if(finalResidual <= options_.residualTolerance || stepNorm(delta) <= options_.stepTolerance) {
            converged = finalResidual <= options_.residualTolerance;
            break;
        }
    }

    registry.writeBackToSketch(sketch, values);

    if(!converged) {
        diagnostics.addWarning(SolverWarning::NotConverged, "Solver did not reach residual tolerance");
    }

    return SolveResult(converged, iterations, finalResidual, diagnostics);
}

std::vector<double> SketchSolver::solveNormalEquations(
    const Matrix& jacobian,
    std::span<const double> residuals,
    double damping,
    SolverDiagnostics& diagnostics
) const {
    if(jacobian.empty() || jacobian.front().empty()) {
        return {};
    }

    const std::size_t rowCount = jacobian.size();
    const std::size_t columnCount = jacobian.front().size();
    Matrix augmented(columnCount, std::vector<double>(columnCount + 1, 0.0));

    for(std::size_t row = 0; row < columnCount; ++row) {
        for(std::size_t column = 0; column < columnCount; ++column) {
            double value = row == column ? damping : 0.0;
            for(std::size_t equation = 0; equation < rowCount; ++equation) {
                value += jacobian[equation][row] * jacobian[equation][column];
            }
            augmented[row][column] = value;
        }

        double rhs = 0.0;
        for(std::size_t equation = 0; equation < rowCount; ++equation) {
            rhs -= jacobian[equation][row] * residuals[equation];
        }
        augmented[row][columnCount] = rhs;
    }

    for(std::size_t pivotColumn = 0; pivotColumn < columnCount; ++pivotColumn) {
        std::size_t pivotRow = pivotColumn;
        for(std::size_t row = pivotColumn + 1; row < columnCount; ++row) {
            if(std::abs(augmented[row][pivotColumn]) > std::abs(augmented[pivotRow][pivotColumn])) {
                pivotRow = row;
            }
        }

        if(std::abs(augmented[pivotRow][pivotColumn]) < 1.0e-14) {
            diagnostics.addWarning(SolverWarning::SingularSystem, "Normal equation matrix was singular");
            return {};
        }

        if(pivotRow != pivotColumn) {
            std::swap(augmented[pivotRow], augmented[pivotColumn]);
        }

        const double pivot = augmented[pivotColumn][pivotColumn];
        for(std::size_t column = pivotColumn; column <= columnCount; ++column) {
            augmented[pivotColumn][column] /= pivot;
        }

        for(std::size_t row = 0; row < columnCount; ++row) {
            if(row == pivotColumn) {
                continue;
            }
            const double factor = augmented[row][pivotColumn];
            for(std::size_t column = pivotColumn; column <= columnCount; ++column) {
                augmented[row][column] -= factor * augmented[pivotColumn][column];
            }
        }
    }

    std::vector<double> solution(columnCount, 0.0);
    for(std::size_t row = 0; row < columnCount; ++row) {
        solution[row] = augmented[row][columnCount];
    }
    return solution;
}

double SketchSolver::residualNorm(std::span<const double> residuals) {
    if(residuals.empty()) {
        return 0.0;
    }

    double sum = 0.0;
    for(const double residual : residuals) {
        sum += residual * residual;
    }
    return std::sqrt(sum / static_cast<double>(residuals.size()));
}

} // namespace mnx
