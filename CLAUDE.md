# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build

```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

Produces two binaries: `task` (CLI) and `task_tests`. A `build/` directory with MSVC/CMake 3.25 files already exists in the repo.

## Run tests

```bash
cd build
./task_tests          # run directly
ctest --output-on-failure   # or via CTest
```

The test binary uses a homegrown `TEST(name, expr)` macro — no external test framework. Tests write to a temp file (`$TEMP/taskflow_test.dat` on Windows, `$TMPDIR/taskflow_test.dat` on Unix) and clean up after themselves.

## Architecture

All business logic lives in headers; `src/main.cpp` is only CLI glue.

| File | Responsibility |
|---|---|
| `include/task.h` | `Task` struct · `TaskStore` (flat-file I/O) · `TaskManager` (business logic) |
| `include/ui.h` | `UI::` namespace — all terminal output (list, stats, help, ok/err) |
| `include/color.h` | `Color::` namespace — ANSI escape code helpers |
| `src/main.cpp` | CLI argument parsing; constructs `TaskManager`, dispatches to `UI::` |
| `tests/test_task.cpp` | Unit tests for `TaskManager` (20 tests, 6 test groups) |

**Storage format:** one task per line, pipe-delimited: `id|done|created_at|title`  
**Storage path:** `~/.taskflow` by default; override with `TASKFLOW_FILE` env var.  
**Windows note:** `storageFile()` in `main.cpp` reads `HOME` (not `USERPROFILE`), so on Windows without `HOME` set it falls back to the current directory.

`UI::initConsole()` must be called at startup on Windows to enable ANSI escape code processing (`ENABLE_VIRTUAL_TERMINAL_PROCESSING`).
