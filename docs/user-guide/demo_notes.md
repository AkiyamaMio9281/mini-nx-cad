# Demo Notes

This project currently stores text-based demo assets instead of checked-in screenshots. The UI and rendering layers are present, but the current verification environment does not provide Qt/CMake, so screenshots should be generated on a workstation with Qt 6 installed.

## Suggested Screenshots

Capture these once the desktop app is built:

1. Sketch tab showing a rectangle, line, circle, selection highlight, and snap feedback.
2. 3D tab showing a rebuilt extrude body.
3. Feature tree dock with sketch and extrude/revolve features.
4. Assembly dock showing multiple component instances.
5. Python-generated part loaded from `samples/parts/rectangle_extrude.mnx.json`.

Recommended output path:

```text
docs/user-guide/screenshots/
```

Suggested filenames:

```text
sketch_editor.png
extrude_viewport.png
feature_tree.png
assembly_tree.png
python_generated_part.png
```

## Demo Flow

1. Build the project with Qt enabled.
2. Launch `mnx_cad_desktop`.
3. Open or load sample part data once file dialogs are implemented, or use current controller hooks/default project path.
4. Switch between Sketch and 3D tabs.
5. Use the feature tree, assembly tree, and property panels to show module separation.
6. Run `scripts/generators/generate_bracket.py` after building the pybind11 module.

## Talking Points

- UI is intentionally thin and delegates to controllers/domain modules.
- Feature rebuilds regenerate display bodies instead of storing rendering state in project files.
- Topology and rendering meshes are separate concepts.
- Python automation uses a facade and does not contain independent modeling logic.
