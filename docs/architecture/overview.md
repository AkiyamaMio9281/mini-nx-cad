# Architecture Overview

Mini NX is organized as a layered CAD prototype. Each module owns one kind of responsibility and depends only on lower-level modules. The shape is intentionally conservative: a boring dependency graph is easier to test, debug, and extend than a clever one.

## Module Map

| Module | Responsibility | Depends On |
| --- | --- | --- |
| `core` | IDs, math, transforms, AABBs, tolerance, status/result | none |
| `geometry` | Points, curves, planes, 2D algorithms | `core` |
| `sketch` | Editable sketch entities and geometry refs | `core`, `geometry` |
| `constraints` | Declarative sketch constraints and dimensions | `core`, `geometry`, `sketch` |
| `solver` | Numerical sketch constraint solving | `core`, `geometry`, `sketch`, `constraints` |
| `commands` | Undo/redo command objects | domain modules |
| `topology` | Simplified B-Rep structures and queries | `core`, `geometry` |
| `modeling` | Feature graph, rebuild engine, extrude/revolve builders | `core`, `geometry`, `sketch`, `constraints`, `solver`, `topology` |
| `rendering` | Camera, render scene/items, mesh buffers, picking | `core`, `geometry`, `topology`, `modeling` |
| `persistence` | Versioned JSON serialization/deserialization | domain modules |
| `assembly` | Component instances, transforms, mates, assembly queries | `core`, `geometry`, `modeling`, `topology` |
| `scripting` | Python automation facade and pybind11 bindings | domain modules |
| `apps/cad_desktop` | Qt application shell and UI panels | app-relevant modules |

## Dependency Rules

The core rule is that model logic never points upward into UI, persistence, or scripting. Qt is allowed in the desktop app, sketch GUI, and Qt rendering widget, but not in pure geometry, sketch, solver, topology, modeling, assembly, or persistence logic.

Rendering consumes display data and body outputs; it does not own CAD document state. Persistence serializes domain state; domain modules do not call persistence. Python bindings are thin wrappers over existing domain APIs and do not contain Python-only modeling behavior.

## Document Flow

The current part flow is:

1. A `PartDocument` owns a `FeatureGraph`.
2. `SketchFeature` wraps a reusable `Sketch`.
3. Modeling features such as `ExtrudeFeature` and `RevolveFeature` depend on prior sketch features.
4. `RebuildEngine` executes features in order and propagates failures.
5. Feature results include simplified `Body` objects with display meshes and topology hooks.
6. `BodyRenderer` converts bodies to render items for the viewport.
7. `ProjectSerializer` stores rebuildable state rather than GPU or widget state.

## UI Boundary

`MainWindow` owns menus, docks, tabs, and widgets. `AppController` coordinates application-level actions and owns the current part/assembly/render scene concepts. Domain work remains in modules below the app:

- Sketch widgets forward input to `SketchController`.
- Feature tree and property panels inspect feature graph data.
- Assembly tree and mate panel inspect assembly state.
- Scene view displays a copied/renderable scene rather than owning the model.

## Ownership

Exclusive ownership is the default:

- Feature graph stores `std::unique_ptr<Feature>`.
- Sketch stores `std::unique_ptr<SketchEntity>`.
- Constraints are cloned where document-level ownership needs a stable copy.

Shared ownership is used sparingly in assembly component instances because multiple component instances can intentionally reference the same part document.

## Extension Points

The most natural next additions are:

- Persistence package format that stores part documents referenced by assemblies.
- More robust profile extraction and triangulation.
- Face/edge level selection in rendering.
- Richer Python automation for constraints, assemblies, and project loading.
- Higher-fidelity topology not derived from display triangulation.
