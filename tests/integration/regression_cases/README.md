# Regression Cases

This directory stores human-readable regression metadata for workflows and edge cases that should remain stable. The executable unit tests live in the normal test directories; these case files explain why each scenario matters and point to the source tests or sample data that exercise it.

Each `case.json` uses:

- `id`: stable case identifier
- `title`: short scenario name
- `modules`: relevant modules
- `sample`: optional sample asset
- `coveredBy`: test or script coverage
- `expected`: expected behavior
- `notes`: reason the case exists
