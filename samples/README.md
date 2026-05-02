# Samples

These sample files are small, text-based assets intended for demos, regression review, and manual loading experiments.

## Sketches

- `sketches/basic_rectangle_sketch.mnx.json`: a single sketch feature containing a closed rectangle.

## Parts

- `parts/rectangle_extrude.mnx.json`: rectangle sketch plus blind extrude.
- `parts/revolve_pin.mnx.json`: simple rectangular profile revolved around an offset vertical axis.

## Assemblies

- `assemblies/two_box_assembly.mnx_assembly.json`: two component instances with a distance mate.

## Notes

The current persistence format stores rebuildable modeling state. Generated display meshes and topology are rebuilt by the feature pipeline rather than stored as authoritative project data.
