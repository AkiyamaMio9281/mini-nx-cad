# Feature History Design

Mini NX uses a feature-history model so geometry can be rebuilt from ordered, parameterized operations instead of being edited as isolated mesh data.

## Core Types

- `Feature`: polymorphic base with ID, type, name, dependencies, and `execute`.
- `FeatureGraph`: ordered feature container with dependency validation and dirty state.
- `FeatureContext`: execution context used during rebuild.
- `FeatureResult`: status, diagnostics, body IDs, and generated bodies.
- `RebuildEngine`: orchestrates full or dirty rebuilds.

## Rebuild Flow

1. Features are inserted into `FeatureGraph` in execution order.
2. Dependencies must point to earlier features.
3. Dirty state propagates to dependents.
4. `RebuildEngine` starts from the first dirty feature or from scratch.
5. If a feature fails, later features are marked dirty and diagnostics are returned.

This mirrors the basic behavior of a commercial CAD feature tree without attempting advanced rollback/edit modes yet.

## Implemented Features

### SketchFeature

Wraps a `Sketch` so downstream modeling features can consume sketch profiles.

### ExtrudeFeature

Consumes a closed sketch profile and a blind distance. It produces:

- Display mesh
- Simplified `Body`
- Topology hooks derived from the mesh

### RevolveFeature

Consumes a supported sketch profile, an axis, and an angle. It produces a stable display mesh and simplified body.

## Profile Handling

`ProfileUtils` detects supported closed profiles and rejects invalid/open profiles. Current profile support favors deterministic demo workflows:

- Rectangle/polyline-style closed profiles
- Circle profile extrusion
- Revolve profiles that do not cross the axis

Concave profiles and advanced loop nesting are intentionally limited.

## UI Integration

The feature tree UI lists features and shows dirty/clean state. Property panel support is currently inspect-only and structured for future editable parameters.

## Persistence

Project persistence stores feature definitions, sketch entities, constraints, and parameters. Display meshes and topology are considered rebuildable outputs.

## Limitations

- No Boolean feature operations.
- No industrial topology naming.
- No editable feature parameter panel beyond shell support.
- No rollback bar or partial history editing workflow yet.
