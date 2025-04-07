# Diet Assistant Program

## How to Build and Run the Program

Follow these steps to build and run the program:

1. **Clone the Repository**  
   Ensure you have cloned the repository to your local machine.

2. **Create a Build Directory**  
   Open a terminal in the project root directory and run:
   ```bash
   mkdir build
   cd build
   ```

3. **Generate Build Files with CMake**  
   Run the following command to generate the build files:
   ```bash
   cmake ..
   ```

4. **Build the Program**  
   Compile the program using:
   ```bash
   cmake --build .
   ```

5. **Run the Program**  
   After the build is complete, run the program using:
   ```bash
   ./dass-a3-food-manager
   ```

## Prerequisites

- **CMake**: Ensure CMake is installed on your system. You can download it from [cmake.org](https://cmake.org/).
- **C++ Compiler**: A C++ compiler that supports C++17 or later (e.g., GCC, Clang, or MSVC).

## Directory Structure

- `src/`: Contains the source code for the program.
- `data/`: Contains the input files like `basic_foods.txt`, `composite_foods.txt`, and `profile_info.txt`.
- `build/`: Directory where the build files will be generated.

## Notes

- Ensure the `data/` directory contains the required input files before running the program.
- If you encounter any issues, check the error messages and ensure all dependencies are installed.