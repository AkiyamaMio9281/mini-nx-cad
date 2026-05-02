from __future__ import annotations

import importlib
import pathlib
import sys


def load_mini_nx():
    repo_root = pathlib.Path(__file__).resolve().parents[1]
    build_dir = repo_root / "build"
    if str(build_dir) not in sys.path:
        sys.path.insert(0, str(build_dir))
    try:
        return importlib.import_module("mini_nx")
    except ImportError as error:
        print(f"mini_nx Python extension is not available: {error}")
        print("Build the pybind11 target first, then rerun this script.")
        return None
