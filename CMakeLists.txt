cmake_minimum_required(VERSION 3.10)
project(DietManager)

# Use C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include all headers from the src folder
include_directories(${CMAKE_SOURCE_DIR}/src)

# Recursively find all .cpp files under src/
file(GLOB_RECURSE SRC_FILES
    src/*.cpp
)

# Define the executable
add_executable(diet_manager ${SRC_FILES})
