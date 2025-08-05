#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

std::mutex mutexA;
std::mutex mutexB;

void task1() {
    std::lock(mutexA, mutexB); // lock both without deadlock
    std::lock_guard<std::mutex> lockA(mutexA, std::adopt_lock);
    std::lock_guard<std::mutex> lockB(mutexB, std::adopt_lock);
    std::cout << "Task1 finished\n";
}

void task2() {
    std::lock(mutexA, mutexB); // same locking order
    std::lock_guard<std::mutex> lockA(mutexA, std::adopt_lock);
    std::lock_guard<std::mutex> lockB(mutexB, std::adopt_lock);
    std::cout << "Task2 finished\n";
}

int main() {
    std::thread t1(task1);
    std::thread t2(task2);

    t1.join();
    t2.join();

    return 0;
}
