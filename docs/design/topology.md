# Simplified Topology Design

Mini NX includes a simplified B-Rep style topology layer. It is not an industrial CAD kernel, but it gives the project explicit topological ownership and traversal concepts separate from display meshes.

## Data Model

The topology module contains:

- `Vertex`: point payload
- `Edge`: start and end vertex indices
- `CoEdge`: oriented use of an edge inside a loop
- `Loop`: ordered coedges
- `Face`: outer loop plus future inner-loop hooks
- `Shell`: collection of faces
- `Body`: owning container for vertices, edges, coedges, loops, faces, and shells

Indices are used as stable internal references within a body. The ownership model is intentionally simple and deterministic for tests and serialization work.

## Geometry vs Topology

Geometry answers "what is the shape?" Topology answers "how are shape elements connected?"

Current topology mostly mirrors feature display meshes:

- Extrude and revolve builders generate a `DisplayMesh`.
- `BodyBuilder` converts mesh triangles into topology vertices, edges, loops, faces, and shells.
- The result gives traversal and validation hooks while the project grows toward richer surfaces.

This means the current topology is useful but not yet a full analytic B-Rep.

## Queries

`TopologyQueries` currently supports:

- Edges of a face
- Faces of a body
- Vertices of an edge
- Adjacent faces of an edge

These queries back tests and are the first step toward face/edge selection and feature operations.

## Validation

`TopologyValidator` catches obvious invalid states:

- Loops with too few coedges
- Coedges referencing missing edges
- Edges referencing missing vertices
- Faces referencing missing loops
- Shells referencing missing faces or no faces

The validator is deliberately conservative: it catches structural mistakes, not all geometric invalidity.

## Limitations

- Topology is triangle-face oriented today.
- Analytic surfaces are not yet stored per face.
- No Boolean operations, healing, sewing, or tolerant topology.
- No persistent naming strategy beyond current IDs and indices.
- No face/edge-level UI selection yet.

## Next Steps

The natural next improvements are:

- Store analytic face surface information for extrude/revolve faces.
- Add stable topology naming for rebuilds.
- Add edge and face picking in the renderer.
- Add topology-aware feature operations such as fillet/chamfer prototypes.
