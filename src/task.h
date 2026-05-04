#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <ctime>
#include <algorithm>

struct Task {
    int         id;
    std::string title;
    bool        done;
    std::string created_at;

    static std::string now() {
        std::time_t t = std::time(nullptr);
        char buf[20];
        std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M", std::localtime(&t));
        return buf;
    }
};

// ── Simple flat-file storage (one task per line, pipe-delimited) ──────────
// Format:  id|done|created_at|title
class TaskStore {
public:
    explicit TaskStore(const std::string& path) : path_(path) {}

    std::vector<Task> load() const {
        std::vector<Task> tasks;
        std::ifstream f(path_);
        if (!f.is_open()) return tasks;   // first run — no file yet

        std::string line;
        while (std::getline(f, line)) {
            if (line.empty()) continue;
            auto t = parseLine(line);
            if (t.id > 0) tasks.push_back(t);
        }
        return tasks;
    }

    void save(const std::vector<Task>& tasks) const {
        std::ofstream f(path_, std::ios::trunc);
        if (!f.is_open())
            throw std::runtime_error("Cannot write to: " + path_);
        for (const auto& t : tasks)
            f << t.id << "|" << (t.done ? 1 : 0) << "|"
              << t.created_at << "|" << t.title << "\n";
    }

private:
    std::string path_;

    static Task parseLine(const std::string& line) {
        Task t{};
        std::istringstream ss(line);
        std::string tok;

        if (!std::getline(ss, tok, '|')) return t;
        t.id = std::stoi(tok);
        if (!std::getline(ss, tok, '|')) return t;
        t.done = tok == "1";
        if (!std::getline(ss, tok, '|')) return t;
        t.created_at = tok;
        if (!std::getline(ss, t.title))  return t;
        return t;
    }
};

// ── Task manager — business logic ─────────────────────────────────────────
class TaskManager {
public:
    explicit TaskManager(const std::string& storePath)
        : store_(storePath), tasks_(store_.load()) {}

    // Returns new task id
    int add(const std::string& title) {
        int id = nextId();
        tasks_.push_back({id, title, false, Task::now()});
        store_.save(tasks_);
        return id;
    }

    bool markDone(int id) {
        auto* t = find(id);
        if (!t) return false;
        t->done = true;
        store_.save(tasks_);
        return true;
    }

    bool markUndone(int id) {
        auto* t = find(id);
        if (!t) return false;
        t->done = false;
        store_.save(tasks_);
        return true;
    }

    bool remove(int id) {
        auto it = std::find_if(tasks_.begin(), tasks_.end(),
                               [id](const Task& t){ return t.id == id; });
        if (it == tasks_.end()) return false;
        tasks_.erase(it);
        store_.save(tasks_);
        return true;
    }

    void clear(bool onlyDone = false) {
        if (onlyDone)
            tasks_.erase(std::remove_if(tasks_.begin(), tasks_.end(),
                         [](const Task& t){ return t.done; }), tasks_.end());
        else
            tasks_.clear();
        store_.save(tasks_);
    }

    const std::vector<Task>& all()    const { return tasks_; }

    std::vector<Task> pending() const {
        std::vector<Task> out;
        for (auto& t : tasks_) if (!t.done) out.push_back(t);
        return out;
    }

    std::vector<Task> done() const {
        std::vector<Task> out;
        for (auto& t : tasks_) if (t.done) out.push_back(t);
        return out;
    }

    int totalCount()   const { return (int)tasks_.size(); }
    int pendingCount() const { return (int)pending().size(); }
    int doneCount()    const { return (int)done().size(); }

private:
    TaskStore        store_;
    std::vector<Task> tasks_;

    int nextId() const {
        int maxId = 0;
        for (auto& t : tasks_) maxId = std::max(maxId, t.id);
        return maxId + 1;
    }

    Task* find(int id) {
        for (auto& t : tasks_) if (t.id == id) return &t;
        return nullptr;
    }
};
