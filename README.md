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
   ./diet_manager
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


## Key Features and How to Use Them

### 1. Food Management
- **View Basic Foods** (Option 1 → 1)
  - Shows all basic food items with nutritional information
  - Each food has calories, protein, carbs, fat, vitamins, etc.

- **View Composite Foods** (Option 1 → 2)
  - Displays all composite foods and their components
  - Shows servings of each component

- **Add Basic Food** (Option 1 → 3)
  - Enter name, keywords (comma-separated)
  - Input nutritional values:
    - Calories, protein, carbs, fat, saturated fat, fiber
    - Vitamins and minerals (comma-separated)

- **Add Composite Food** (Option 1 → 4)
  - Enter name and keywords
  - Add components using their IDs (shown in View Basic/Composite Foods)
  - Specify servings for each component

### 2. Daily Log Management
- **Log Food Consumption** (Option 2 → 1)
  - Enter date (YYYY-MM-DD format)
  - Search foods by keywords
  - Choose whether to match all or any keywords
  - Select food from search results
  - Enter number of servings

- **View Daily Log** (Option 2 → 2)
  - Shows all logged food entries with dates and servings

- **View Log by Date** (Option 2 → 3)
  - Enter specific date to view entries
  - Option to update servings for any entry

- **Delete Log Entry** (Option 2 → 4)
  - Select date and entry to delete
  - Confirm deletion

- **Undo Last Action** (Option 2 → 5)
  - Reverts the most recent change to the log

### 3. Diet Profile Management
- **View Diet Profile** (Option 3 → 1)
  - Shows current profile settings:
    - Name, gender, height, weight, age
    - Activity level and calculation method

- **Edit Diet Profile** (Option 3 → 2)
  - Modify any profile parameter:
    - Personal details
    - Activity level (1.2 to 1.9)

- **Change Calorie Calculation Method** (Option 3 → 3)
  - Choose between:
    1. Mifflin-St Jeor
    2. Harris-Benedict

- **View Calorie Summary** (Option 3 → 4)
  - Enter date to see:
    - Target calorie intake
    - Actual calories consumed
    - Difference from target

### Example Workflow
1. Set up your profile (Option 3 → 2)
2. Add some basic foods (Option 1 → 3)
3. Create composite foods (Option 1 → 4)
4. Log your daily food intake (Option 2 → 1)
5. Check your calorie summary (Option 3 → 4)

## Data Files
- `data/basic_foods.txt`: Pre-loaded basic foods
- `data/composite_foods.txt`: Saved composite foods
- `data/profile_info.txt`: User profile data
- `data/daily_log.txt`: Food consumption log

## Tips
- Use keywords effectively for easy food searching
- Keep track of food IDs for creating composite foods
- Regular backups of data files are recommended
- Use the undo feature if you make a mistake in logging