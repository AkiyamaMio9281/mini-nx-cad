# Test Plan

Mini NX is tested primarily through deterministic non-GUI tests. GUI behavior is kept thin enough that most logic can be validated through domain modules.

## Test Categories

| Area | Test Target | Focus |
| --- | --- | --- |
| Core | `mnx_core_tests` | IDs, vectors, matrices, transforms, AABBs, tolerance, status/result |
| Geometry | `mnx_geometry_tests` | Primitives, projections, distances, intersections, curve sampling |
| Sketch | `mnx_sketch_tests` | Entity lifecycle, bounds, geometry refs, sketch plane |
| Constraints | `mnx_constraint_tests` | Constraint metadata and reference validation |
| Solver | `mnx_solver_tests` | Success, failure, under-constrained, tangent, parallel/perpendicular cases |
| Commands | `mnx_command_tests` | Undo/redo for create/delete/move/constraints/dimensions |
| Modeling | `mnx_modeling_tests` | Feature graph, rebuild, extrude, revolve |
| Topology | `mnx_topology_tests` | Body consistency, traversal, validator failures |
| Rendering | `mnx_rendering_tests` | Camera math, mesh bounds, scene population, picking helpers |
| Persistence | `mnx_persistence_tests` | Round-trip and invalid input handling |
| Assembly | `mnx_assembly_tests` | Component instances, hierarchy, mates, persistence |
| Scripting | `mnx_scripting_tests` | Automation facade, project save, STL export |

## Running Tests

Preferred:

```bash
cmake -S . -B build -DMNX_BUILD_TESTS=ON
cmake --build build
ctest --test-dir build --output-on-failure
```

Fallback used in constrained environments:

```bash
g++ -std=c++20 -I include -I tests/core ... -o build/mnx_scripting_tests.exe
build/mnx_scripting_tests.exe
```

## Regression Corpus

Regression metadata lives under `tests/integration/regression_cases/`. Each case has:

- `case.json`: structured metadata, expected behavior, and related modules
- Optional input file references under `samples/`
- Optional notes about the bug or scenario being protected

The corpus is intentionally human-readable so interviewers and future contributors can understand why a case exists without reading test code first.

## Current Gaps

- GUI interaction tests are not automated.
- Python import tests require pybind11 and a built extension.
- Persistence package tests for full assemblies with embedded part documents are not complete.
- Performance tests are benchmark scripts, not CI-enforced thresholds.

## Quality Rules

- Prefer deterministic assertions over screenshot or timing assertions.
- Keep floating-point checks routed through tolerances.
- Tests should exercise public APIs where practical.
- Regression cases should be added when a bug is fixed or an unsupported edge case becomes deliberate behavior.
