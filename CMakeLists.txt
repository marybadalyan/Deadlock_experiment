# Specify the minimum version of CMake required.
cmake_minimum_required(VERSION 3.16)

# Set the project name and specify that it's a C++ project.
project(DeadLockExample CXX)

# Set the C++ standard to C++17 (your code uses features from it).
# This is the modern way to set the standard for a target.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# This command finds the system's thread library. It's necessary because
# you are using std::thread.
find_package(Threads REQUIRED)

# This is the key command. It tells CMake to create an executable named
# "main" from the specified source files.
# This replaces the need for #include "ThreadPool.cpp".
add_executable(fix
   fix.cpp
)

add_executable(deadlock
    deadlock.cpp
)
