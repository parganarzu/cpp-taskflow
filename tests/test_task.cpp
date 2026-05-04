#include <iostream>
#include <cassert>
#include <cstdlib>
#include <fstream>
#include "task.h"

int passed = 0, failed = 0;

#define TEST(name, expr) \
    do { \
        if (expr) { std::cout << "  [PASS] " name "\n"; ++passed; } \
        else      { std::cout << "  [FAIL] " name "\n"; ++failed; } \
    } while(0)

// Use a temp file for tests
const std::string TMP = "/tmp/taskflow_test.dat";

void cleanup() {
    std::remove(TMP.c_str());
}

void testAdd() {
    std::cout << "\n── Add ──────────────────────────────\n";
    cleanup();
    TaskManager mgr(TMP);

    int id1 = mgr.add("Buy milk");
    int id2 = mgr.add("Write tests");

    TEST("first task id is 1",     id1 == 1);
    TEST("second task id is 2",    id2 == 2);
    TEST("total count is 2",       mgr.totalCount() == 2);
    TEST("pending count is 2",     mgr.pendingCount() == 2);
    TEST("done count is 0",        mgr.doneCount() == 0);
    TEST("title stored correctly", mgr.all()[0].title == "Buy milk");
}

void testDone() {
    std::cout << "\n── Done / Undone ────────────────────\n";
    cleanup();
    TaskManager mgr(TMP);
    mgr.add("Task A");
    mgr.add("Task B");

    TEST("mark done returns true",  mgr.markDone(1));
    TEST("done count is 1",         mgr.doneCount() == 1);
    TEST("pending count is 1",      mgr.pendingCount() == 1);
    TEST("mark invalid id false",   !mgr.markDone(99));

    TEST("mark undone returns true", mgr.markUndone(1));
    TEST("pending count back to 2",  mgr.pendingCount() == 2);
}

void testRemove() {
    std::cout << "\n── Remove ───────────────────────────\n";
    cleanup();
    TaskManager mgr(TMP);
    mgr.add("Task A");
    mgr.add("Task B");
    mgr.add("Task C");

    TEST("remove existing returns true",  mgr.remove(2));
    TEST("total count is 2",              mgr.totalCount() == 2);
    TEST("remove non-existing is false",  !mgr.remove(99));
    TEST("total count still 2",           mgr.totalCount() == 2);
}

void testClear() {
    std::cout << "\n── Clear ────────────────────────────\n";
    cleanup();
    TaskManager mgr(TMP);
    mgr.add("A"); mgr.add("B"); mgr.add("C");
    mgr.markDone(1);

    mgr.clear(/*onlyDone=*/true);
    TEST("clear done: removes 1", mgr.totalCount() == 2);
    TEST("clear done: no done left", mgr.doneCount() == 0);

    mgr.clear(false);
    TEST("clear all: count is 0", mgr.totalCount() == 0);
}

void testPersistence() {
    std::cout << "\n── Persistence ──────────────────────\n";
    cleanup();
    {
        TaskManager mgr(TMP);
        mgr.add("Persist me");
        mgr.markDone(1);
    }
    // New instance reads from same file
    TaskManager mgr2(TMP);
    TEST("task survives reload",      mgr2.totalCount() == 1);
    TEST("done status survives reload", mgr2.all()[0].done == true);
    TEST("title survives reload",     mgr2.all()[0].title == "Persist me");
}

void testIds() {
    std::cout << "\n── ID monotonicity ──────────────────\n";
    cleanup();
    TaskManager mgr(TMP);
    mgr.add("A"); mgr.add("B"); mgr.add("C");
    mgr.remove(2);
    int newId = mgr.add("D");
    TEST("new id after remove > max existing", newId == 4);
}

int main() {
    std::cout << "════════════════════════════════════\n";
    std::cout << "      cpp-taskflow Unit Tests\n";
    std::cout << "════════════════════════════════════";

    testAdd();
    testDone();
    testRemove();
    testClear();
    testPersistence();
    testIds();

    cleanup();

    std::cout << "\n════════════════════════════════════\n";
    std::cout << "Results: " << passed << " passed, " << failed << " failed\n";
    std::cout << "════════════════════════════════════\n";

    return failed == 0 ? 0 : 1;
}
