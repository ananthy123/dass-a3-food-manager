cmake_minimum_required(VERSION 3.10)
project(diet_manager)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include all source files
file(GLOB SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/CLI/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Food/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Database/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/User/*.cpp"
)

# Add executable
add_executable(diet_manager src/main.cpp ${SOURCES})

# Include directories
target_include_directories(diet_manager PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")

# Set output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
