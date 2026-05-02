# Mini NX

Mini NX is a modular C++20 desktop CAD prototype built to demonstrate the architecture and workflows behind engineering software: 2D sketching, constraints, feature history, basic 3D modeling, simplified topology, rendering, persistence, assembly structure, undo/redo, and Python automation.

The goal is not to clone a commercial CAD kernel. The goal is a serious, readable, testable prototype that shows how a CAD application can be divided into clean subsystems and grown phase by phase without turning the UI into the model.

## Current Capabilities

- CMake-based C++20 project with per-module library targets.
- Core math, typed IDs, tolerances, `Status`, and `Result`.
- 2D geometry primitives and algorithms.
- Sketch data model with stable entity IDs and geometry references.
- Basic Qt sketch interaction shell with select, line, circle, and rectangle tools.
- Declarative sketch constraints and a numerical sketch solver.
- Command stack for undo/redo of sketch edits and constraints.
- Feature history graph with dirty propagation and rebuild orchestration.
- Working extrude and revolve features that generate display meshes and simplified bodies.
- Simplified B-Rep/topology layer with validation and traversal queries.
- Qt/OpenGL-oriented rendering layer with camera controls and CPU picking helpers.
- JSON project persistence for part state, plus assembly persistence hooks.
- Assembly model with component instances, hierarchy, transforms, and first-pass mates.
- Thin Python automation facade and optional pybind11 module target.
- Unit and smoke tests across core, geometry, sketch, constraints, solver, commands, modeling, topology, rendering, persistence, assembly, and scripting.

## Repository Structure

```text
apps/cad_desktop/     Qt desktop shell and UI panels
cmake/                build helpers for warnings, sanitizers, static analysis, dependencies
docs/                 architecture, algorithm, design, and test documentation
include/mnx/          public headers by module
samples/              sample sketch, part, and assembly project files
scripts/              Python generator, regression, and benchmark scripts
src/                  module implementations
tests/                unit, integration, and regression metadata
```

## Architecture Snapshot

Dependency direction is intentionally one-way:

```text
core
  -> geometry
    -> sketch
      -> constraints
        -> solver
commands -> sketch / constraints / solver
topology -> core / geometry
modeling -> core / geometry / sketch / constraints / solver / topology
rendering -> core / geometry / topology / modeling
persistence -> domain modules
assembly -> core / geometry / modeling / topology
scripting -> domain modules
apps -> UI and application coordination
```

Qt stays at the app, sketch GUI, and rendering widget boundary. Domain modules do not depend on widgets. Display meshes are separate from topology, and persistence stores rebuildable model state rather than GPU state.

See [docs/architecture/overview.md](docs/architecture/overview.md) for the longer version.

## Build

Requirements:

- C++20 compiler
- CMake 3.24+
- Qt 6 Widgets/OpenGLWidgets for the desktop app
- GoogleTest optional; tests have a lightweight fallback harness
- pybind11 optional; the `mini_nx` Python extension is built only when found

Typical configure/build:

```bash
cmake -S . -B build -DMNX_BUILD_TESTS=ON -DMNX_BUILD_DESKTOP=ON
cmake --build build --config Debug
```

Useful options:

```bash
-DMNX_ENABLE_ASAN=ON
-DMNX_ENABLE_UBSAN=ON
-DMNX_ENABLE_WARNINGS=ON
-DMNX_ENABLE_CLANG_TIDY=ON
```

In this development environment, CMake was not available on PATH, so verification has also been done with direct `g++` compile commands for non-Qt modules.

## Test

With CMake:

```bash
ctest --test-dir build --output-on-failure
```

The current suite covers:

- Core math, IDs, status/result
- Geometry primitives and algorithms
- Sketch data model and geometry references
- Constraint validation
- Sketch solver success and failure cases
- Command undo/redo
- Feature graph, extrude, revolve
- Topology validation and traversal
- Rendering camera/mesh/picking helpers
- Persistence round-trips and invalid input
- Assembly insertion, mates, and persistence
- Scripting facade smoke tests
- Sample asset load/rebuild checks

Regression case metadata lives under `tests/integration/regression_cases/`.

## Python Automation

When pybind11 is available, the project builds a `mini_nx` module exposing a small workflow API:

```python
import mini_nx

part = mini_nx.create_part("demo_box")
part.new_sketch()
part.add_line(0, 0, 4, 0)
part.add_line(4, 0, 4, 2)
part.add_line(4, 2, 0, 2)
part.add_line(0, 2, 0, 0)
part.extrude(1.0)
part.save_project("demo_box.mnx.json")
part.export_stl("demo_box.stl")
```

Sample scripts:

- `scripts/generators/generate_bracket.py`
- `scripts/regression/basic_extrude_regression.py`
- `scripts/benchmarks/extrude_benchmark.py`

If the extension is not built, the scripts exit cleanly and print the missing dependency.

## Samples

Sample project files are checked in:

- `samples/sketches/basic_rectangle_sketch.mnx.json`
- `samples/parts/rectangle_extrude.mnx.json`
- `samples/parts/revolve_pin.mnx.json`
- `samples/assemblies/two_box_assembly.mnx_assembly.json`

Screenshot placeholders and generation notes are in `docs/user-guide/demo_notes.md`.

## Honest Limitations

- The sketch solver is a numerical prototype, not an industrial constraint solver.
- Extrude/revolve support simple closed profiles; concave and advanced profile cases are intentionally limited.
- Topology is simplified and often built from display mesh triangles.
- Assembly mates are deterministic and basic, not a full DOF solver.
- Persistence uses a small local JSON implementation until the preferred dependency is available.
- Python bindings expose workflow-level automation, not the full internal object graph.

## Why This Project Matters

Mini NX is structured to show the engineering muscles used in CAD, graphics, and simulation software: explicit ownership, dependency boundaries, typed geometry, deterministic tests, rebuild pipelines, topology queries, UI/domain separation, and a practical automation surface.
