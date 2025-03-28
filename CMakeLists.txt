cmake_minimum_required(VERSION 3.10)
project(DietManager)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(src)

# Gather all .cpp files
file(GLOB_RECURSE SRC_FILES
    src/*.cpp
)

# Create the executable
add_executable(diet_manager ${SRC_FILES})
