from __future__ import annotations

import pathlib
import sys
import tempfile

sys.path.insert(0, str(pathlib.Path(__file__).resolve().parents[1]))
from mini_nx_bootstrap import load_mini_nx


def main() -> int:
    mini_nx = load_mini_nx()
    if mini_nx is None:
        return 0

    with tempfile.TemporaryDirectory() as temp:
        output = pathlib.Path(temp) / "basic_extrude.stl"
        part = mini_nx.Part("basic_extrude_regression")
        part.new_sketch()
        part.add_line(0.0, 0.0, 2.0, 0.0)
        part.add_line(2.0, 0.0, 2.0, 1.0)
        part.add_line(2.0, 1.0, 0.0, 1.0)
        part.add_line(0.0, 1.0, 0.0, 0.0)
        part.extrude(0.75)
        part.export_stl(str(output))
        text = output.read_text(encoding="utf-8")
        if "facet normal" not in text:
            raise RuntimeError("STL export did not contain any facets")
    print("basic_extrude_regression passed")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
