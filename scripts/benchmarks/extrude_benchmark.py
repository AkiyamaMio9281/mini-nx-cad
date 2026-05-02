from __future__ import annotations

import pathlib
import sys
import tempfile
import time

sys.path.insert(0, str(pathlib.Path(__file__).resolve().parents[1]))
from mini_nx_bootstrap import load_mini_nx


def main() -> int:
    mini_nx = load_mini_nx()
    if mini_nx is None:
        return 0

    iterations = 25
    start = time.perf_counter()
    with tempfile.TemporaryDirectory() as temp:
        temp_dir = pathlib.Path(temp)
        for index in range(iterations):
            part = mini_nx.create_part(f"bench_extrude_{index}")
            part.new_sketch()
            part.add_line(0.0, 0.0, 1.0, 0.0)
            part.add_line(1.0, 0.0, 1.0, 1.0)
            part.add_line(1.0, 1.0, 0.0, 1.0)
            part.add_line(0.0, 1.0, 0.0, 0.0)
            part.extrude(1.0)
            part.export_stl(str(temp_dir / f"bench_{index}.stl"))
    elapsed = time.perf_counter() - start
    print(f"{iterations} extrude/export iterations in {elapsed:.4f}s")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
