cmake_minimum_required(VERSION 3.14)
project(cpp-taskflow VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

include_directories(include)

# ── Main binary ───────────────────────────────────────────────────────────
add_executable(task src/main.cpp)

# ── Tests ─────────────────────────────────────────────────────────────────
add_executable(task_tests tests/test_task.cpp)

enable_testing()
add_test(NAME TaskTests COMMAND task_tests)

# ── Install ───────────────────────────────────────────────────────────────
install(TARGETS task DESTINATION bin)
