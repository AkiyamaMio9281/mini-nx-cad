# Sketch Solver

The sketch solver is a practical numerical prototype. It is designed to make common 2D sketch cases work predictably, expose useful diagnostics, and remain independent of Qt and document UI state.

## Inputs

The solver consumes:

- A mutable `Sketch`
- A list of declarative `Constraint` objects
- Solver options: max iterations, residual tolerance, step tolerance, damping

Constraints reference sketch geometry through `GeometryRef`, which can point to whole entities or sub-entities such as start point, end point, center point, and midpoint.

## Variable Extraction

`VariableRegistry` maps editable sketch handles into numerical variables. Current variables are mostly point coordinates and scalar circle radii. This allows the equation builder to read and write sketch geometry without the constraint objects carrying solver state.

## Equations

`EquationBuilder` converts constraints into residual equations. Supported first-pass constraints include:

- Coincident
- Horizontal
- Vertical
- Point-point distance
- Radius
- Parallel
- Perpendicular
- Tangent
- Equal length
- Angle where supported by current equation forms

Each equation returns a scalar residual. A perfect solve drives residuals toward zero.

## Jacobian and Iteration

`JacobianBuilder` computes a numerical Jacobian. `SketchSolver` then runs a damped Gauss-Newton iteration:

1. Evaluate residuals.
2. Build numerical Jacobian.
3. Solve damped normal equations.
4. Apply the step to variables.
5. Stop on residual or step tolerance, or report non-convergence.

The implementation favors deterministic behavior and readable diagnostics over advanced nonlinear optimization.

## Diagnostics

`SolveResult` reports:

- `converged`
- `iterationCount`
- `finalResidual`
- `SolverDiagnostics`

Diagnostics can include under-constrained, not converged, singular system, and invalid constraint warnings.

## Known Limitations

- The solver is order- and scale-sensitive for harder nonlinear systems.
- It does not perform full symbolic DOF analysis.
- Conflicting constraint identification is basic.
- Large sketches and badly conditioned systems need a stronger sparse linear algebra approach.
- Constraint coverage is intentionally smaller than a production CAD solver.

## Tests

The solver test suite includes:

- Constrained rectangle dimensions
- Triangle distances
- Tangent line-circle case
- Inconsistent constraints
- Under-constrained scenario
- Parallel and perpendicular line constraints

Regression metadata for solver-related cases lives under `tests/integration/regression_cases/`.
