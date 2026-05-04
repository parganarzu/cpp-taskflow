#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "task.h"
#include "color.h"

#ifdef _WIN32
#include <windows.h>
#endif

namespace UI {

inline void initConsole() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;
    if (GetConsoleMode(hOut, &mode))
        SetConsoleMode(hOut, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
#endif
}

inline void banner() {
    std::cout << Color::BOLD << Color::CYAN
              << "+================================+\n"
              << "|      cpp-taskflow  [v1.0]      |\n"
              << "+================================+\n"
              << Color::RESET;
}

inline void help() {
    banner();
    std::cout << "\n" << Color::bold("Usage:") << "  task <command> [args]\n\n"
              << Color::bold("Commands:\n")
              << "  " << Color::cyan("add")    << " <title>   Add a new task\n"
              << "  " << Color::cyan("list")   << "           List all tasks\n"
              << "  " << Color::cyan("done")   << " <id>      Mark task as done\n"
              << "  " << Color::cyan("undone") << " <id>      Mark task as pending\n"
              << "  " << Color::cyan("remove") << " <id>      Remove a task\n"
              << "  " << Color::cyan("clear")  << "           Remove all tasks\n"
              << "  " << Color::cyan("clear")  << " done      Remove only completed tasks\n"
              << "  " << Color::cyan("stats")  << "           Show statistics\n"
              << "  " << Color::cyan("help")   << "           Show this help\n\n"
              << Color::dim("Example:\n")
              << "  task add \"Finish the README\"\n"
              << "  task list\n"
              << "  task done 1\n\n";
}

inline void printTask(const Task& t) {
    std::string icon  = t.done ? Color::green(u8"✓") : Color::yellow(u8"○");
    std::string id    = Color::dim("[" + std::to_string(t.id) + "]");
    std::string title = t.done
                        ? Color::dim(t.title)
                        : Color::WHITE + t.title + Color::RESET;
    std::string date  = Color::dim(t.created_at);

    std::cout << "  " << icon << "  " << id << "  " << title
              << "  " << date << "\n";
}

inline void printList(const std::vector<Task>& tasks, const std::string& filter = "all") {
    if (tasks.empty()) {
        std::cout << Color::dim("  No tasks found. Add one with: ")
                  << Color::cyan("task add \"Your task\"") << "\n";
        return;
    }

    int pending = 0, done = 0;
    for (auto& t : tasks) { t.done ? ++done : ++pending; }

    std::cout << "\n";
    for (auto& t : tasks) {
        if (filter == "pending" &&  t.done) continue;
        if (filter == "done"    && !t.done) continue;
        printTask(t);
    }

    std::cout << "\n  "
              << Color::dim("------------------------------\n")
              << "  "
              << Color::green(std::to_string(done) + " done")
              << "  "
              << Color::yellow(std::to_string(pending) + " pending")
              << "  "
              << Color::dim(std::to_string(tasks.size()) + " total")
              << "\n\n";
}

inline void stats(const TaskManager& mgr) {
    int total   = mgr.totalCount();
    int done    = mgr.doneCount();
    int pending = mgr.pendingCount();
    int pct     = total > 0 ? (done * 100 / total) : 0;

    int filled = total > 0 ? (done * 20 / total) : 0;
    std::string bar = Color::GREEN;
    for (int i = 0; i < 20; ++i)
        bar += (i < filled) ? u8"█" : (Color::RESET + Color::DIM + u8"░");
    bar += Color::RESET;

    std::cout << "\n  " << Color::bold("Progress:") << "  [" << bar << "]"
              << "  " << Color::bold(std::to_string(pct) + "%") << "\n\n"
              << "  " << Color::green(u8"✓ Done     : ") << done    << "\n"
              << "  " << Color::yellow(u8"○ Pending  : ") << pending << "\n"
              << "  " << Color::cyan( u8"  Total    : ") << total   << "\n\n";
}

inline void ok(const std::string& msg)  {
    std::cout << "  " << Color::green(u8"✓") << "  " << msg << "\n";
}
inline void err(const std::string& msg) {
    std::cerr << "  " << Color::red(u8"✗") << "  " << Color::red(msg) << "\n";
}

} // namespace UI