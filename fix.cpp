#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <atomic>

// Two mutexes that will be used to demonstrate a deadlock.
std::mutex mutexA;
std::mutex mutexB;

// An atomic boolean to signal completion. The watchdog uses this.
std::atomic<bool> done{false};

/**
 * @brief Task 1 locks mutexA, then tries to lock mutexB.
 */
void task1() {
    std::cout << "Task 1 trying to lock mutexA...\n";
    std::lock_guard<std::mutex> lockA(mutexA);
    std::cout << "Task 1 locked mutexA.\n";

    // Sleep to give task2 a chance to lock mutexB.
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    std::cout << "Task 1 trying to lock mutexB... (will deadlock here)\n";
    std::lock_guard<std::mutex> lockB(mutexB); // DEADLOCK! Waits for task2 to release mutexB.

    std::cout << "Task 1 finished\n";
}

/**
 * @brief Task 2 locks mutexB, then tries to lock mutexA.
 */
void task2() {
    std::cout << "Task 2 trying to lock mutexB...\n";
    std::lock_guard<std::mutex> lockB(mutexB);
    std::cout << "Task 2 locked mutexB.\n";

    // Sleep to give task1 a chance to lock mutexA.
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    std::cout << "Task 2 trying to lock mutexA... (will deadlock here)\n";
    std::lock_guard<std::mutex> lockA(mutexA); // DEADLOCK! Waits for task1 to release mutexA.

    std::cout << "Task 2 finished\n";
}

/**
 * @brief A simple watchdog timer to detect the deadlock.
 * If the tasks don't complete within 3 seconds, it terminates the program.
 */
void watchdog() {
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(3s); // Wait for 3 seconds.

    // If 'done' is still false, the tasks are stuck.
    if (!done.load()) {
        std::cerr << "\nDEADLOCK DETECTED! Terminating program.\n";
        std::terminate(); // Forcefully exit.
    }
}

int main() {
    std::cout << "Starting program that will demonstrate a deadlock.\n";
    std::thread t1(task1);
    std::thread t2(task2);
    std::thread w(watchdog);

    // The main thread will also block here, waiting for t1 and t2 to finish,
    // which they never will.
    t1.join();
    t2.join();

    done.store(true); // This line will never be reached.
    w.join();

    std::cout << "Program completed without deadlock\n";
    return 0;
}
