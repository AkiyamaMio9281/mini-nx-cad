from __future__ import annotations

import pathlib
import sys

sys.path.insert(0, str(pathlib.Path(__file__).resolve().parents[1]))
from mini_nx_bootstrap import load_mini_nx


def main() -> int:
    mini_nx = load_mini_nx()
    if mini_nx is None:
        return 0

    out_dir = pathlib.Path(__file__).resolve().parents[2] / "samples" / "parts"
    out_dir.mkdir(parents=True, exist_ok=True)

    part = mini_nx.create_part("scripted_bracket")
    part.new_sketch()
    part.add_line(0.0, 0.0, 4.0, 0.0)
    part.add_line(4.0, 0.0, 4.0, 2.0)
    part.add_line(4.0, 2.0, 0.0, 2.0)
    part.add_line(0.0, 2.0, 0.0, 0.0)
    part.extrude(1.0)
    part.save_project(str(out_dir / "scripted_bracket.mnx.json"))
    part.export_stl(str(out_dir / "scripted_bracket.stl"))
    print("Generated scripted_bracket project and STL")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
