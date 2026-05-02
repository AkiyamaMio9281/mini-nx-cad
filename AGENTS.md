# AGENTS.md — Mini NX Implementation Spec for Codex

> Audience: **Codex / coding agent**, not a human learner.  
> Role: You are implementing a large-scale C++20 desktop CAD prototype called **Mini NX**.  
> Primary objective: produce a **modular, testable, buildable, extensible** codebase that resembles the architecture and workflows of a commercial CAD application.  
> Non-goal: do **not** optimize for the shortest demo or the fewest files. Optimize for **clean architecture, clear module boundaries, correctness, testability, and future extensibility**.

---

# 0. Mission

Build a desktop CAD prototype with the following capabilities:

- 2D sketch editing
- geometric constraints
- dimension-driven parameter solving
- feature history tree
- 3D feature-based modeling:
  - extrude
  - revolve
- simplified B-Rep / topology representation
- assembly document with component instances and simple mates
- undo / redo
- save / load
- Python automation bindings
- unit / integration / regression tests
- CI-ready CMake project

This project is intended to align with a **C++ CAD / geometry / graphics / engineering software** internship role. The implementation must therefore emphasize:

- modern C++ engineering
- data structures
- object-oriented design
- geometry / graphics domain logic
- maintainability
- testing discipline
- modularity

---

# 1. High-Level Directives

## 1.1 Architectural priorities
Prefer these properties in order:

1. **Buildable**
2. **Testable**
3. **Modular**
4. **Readable**
5. **Extensible**
6. **Performant enough**
7. **Fancy**

Do not sacrifice architecture for quick hacks.

## 1.2 Implementation priorities
Implement in this order:

1. project skeleton / build / tests
2. core math / ids / utilities
3. geometry kernel
4. sketch data model
5. sketch GUI basics
6. constraint data model
7. constraint solver
8. command stack / undo-redo
9. feature history tree
10. extrude / revolve
11. topology layer
12. 3D viewport / rendering
13. persistence
14. assembly
15. Python bindings
16. polishing / docs / regression corpus

## 1.3 Coding priorities
- Prefer explicit types over magic.
- Prefer small, cohesive classes over massive god objects.
- Prefer composition over inheritance where practical.
- Use inheritance for true polymorphic object families only.
- Keep UI separated from model logic.
- Keep rendering separated from CAD document logic.
- Keep topology separated from display mesh caches.
- Keep constraints as declarative rules; do not hide solver state inside constraint objects.
- Keep floating-point tolerance centralized.

## 1.4 What not to do
Do **not**:
- dump all logic into the Qt main window
- mix rendering code with business/model code
- hardcode ad-hoc tolerances across the codebase
- create circular dependencies between modules
- skip tests for math/geometry/solver
- design everything around global singletons
- expose raw UI pointers deep into domain layers
- hide side effects in utility functions with surprising behavior
- chase industrial Parasolid-level fidelity; this is a prototype, not a full kernel

---

# 2. Required Tech Stack

- **Language**: C++20
- **GUI**: Qt 6
- **Linear algebra**: Eigen
- **Build**: CMake
- **Testing**: GoogleTest
- **Python bindings**: pybind11
- **JSON**: nlohmann/json
- **Rendering**: Qt + OpenGL
- **Formatting**: clang-format
- **Lint**: clang-tidy

If a dependency is difficult to integrate, prefer the simplest stable integration path. Do not replace the stack without strong justification.

---

# 3. Repository Layout (Required)

Use this exact or near-exact layout. If you deviate, keep the same module boundaries.

```text
mini-nx/
├─ CMakeLists.txt
├─ .clang-format
├─ .clang-tidy
├─ cmake/
│  ├─ Warnings.cmake
│  ├─ Sanitizers.cmake
│  ├─ StaticAnalysis.cmake
│  └─ Dependencies.cmake
├─ third_party/
├─ docs/
│  ├─ architecture/
│  ├─ algorithms/
│  ├─ design/
│  ├─ testing/
│  └─ user-guide/
├─ apps/
│  ├─ cad_desktop/
│  │  ├─ CMakeLists.txt
│  │  ├─ main.cpp
│  │  ├─ MainWindow.h
│  │  ├─ MainWindow.cpp
│  │  ├─ AppController.h
│  │  └─ AppController.cpp
│  └─ python_tools/
├─ include/
│  └─ mnx/
│     ├─ core/
│     ├─ geometry/
│     ├─ sketch/
│     ├─ constraints/
│     ├─ solver/
│     ├─ commands/
│     ├─ modeling/
│     ├─ topology/
│     ├─ rendering/
│     ├─ persistence/
│     ├─ assembly/
│     └─ scripting/
├─ src/
│  ├─ core/
│  ├─ geometry/
│  ├─ sketch/
│  ├─ constraints/
│  ├─ solver/
│  ├─ commands/
│  ├─ modeling/
│  ├─ topology/
│  ├─ rendering/
│  ├─ persistence/
│  ├─ assembly/
│  └─ scripting/
├─ tests/
│  ├─ core/
│  ├─ geometry/
│  ├─ sketch/
│  ├─ constraints/
│  ├─ solver/
│  ├─ commands/
│  ├─ modeling/
│  ├─ topology/
│  ├─ persistence/
│  ├─ assembly/
│  └─ integration/
├─ scripts/
│  ├─ regression/
│  ├─ benchmarks/
│  └─ generators/
├─ samples/
│  ├─ sketches/
│  ├─ parts/
│  └─ assemblies/
└─ README.md
```

---

# 4. Global Code Style Rules

## 4.1 Naming
- Namespace: `mnx`
- Types / classes / enums: `PascalCase`
- Functions / methods: `camelCase`
- Private members: trailing underscore, e.g. `value_`
- Constants: `kPascalCase`
- Files: `PascalCase.h/.cpp` for types, or logical names by module

## 4.2 Header structure
- `#pragma once`
- minimal includes
- forward declarations where reasonable
- no `using namespace` in headers

## 4.3 Source structure
- include own header first
- internal helpers in anonymous namespace
- keep functions short unless mathematical clarity requires otherwise

## 4.4 Error handling
- Use explicit `Status`, `Result<T>`, or clear exceptions only if a strong reason exists.
- Prefer deterministic, inspectable failure paths.
- Domain-level operations should return rich diagnostics where useful.

## 4.5 Floating point
Use a centralized tolerance utility.  
Never scatter hard-coded `1e-9` or `1e-6` across the codebase unless routed through a named tolerance constant.

---

# 5. Build System Requirements

## 5.1 Root CMake requirements
Root `CMakeLists.txt` must:
- set C++20
- export compile commands
- include custom CMake modules
- add `src`, `apps/cad_desktop`, and `tests`
- enable testing
- define build options for sanitizers / warnings / static analysis

## 5.2 Required CMake modules
Create these files:
- `cmake/Warnings.cmake`
- `cmake/Sanitizers.cmake`
- `cmake/StaticAnalysis.cmake`
- `cmake/Dependencies.cmake`

## 5.3 Library targets
Create one target per major module:
- `mnx_core`
- `mnx_geometry`
- `mnx_sketch`
- `mnx_constraints`
- `mnx_solver`
- `mnx_commands`
- `mnx_modeling`
- `mnx_topology`
- `mnx_rendering`
- `mnx_persistence`
- `mnx_assembly`
- `mnx_scripting`

Then link them deliberately. Avoid an everything-target until maybe the app layer.

## 5.4 Build configurations
Support:
- Debug
- Release

Debug should optionally support:
- AddressSanitizer
- UBSan

---

# 6. Deliverable Phases

Codex must implement in phases.  
At the end of each phase:
- code builds
- tests for that phase pass
- there is no knowingly broken half-state checked in

---

# 7. Phase 0 — Project Skeleton

## Goal
Create a stable engineering scaffold.

## Required files

### Root
- `CMakeLists.txt`
- `.clang-format`
- `.clang-tidy`

### cmake/
- `Warnings.cmake`
- `Sanitizers.cmake`
- `StaticAnalysis.cmake`
- `Dependencies.cmake`

### apps/cad_desktop/
- `CMakeLists.txt`
- `main.cpp`
- `MainWindow.h`
- `MainWindow.cpp`
- `AppController.h`
- `AppController.cpp`

### src/
- `CMakeLists.txt`

### tests/
- `CMakeLists.txt`

## Exact responsibilities

### `apps/cad_desktop/main.cpp`
Must:
- initialize `QApplication`
- construct `MainWindow`
- show it
- return `app.exec()`

### `MainWindow`
Must:
- be a thin UI shell
- own menus, toolbars, docks, central view placeholders
- not own CAD business logic
- delegate to `AppController`

### `AppController`
Must:
- manage high-level application commands
- own current document/session concepts
- handle new/open/save dispatch hooks
- connect UI actions to domain operations
- not implement solver/geometry logic directly

### `src/CMakeLists.txt`
Must define the module library targets even if some are stubbed at first.

### `tests/CMakeLists.txt`
Must define at least one smoke test target and register it with CTest.

## Acceptance criteria
- desktop app launches
- at least one unit test runs
- all targets configure and build cleanly

---

# 8. Phase 1 — Core Module

## Goal
Provide foundational math, ids, status/result, and tolerance handling.

## Required directories
```text
include/mnx/core/
src/core/
tests/core/
```

## Required files

### ids
- `include/mnx/core/ids/EntityId.h`
- `include/mnx/core/ids/FeatureId.h`
- `include/mnx/core/ids/ConstraintId.h`
- `include/mnx/core/ids/BodyId.h`
- `include/mnx/core/ids/ComponentId.h`

### math
- `include/mnx/core/math/Vec2.h`
- `include/mnx/core/math/Vec3.h`
- `include/mnx/core/math/Mat3.h`
- `include/mnx/core/math/Mat4.h`
- `include/mnx/core/math/Transform2D.h`
- `include/mnx/core/math/Transform3D.h`
- `include/mnx/core/math/AABB2.h`
- `include/mnx/core/math/AABB3.h`
- `include/mnx/core/math/Tolerance.h`

### utils
- `include/mnx/core/utils/Status.h`
- `include/mnx/core/utils/Result.h`
- `include/mnx/core/utils/Logger.h`

### tests
- `tests/core/Vec2Tests.cpp`
- `tests/core/Vec3Tests.cpp`
- `tests/core/Transform2DTests.cpp`
- `tests/core/AABB2Tests.cpp`
- `tests/core/ToleranceTests.cpp`

## File-level intent

### `EntityId.h` and related id files
Implement lightweight strongly-typed IDs.
Requirements:
- cheap to copy
- comparable
- hashable
- invalid/default state supported
- no accidental mixing across ID types

A strongly-typed wrapper around an integer or UUID-like integer is fine.

### `Vec2.h`
Implement:
- constructors
- x/y access
- `+ - * /`
- dot
- cross
- `norm`
- `squaredNorm`
- `normalized`
- approximate comparison

Prefer inline implementation.

### `Vec3.h`
Same pattern as `Vec2.h`, with 3D operations.

### `Mat3.h` / `Mat4.h`
Use either Eigen-backed wrappers or aliases plus helper functions.
Must support:
- identity
- multiplication
- transform composition
- common constructors

### `Transform2D.h`
Implement a 2D homogeneous transform abstraction.
Must support:
- translation
- rotation
- scaling
- composition
- inverse
- apply to point / vector

### `AABB2.h` / `AABB3.h`
Implement:
- empty/invalid state
- expand with point
- union
- intersection test
- contains point
- center/size

### `Tolerance.h`
Provide centralized floating-point helpers:
- `almostEqual`
- `isZero`
- named tolerances

### `Status.h` / `Result.h`
Provide simple inspectable success/failure abstractions for domain operations.

## Tests
Tests must be small and deterministic. Cover:
- arithmetic correctness
- zero vector normalization behavior
- transform composition
- AABB edge cases
- tolerance correctness

## Acceptance criteria
- module builds
- tests pass
- no UI dependencies
- API is reusable by geometry and sketch modules

---

# 9. Phase 2 — Geometry Kernel

## Goal
Implement reusable geometry primitives and algorithms.

## Required directories
```text
include/mnx/geometry/
src/geometry/
tests/geometry/
```

## Required files

### primitives
- `Point2.h`
- `Point3.h`
- `Line2D.h`
- `Segment2D.h`
- `Circle2D.h`
- `Arc2D.h`
- `Polyline2D.h`

### abstract geometry
- `Curve2D.h`
- `Bezier2D.h`
- `BSpline2D.h`
- `Plane.h`
- `Surface.h`
- `PlanarSurface.h`

### algorithms
- `algorithms/Intersections2D.h`
- `algorithms/Intersections2D.cpp`
- `algorithms/Projections2D.h`
- `algorithms/Projections2D.cpp`
- `algorithms/Distances2D.h`
- `algorithms/Distances2D.cpp`
- `algorithms/Bounds2D.h`
- `algorithms/Bounds2D.cpp`
- `algorithms/CurveSampling.h`
- `algorithms/CurveSampling.cpp`

### tests
- `Line2DTests.cpp`
- `Circle2DTests.cpp`
- `Arc2DTests.cpp`
- `IntersectionTests.cpp`
- `ProjectionTests.cpp`

## Required behavior

### `Point2.h` / `Point3.h`
Keep them semantically distinct from vectors:
- point - point = vector
- point + vector = point

### `Line2D.h`
Represent an infinite line.
Must support:
- projection of point
- point-line distance
- direction access
- construction from point + direction

### `Segment2D.h`
Must support:
- start/end points
- length
- midpoint
- bounds
- clamped projection

### `Circle2D.h`
Must support:
- center
- radius
- point evaluation
- bounds

### `Arc2D.h`
Must support:
- center
- radius
- start angle
- end angle
- direction or sweep interpretation
- parameter evaluation

### `Curve2D.h`
Abstract base class with at least:
- `evaluate(double t)`
- `tangent(double t)`
- `bounds()`

### `Bezier2D.h`
Implement at least cubic evaluation.

### `BSpline2D.h`
Can be a minimal scaffold first if necessary, but define the abstraction cleanly.

### `Plane.h`
Must support:
- point
- normal
- signed distance
- local basis generation

### `Intersections2D`
Support at minimum:
- line-line
- segment-segment
- line-circle
- circle-circle
- segment-circle

Return a structured result, not bare booleans.

## Acceptance criteria
- geometry module builds independently
- robust tests cover edge cases
- no Qt dependencies
- primitives are ready for sketch usage

---

# 10. Phase 3 — Sketch Data Model

## Goal
Represent editable 2D sketch entities independent of GUI.

## Required files

- `include/mnx/sketch/SketchEntityType.h`
- `include/mnx/sketch/SketchEntity.h`
- `include/mnx/sketch/SketchPoint.h`
- `include/mnx/sketch/SketchLine.h`
- `include/mnx/sketch/SketchCircle.h`
- `include/mnx/sketch/SketchArc.h`
- `include/mnx/sketch/SketchSpline.h`
- `include/mnx/sketch/SketchPlane.h`
- `include/mnx/sketch/Sketch.h`
- `include/mnx/sketch/GeometryRef.h`
- `include/mnx/sketch/SelectionState.h`
- source files for each
- tests for create/delete/query/bounds/ref resolution

## Required behavior

### `SketchEntity.h`
Abstract base class.
Must expose:
- id
- type
- bounds
- transform
- handle access for sub-entity references

### `SketchPoint`
Own a point.

### `SketchLine`
Prefer start/end point storage, since editing and constraints are endpoint-based.

### `SketchCircle`
Store center + radius.

### `SketchArc`
Store center + radius + start/end angles.

### `SketchSpline`
May initially be a structural placeholder, but define the class cleanly.

### `GeometryRef.h`
Must define sub-entity references, e.g.:
- whole entity
- start point
- end point
- center point
- midpoint

### `SketchPlane.h`
Define sketch-local 2D <-> 3D mapping.

### `Sketch.h`
Must manage:
- entity insertion
- entity deletion
- entity lookup by ID
- entity iteration
- sketch bounds
- associated constraint IDs

Internal storage should be deterministic enough for serialization and tests.

## Acceptance criteria
- sketch module works with no GUI
- entities and refs can be queried correctly
- tests pass

---

# 11. Phase 4 — Sketch GUI and Interaction

## Goal
Implement a usable 2D sketch editor shell.

## Required files

- `include/mnx/sketch/gui/SketchView.h`
- `include/mnx/sketch/gui/SketchController.h`
- `include/mnx/sketch/gui/SketchRenderer.h`
- `include/mnx/sketch/gui/SelectionManager.h`
- `include/mnx/sketch/gui/SnapManager.h`
- `include/mnx/sketch/gui/Tool.h`
- `include/mnx/sketch/gui/SelectTool.h`
- `include/mnx/sketch/gui/LineTool.h`
- `include/mnx/sketch/gui/CircleTool.h`
- `include/mnx/sketch/gui/ArcTool.h`
- `include/mnx/sketch/gui/RectangleTool.h`
- source files for all

## Separation of responsibilities

### `SketchView`
Qt widget / view object.
Must:
- render current sketch
- receive mouse input
- map screen coords to sketch coords
- not own all editing business logic

### `SketchController`
Must:
- own current active tool
- coordinate selection and snap
- convert UI actions into document/model operations
- be the bridge between view and command/model layers

### `SketchRenderer`
Must:
- draw entities
- draw highlights
- draw overlays
- remain stateless or minimally stateful

### `SelectionManager`
Must:
- manage selected entities/handles
- support hit test results

### `SnapManager`
Must:
- detect snap targets:
  - endpoint
  - midpoint
  - center
  - optional grid

### Tool classes
Each tool handles its own interaction state.

## Minimum implemented tools
- select
- line
- circle
- rectangle

Arc may come slightly later if necessary, but the class/file should still exist.

## Acceptance criteria
- user can draw line/circle/rectangle
- user can select entities
- user sees hover/selection feedback
- snapping works at least for endpoints and circle centers

---

# 12. Phase 5 — Constraint Data Model

## Goal
Represent sketch constraints declaratively and cleanly.

## Required files
- `ConstraintType.h`
- `Constraint.h`
- `CoincidentConstraint.h`
- `HorizontalConstraint.h`
- `VerticalConstraint.h`
- `ParallelConstraint.h`
- `PerpendicularConstraint.h`
- `DistanceConstraint.h`
- `AngleConstraint.h`
- `RadiusConstraint.h`
- `TangentConstraint.h`
- `EqualLengthConstraint.h`
- `Dimension.h`

## Requirements

### `Constraint.h`
Abstract base class.
Must expose:
- id
- type
- geometry refs used
- whether it is driving/reference, if applicable
- a label or debug description

### Constraint subclasses
Implement clean data storage only.
Do not entangle solve-state into the constraint classes.

### `Dimension.h`
Represents visible/editable dimensional annotation state.
Should connect UI-readable dimensions with the underlying driving constraint.

## Acceptance criteria
- constraints can be created and attached to sketch elements
- refs are validated
- tests cover valid/invalid cases

---

# 13. Phase 6 — Constraint Solver

## Goal
Solve sketch geometry based on dimensional and geometric constraints.

## Required files
- `SolveResult.h`
- `SolverDiagnostics.h`
- `SolverVariable.h`
- `VariableRegistry.h`
- `ConstraintEquation.h`
- `EquationBuilder.h`
- `JacobianBuilder.h`
- `SketchSolver.h`
- corresponding `.cpp` files

## Solver approach
Implement a practical prototype solver, not an industrial-grade kernel solver.

Recommended first implementation:
- variable extraction from sketch geometry
- residual equations per constraint
- numerical Jacobian
- Gauss-Newton iteration
- write-back to sketch geometry

## Required supported constraints in first functional version
1. coincident
2. horizontal
3. vertical
4. point-point distance
5. radius
6. parallel
7. perpendicular

Then add:
8. angle
9. tangent
10. equal length

## Diagnostics requirements
Return structured information:
- converged / not converged
- iteration count
- final residual
- likely conflicting constraints
- under-constrained indication where possible
- singular / ill-conditioned warnings

## Tests
Must include:
- rectangle with dimensions
- triangle with distances
- line-circle tangency
- inconsistent constraints
- under-constrained scenario

## Acceptance criteria
- common sketches solve reliably enough for demos
- solver is separated from UI
- tests cover both success and failure cases

---

# 14. Phase 7 — Command System / Undo-Redo

## Goal
All editing operations should become undoable.

## Required files
- `Command.h`
- `CommandManager.h`
- `CompositeCommand.h`
- `CreateSketchEntityCommand.h`
- `DeleteSketchEntityCommand.h`
- `MoveSketchHandleCommand.h`
- `AddConstraintCommand.h`
- `RemoveConstraintCommand.h`
- `EditDimensionCommand.h`
- `CreateFeatureCommand.h`
- `InsertComponentCommand.h`

## Requirements

### `Command.h`
Pure command interface with:
- execute
- undo
- label

### `CommandManager.h`
Manages:
- undo stack
- redo stack
- execution

### Command classes
Must act on model/document state, not directly on widgets.

## Acceptance criteria
- create/delete/edit operations are undoable
- dimension edits are undoable
- feature creation is undoable

---

# 15. Phase 8 — Feature History Tree

## Goal
Move from a sketch editor to a feature-based CAD workflow.

## Required files
- `FeatureType.h`
- `Feature.h`
- `FeatureContext.h`
- `FeatureResult.h`
- `FeatureGraph.h`
- `RebuildEngine.h`
- `SketchFeature.h`
- `ExtrudeFeature.h`
- `RevolveFeature.h`

## Requirements

### `Feature.h`
Abstract base class with:
- ID
- type
- execute(context)
- dependency list access

### `FeatureGraph`
Tracks:
- feature ordering
- dependency relations
- dirty state

### `RebuildEngine`
Responsible for:
- rebuild from scratch
- rebuild from dirty feature onward
- propagating failures
- diagnostics

### `SketchFeature`
Wraps a sketch into the feature pipeline.

### `ExtrudeFeature`
Must:
- validate profile
- generate 3D result
- return body/mesh/topology references

### `RevolveFeature`
Must:
- validate profile and axis
- generate 3D result

## UI expectations
Create feature-tree-related UI files:
- `FeatureTreeWidget.h/.cpp`
- `FeaturePropertyPanel.h/.cpp`

Feature tree must support:
- listing features
- selecting feature
- editing parameters
- triggering rebuild

## Acceptance criteria
- parameter edit causes rebuild
- feature dependency ordering works
- user can inspect feature history

---

# 16. Phase 9 — Topology / Simplified B-Rep

## Goal
Represent 3D solid topology explicitly enough to look like a CAD kernel prototype.

## Required files
- `Vertex.h`
- `Edge.h`
- `CoEdge.h`
- `Loop.h`
- `Face.h`
- `Shell.h`
- `Body.h`
- `TopologyValidator.h`
- `TopologyQueries.h`

## Data model requirements

### `Vertex`
- point geometry reference or direct geometry payload

### `Edge`
- start/end vertices
- underlying curve geometry ref

### `CoEdge`
- oriented usage of an edge in a loop

### `Loop`
- ordered coedges

### `Face`
- underlying surface ref
- outer loop
- optional inner loops

### `Shell`
- collection of faces

### `Body`
- collection of shells
- bounds / metadata

## Important
Topology must be separate from:
- rendered mesh caches
- GUI selection state

## Acceptance criteria
- extrude/revolve can populate topology
- body adjacency queries work
- topology validator catches obvious broken states

---

# 17. Phase 10 — 3D Modeling (Extrude / Revolve Builders)

## Goal
Create usable 3D geometry from sketch profiles.

## Required files
- `ProfileUtils.h/.cpp`
- `ExtrudeBuilder.h/.cpp`
- `RevolveBuilder.h/.cpp`
- `MeshBuilder.h/.cpp`
- `BodyBuilder.h/.cpp`

## `ProfileUtils`
Must:
- detect closed profiles
- extract loops
- compute loop orientation
- reject unsupported invalid profiles

## `ExtrudeBuilder`
Must:
- take closed profile
- generate caps and side faces
- build display mesh
- build simplified topology

## `RevolveBuilder`
Must:
- revolve around chosen axis
- support angle parameter
- build display mesh
- build simplified topology

## Acceptance criteria
- rectangle extrudes to box-like body
- circle extrudes to cylinder-like body
- revolved profile produces a stable mesh/body

---

# 18. Phase 11 — Rendering / 3D Viewport

## Goal
Render bodies in 3D and allow basic navigation.

## Required files
- `Camera.h/.cpp`
- `SceneView.h/.cpp`
- `RenderScene.h/.cpp`
- `RenderItem.h/.cpp`
- `Mesh.h/.cpp`
- `MeshBuffer.h/.cpp`
- `BodyRenderer.h/.cpp`
- `Picking.h/.cpp`

## Requirements

### `Camera`
Support:
- orbit
- pan
- zoom
- view/projection matrix generation

### `SceneView`
Qt OpenGL widget that:
- draws scene
- receives mouse input
- updates camera
- performs picking

### `RenderScene`
Owns render items.

### `RenderItem`
Wraps transform/visibility/selection flags plus geometry handles or mesh data.

### `Picking`
First implementation can be CPU-based.
At minimum support:
- ray vs AABB
- ray vs triangle mesh

## Acceptance criteria
- user can orbit/pan/zoom
- generated bodies are visible
- body selection/highlight works at basic level

---

# 19. Phase 12 — Persistence

## Goal
Save and restore project state.

## Required files
- `SchemaVersion.h`
- `JsonConverters.h`
- `ProjectSerializer.h`
- `ProjectDeserializer.h`
- `PersistenceUtils.h`

## Requirements
Save enough state to restore:
- sketches
- constraints
- features
- bodies
- assemblies
- component transforms
- parameters
- document metadata

Use a versioned schema.

Prefer a simple JSON-based first implementation.

## Tests
Must include:
- round-trip serialization
- loading invalid schema gracefully
- backwards-compatible design hooks even if only one version exists initially

## Acceptance criteria
- project can be saved and loaded
- reopened project rebuilds or restores correctly

---

# 20. Phase 13 — Assembly

## Goal
Allow composition of multiple parts into an assembly.

## Required files
- `AssemblyDocument.h/.cpp`
- `ComponentInstance.h/.cpp`
- `MateType.h`
- `MateConstraint.h/.cpp`
- `AssemblySolver.h/.cpp`
- `TransformNode.h/.cpp`
- `AssemblyQueries.h/.cpp`

## Required behavior
- insert part into assembly as component instance
- support multiple instances of same part
- maintain local transform per instance
- simple hierarchy support
- implement first mates:
  - fixed
  - coincident
  - distance
  - parallel

## UI files
- `AssemblyTreeWidget.h/.cpp`
- `MateEditorPanel.h/.cpp`

## Acceptance criteria
- multiple components visible
- transforms persist
- simple mates update instance placement

---

# 21. Phase 14 — Python Bindings

## Goal
Expose useful automation API for testing and demos.

## Required files
- `PythonModule.h/.cpp`
- `PythonBindingsPart.h/.cpp`
- `PythonBindingsSketch.h/.cpp`
- `PythonBindingsFeature.h/.cpp`
- `PythonBindingsExport.h/.cpp`

## API priorities
Expose a small but useful API surface:
- create part
- new sketch
- add line
- add circle
- add distance constraint
- solve sketch
- extrude
- revolve
- save project
- export STL

## Scripts
Create:
- `scripts/regression/*.py`
- `scripts/benchmarks/*.py`
- `scripts/generators/*.py`

## Acceptance criteria
- Python can generate at least one part end-to-end
- Python can run a regression or benchmark script

---

# 22. Testing Requirements

Testing is mandatory, not optional.

## 22.1 Required categories
1. unit tests
2. integration tests
3. regression tests

## 22.2 Unit tests
Must exist for:
- core
- geometry
- sketch
- constraints
- solver
- topology
- persistence

## 22.3 Integration tests
Must exist for:
- sketch + solver
- sketch + feature
- feature + persistence
- assembly + persistence

## 22.4 Regression corpus
Create regression cases under `tests/integration/regression_cases/`.

Each case should contain:
- input project or generated setup
- expected result summary
- optional output hash or topology summary
- note describing bug or scenario

## 22.5 Test philosophy
Prefer deterministic tests.  
Avoid fragile GUI interaction tests unless necessary.  
Most logic should be testable without the GUI.

---

# 23. Documentation Requirements

Codex must generate and maintain docs while implementing.

## Required docs
- `README.md`
- `docs/architecture/overview.md`
- `docs/algorithms/sketch_solver.md`
- `docs/design/topology.md`
- `docs/design/features.md`
- `docs/testing/test_plan.md`

## README must include
- project summary
- features
- tech stack
- build instructions
- test instructions
- screenshots placeholder or references
- repository structure

## Architecture doc must include
- module list
- dependency direction
- why UI/domain/rendering are separated

---

# 24. Dependency Direction Rules

These dependency rules are mandatory.

## Allowed general direction
- `core` -> nothing internal
- `geometry` -> `core`
- `sketch` -> `core`, `geometry`
- `constraints` -> `core`, `geometry`, `sketch`
- `solver` -> `core`, `geometry`, `sketch`, `constraints`
- `commands` -> domain modules as needed
- `modeling` -> `core`, `geometry`, `sketch`, `constraints`, `solver`, `topology`
- `topology` -> `core`, `geometry`
- `rendering` -> `core`, `geometry`, `topology`, maybe modeling result data, but not vice versa
- `persistence` -> may depend on most domain modules, but domain modules should not depend on persistence
- `assembly` -> `core`, `geometry`, `modeling`, `topology`
- `scripting` -> depends on domain modules
- `apps` -> depends on all app-relevant modules

## Forbidden
- `core` depending on UI or rendering
- `geometry` depending on Qt
- `solver` depending on Qt
- `topology` depending on rendering
- `modeling` depending on widget classes
- persistence logic embedded directly inside Qt views

---

# 25. Required Implementation Sequence for Codex

Follow this exact sequence as closely as possible.

## Step A
Create project scaffold and build system.

## Step B
Implement `core` module and tests.

## Step C
Implement `geometry` module and tests.

## Step D
Implement `sketch` data model and tests.

## Step E
Implement basic sketch GUI tools.

## Step F
Implement constraint model.

## Step G
Implement solver and solver tests.

## Step H
Implement command manager and sketch edit undo-redo.

## Step I
Implement feature graph, sketch feature, extrude feature.

## Step J
Implement revolve feature.

## Step K
Implement simplified topology and connect feature results to topology.

## Step L
Implement 3D viewport.

## Step M
Implement persistence.

## Step N
Implement assembly basics.

## Step O
Implement Python bindings.

## Step P
Expand regression corpus, docs, samples.

At every step, ensure build and tests remain green.

---

# 26. Definition of Done per Phase

A phase is not done when files merely exist.  
A phase is done only when all are true:

1. code compiles
2. tests for that phase pass
3. public API headers are reasonably documented or self-explanatory
4. no knowingly dead placeholder calls in core logic paths
5. there is at least one minimal integration path proving the phase is usable

---

# 27. File Templates / Class Patterns

## 27.1 Class header template
```cpp
#pragma once

namespace mnx {

class Example {
public:
    Example();
    void doSomething();

private:
    int value_ = 0;
};

} // namespace mnx
```

## 27.2 Source template
```cpp
#include "mnx/module/Example.h"

namespace mnx {

Example::Example() = default;

void Example::doSomething() {
    // implementation
}

} // namespace mnx
```

## 27.3 Test template
```cpp
#include <gtest/gtest.h>
#include "mnx/core/math/Vec2.h"

using namespace mnx;

TEST(Vec2Tests, AdditionWorks) {
    Vec2 a(1.0, 2.0);
    Vec2 b(3.0, 4.0);
    Vec2 c = a + b;
    EXPECT_DOUBLE_EQ(c.x(), 4.0);
    EXPECT_DOUBLE_EQ(c.y(), 6.0);
}
```

---

# 28. Quality Bar

## 28.1 Minimum engineering bar
- no giant god classes
- no module-level architectural mess
- no testless math/solver code
- no obvious resource leaks
- no raw owning pointers where ownership is ambiguous

## 28.2 Preferred ownership style
Use:
- `std::unique_ptr` for exclusive ownership
- `std::shared_ptr` sparingly and intentionally
- references / non-owning pointers only where lifetime is clear

## 28.3 Qt boundary
Prefer keeping Qt types at the view/controller boundary.  
Avoid infecting pure geometry/solver code with Qt classes.

---

# 29. Known Difficult Areas and How to Handle Them

## 29.1 Solver robustness
Do not overengineer immediately.
Start with:
- limited supported constraints
- numerical Jacobian
- clear diagnostics
- many tests

## 29.2 Topology complexity
Implement a simplified topology model.
Correct architecture is more important than industrial completeness.

## 29.3 Boolean operations
These are optional extensions unless enough time exists.
Do not block core milestones on full boolean support.

## 29.4 Rendering complexity
Use the simplest functional OpenGL path.
A stable viewport beats a flashy renderer.

---

# 30. Stretch Goals (Only After Core Goals Work)

Only attempt after all main phases are stable:
- B-spline editing UI
- fillet/chamfer features
- pattern feature
- mirror feature
- section view
- hidden-line rendering
- better picking granularity (face/edge)
- mesh boolean prototype
- plugin architecture

These are not phase blockers.

---

# 31. Final Acceptance Checklist

The implementation is ready when all are true:

- [ ] Project configures with CMake
- [ ] Desktop app launches
- [ ] Unit tests run
- [ ] Core math implemented
- [ ] Geometry kernel implemented
- [ ] Sketch data model implemented
- [ ] Basic sketch editing works
- [ ] Constraints implemented
- [ ] Solver works on common cases
- [ ] Undo/redo works
- [ ] Feature history tree works
- [ ] Extrude works
- [ ] Revolve works
- [ ] Simplified topology exists
- [ ] 3D viewport works
- [ ] Save/load works
- [ ] Assembly basics work
- [ ] Python bindings work
- [ ] Docs exist
- [ ] Regression cases exist

---

# 32. Execution Guidance for Codex

When coding, always think in this order:
1. what module owns this responsibility?
2. what header/API should exist first?
3. how can this be tested without the GUI?
4. how should UI call into it, rather than contain it?
5. what minimal version solves the phase requirement cleanly?

When unsure, choose the design that:
- reduces coupling
- increases testability
- keeps future feature additions straightforward

This codebase should feel like a serious engineering software prototype, not a jam-session demo.
