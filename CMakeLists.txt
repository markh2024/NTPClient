# Specify the minimum version of CMake
cmake_minimum_required(VERSION 3.10)

# Project name and version
project(NTPClientApp VERSION 1.0)

# Set the C++ standard to C++11 (you can change this to C++17 or later if needed)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the executable
add_executable(NTPClientApp
    source/main.cpp               # Main program file
    source/NTPClient.cpp      # Source file for NTPClient class
)

# Specify the directories for the header files
target_include_directories(NTPClientApp PUBLIC
    ${PROJECT_SOURCE_DIR}/headers
)

# Find the required system libraries (on Linux)
find_package(Threads REQUIRED)
target_link_libraries(NTPClientApp PRIVATE Threads::Threads)
