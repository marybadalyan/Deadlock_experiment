#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <atomic>

std::mutex mutexA;
std::mutex mutexB;

std::atomic<bool> done{false};

void task1() {
    std::lock_guard<std::mutex> lockA(mutexA);
    std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Let task2 lock mutexB
    std::lock_guard<std::mutex> lockB(mutexB);  // Deadlock here
    std::cout << "Task1 finished\n";
}

void task2() {
    std::lock_guard<std::mutex> lockB(mutexB);
    std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Let task1 lock mutexA
    std::lock_guard<std::mutex> lockA(mutexA);  // Deadlock here
    std::cout << "Task2 finished\n";
}

void watchdog() {
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(3s);  // Wait for 3 seconds

    if (!done.load()) {
        std::cerr << "Deadlock detected! Terminating program.\n";
        std::terminate();  // or std::exit(EXIT_FAILURE);
    }
}

int main() {
    std::thread t1(task1);
    std::thread t2(task2);
    std::thread w(watchdog);

    t1.join();
    t2.join();
    done.store(true);  // Mark work done
    w.join();

    std::cout << "Program completed without deadlock\n";
    return 0;
}
