# C++ Deadlock Demonstration and Solution

This project provides a clear, practical example of a common multithreading issue—a **deadlock**—and demonstrates how to fix it. It includes two C++ programs: one that intentionally creates a deadlock and another that shows two effective solutions.

## Overview

In concurrent programming, a deadlock is a state in which two or more threads are blocked forever, each waiting for the other to release a resource that it needs. This project visualizes this problem using two mutexes (`mutexA` and `mutexB`) and two threads.

-   `deadlock.cpp`: This program demonstrates a classic deadlock scenario.
    -   `task1` locks `mutexA` and then tries to lock `mutexB`.
    -   `task2` locks `mutexB` and then tries to lock `mutexA`.
    -   This creates a circular dependency where neither thread can proceed. A watchdog timer is included to detect the deadlock and terminate the program.
-   `fix.cpp`: This program shows two common ways to prevent the deadlock.
    1.  **Consistent Lock Ordering**: Both tasks are modified to lock the mutexes in the same global order (`mutexA` then `mutexB`). This breaks the circular wait condition, preventing the deadlock.
    2.  **`std::scoped_lock` (C++17)**: A modern and safer approach where `std::scoped_lock` is used to acquire locks on both `mutexA` and `mutexB` simultaneously in a single, deadlock-aware operation.

## Building the Project

The project uses CMake to manage the build process. You will need to have CMake and a C++ compiler (that supports C++17) installed.

1.  **Create a build directory:**
    It's good practice to build the project out-of-source.

    ```bash
    mkdir build
    cd build
    ```

2.  **Run CMake to configure the project:**
    This command will find your compiler and generate the necessary build files (e.g., Makefiles).

    ```bash
    cmake ..
    ```

3.  **Compile the code:**
    This will build two executables: `deadlock` and `fix`.

    ```bash
    make
    # or cmake --build .
    ```

## Running the Executables

After building, you can run the two programs from the `build` directory.

### 1. Running the Deadlock Example

This program will hang and will be terminated by its internal watchdog after 3 seconds.

```bash
./deadlock

Expected Output:

Starting program that will demonstrate a deadlock.
Task 1 trying to lock mutexA...
Task 1 locked mutexA.
Task 2 trying to lock mutexB...
Task 2 locked mutexB.
Task 1 trying to lock mutexB... (will deadlock here)
Task 2 trying to lock mutexA... (will deadlock here)

DEADLOCK DETECTED! Terminating program.
Aborted (core dumped)

2. Running the Fixed Example
This program will run to completion without any issues, demonstrating that the deadlock has been resolved.

./fix

Expected Output:

--- Running fixed tasks with consistent lock ordering ---
Task 1 trying to lock mutexA...
Task 1 locked mutexA.
Task 1 trying to lock mutexB...
Task 1 locked mutexB.
Task 1 finished.
Task 2 trying to lock mutexA...
Task 2 locked mutexA.
Task 2 trying to lock mutexB...
Task 2 locked mutexB.
Task 2 finished.

--- Running tasks with C++17 std::scoped_lock ---
Modern task trying to lock both mutexes...
Modern task acquired both locks and finished.
Modern task trying to lock both mutexes...
Modern task acquired both locks and finished.

Program completed without deadlock.
