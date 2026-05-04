# ✅ cpp-taskflow

> A minimal, fast, and colorful **command-line task manager** written in C++17.  
> No dependencies. Single binary. Stores tasks in `~/.taskflow`.

```
$ task add "Build something cool"
  ✓  Added task [1]: Build something cool

$ task add "Write unit tests"
  ✓  Added task [2]: Write unit tests

$ task done 1
  ✓  Marked [1] as done ✓

$ task list

  ✓  [1]  Build something cool    2025-01-15 14:32
  ○  [2]  Write unit tests        2025-01-15 14:32

  ──────────────────────────────
  1 done  1 pending  2 total
```

## Features

- ➕ **Add, complete, remove** tasks from the terminal
- 🎨 **Color output** — status icons, dimmed completed tasks, progress bar
- 💾 **Persistent storage** — survives reboots, stored in `~/.taskflow`
- ⚡ **Zero dependencies** — pure C++17, no libraries needed
- 🧪 **Unit tested** — 20 tests covering all core logic
- 📦 **CMake build** with install target

## Installation

### Build from source

```bash
git clone https://github.com/YOUR_USERNAME/cpp-taskflow
cd cpp-taskflow
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

### Optional: install to `/usr/local/bin`

```bash
sudo cmake --install .
```

Then use `task` from anywhere.

## Usage

```bash
task add "Your task title"    # Add a new task
task list                     # List all tasks
task list done                # Show only completed
task list pending             # Show only pending
task done <id>                # Mark as done
task undone <id>              # Mark as pending again
task remove <id>              # Delete a task
task clear                    # Delete all tasks
task clear done               # Delete only completed tasks
task stats                    # Show progress bar + counts
task help                     # Show help
```

### Stats output

```
$ task stats

  Progress:  ████████████░░░░░░░░  60%

  ✓ Done     : 3
  ○ Pending  : 2
    Total    : 5
```

## Project Structure

```
cpp-taskflow/
├── include/
│   ├── task.h       ← Task model + file storage + TaskManager
│   ├── ui.h         ← Terminal output, colors, list printer
│   └── color.h      ← ANSI color helpers
├── src/
│   └── main.cpp     ← CLI argument parsing
├── tests/
│   └── test_task.cpp ← Unit tests (no external framework)
└── CMakeLists.txt
```

## Running Tests

```bash
cd build
./task_tests

# or via CTest:
ctest --output-on-failure
```

## Custom Storage Path

Override the default `~/.taskflow` file:

```bash
TASKFLOW_FILE=./myproject.tasks task list
```

Great for per-project task lists!

## License

MIT
