#include <iostream>
#include <string>
#include <cstdlib>
#include "task.h"
#include "ui.h"

// Storage file: ~/.taskflow  (or override with TASKFLOW_FILE env var)
static std::string storageFile() {
    const char* env = std::getenv("TASKFLOW_FILE");
    if (env) return env;
    const char* home = std::getenv("HOME");
    return std::string(home ? home : ".") + "/.taskflow";
}

int main(int argc, char* argv[]) {
    UI::initConsole();
    if (argc < 2) { UI::help(); return 0; }

    TaskManager mgr(storageFile());
    std::string cmd = argv[1];

    // ── add <title> ───────────────────────────────────────────────────────
    if (cmd == "add") {
        if (argc < 3) { UI::err("Usage: task add \"<title>\""); return 1; }
        std::string title;
        for (int i = 2; i < argc; ++i) {
            if (i > 2) title += ' ';
            title += argv[i];
        }
        int id = mgr.add(title);
        UI::ok("Added task [" + std::to_string(id) + "]: " + title);
    }

    // ── list ─────────────────────────────────────────────────────────────
    else if (cmd == "list" || cmd == "ls") {
        std::string filter = (argc >= 3) ? argv[2] : "all";
        UI::printList(mgr.all(), filter);
    }

    // ── done <id> ─────────────────────────────────────────────────────────
    else if (cmd == "done") {
        if (argc < 3) { UI::err("Usage: task done <id>"); return 1; }
        int id = std::stoi(argv[2]);
        if (mgr.markDone(id))
            UI::ok("Marked [" + std::to_string(id) + "] as done ✓");
        else
            UI::err("Task [" + std::to_string(id) + "] not found.");
    }

    // ── undone <id> ───────────────────────────────────────────────────────
    else if (cmd == "undone") {
        if (argc < 3) { UI::err("Usage: task undone <id>"); return 1; }
        int id = std::stoi(argv[2]);
        if (mgr.markUndone(id))
            UI::ok("Marked [" + std::to_string(id) + "] as pending ○");
        else
            UI::err("Task [" + std::to_string(id) + "] not found.");
    }

    // ── remove <id> ───────────────────────────────────────────────────────
    else if (cmd == "remove" || cmd == "rm") {
        if (argc < 3) { UI::err("Usage: task remove <id>"); return 1; }
        int id = std::stoi(argv[2]);
        if (mgr.remove(id))
            UI::ok("Removed task [" + std::to_string(id) + "].");
        else
            UI::err("Task [" + std::to_string(id) + "] not found.");
    }

    // ── clear [done] ──────────────────────────────────────────────────────
    else if (cmd == "clear") {
        bool onlyDone = (argc >= 3 && std::string(argv[2]) == "done");
        mgr.clear(onlyDone);
        UI::ok(onlyDone ? "Cleared all completed tasks." : "Cleared all tasks.");
    }

    // ── stats ─────────────────────────────────────────────────────────────
    else if (cmd == "stats") {
        UI::stats(mgr);
    }

    // ── help ──────────────────────────────────────────────────────────────
    else if (cmd == "help" || cmd == "--help" || cmd == "-h") {
        UI::help();
    }

    else {
        UI::err("Unknown command: " + cmd);
        UI::help();
        return 1;
    }

    return 0;
}