#include "CLIManager.h"
#include "../Food/BasicFood.h"
#include "../Food/CompositeFood.h"
#include <iostream>
#include <sstream>
#include <limits>
#include <fstream>

using namespace std;

const std::string BASIC_FOODS_FILE = "../data/basic_foods.txt";
const std::string COMPOSITE_FOODS_FILE = "../data/composite_foods.txt";
const std::string DAILY_LOG_FILE = "../data/daily_log.txt";

bool validateBasicFoodsFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "❌ File not found: " << filename << std::endl;
        return false;
    }

    std::string line;
    int lineNumber = 0;
    bool hasError = false;

    while (std::getline(file, line)) {
        lineNumber++;
        if (line.empty() || line[0] == '#') continue;

        std::stringstream ss(line);
        std::string fields[11];
        for (int i = 0; i < 11; ++i) {
            if (!std::getline(ss, fields[i], ';')) {
                std::cerr << "❌ Line " << lineNumber << ": Missing field " << i+1 << "\n";
                hasError = true;
                break;
            }
        }

        for (int i = 3; i <= 8; ++i) {
            try {
                std::stod(fields[i]);
            } catch (...) {
                std::cerr << "❌ Line " << lineNumber << ": Invalid number in field " << i+1
                          << " → " << fields[i] << "\n";
                hasError = true;
            }
        }
    }

    if (!hasError) {
        std::cout << "✅ basic_foods.txt passed validation.\n";
    } else {
        std::cerr << "⚠️ Validation errors found. Please fix them before continuing.\n";
    }

    return !hasError;
}

CLIManager::CLIManager()
    : db(BASIC_FOODS_FILE, COMPOSITE_FOODS_FILE),
      log(DAILY_LOG_FILE),
      profile("Ananth", "male", 180, 25, 75, 1.55),
      autoSaveEnabled(false) { // Initialize to false by default

    // Validate file before loading
    if (!validateBasicFoodsFile(BASIC_FOODS_FILE)) {
        std::cerr << "🚫 Invalid format in basic_foods.txt. Fix it and rerun.\n";
        exit(1); // 🔁 STOP program from continuing
    }
    
    db.loadDatabase(); // ✅ only called if validation passed
    log.loadLog();
    
    // Prompt for save preference
    // promptSavePreference();
}

void CLIManager::showMainMenu() {
    std::cout << "\n=== YADA: Yet Another Diet Assistant ===\n";
    std::cout << "1. Food Management\n";
    std::cout << "2. Daily Log\n";
    std::cout << "3. Diet Profile\n";
    std::cout << "4. Save Database\n";
    std::cout << "5. Exit\n";
    std::cout << "Choose an option: ";
}

void CLIManager::showFoodMenu() {
    std::cout << "\n=== Food Management ===\n";
    std::cout << "1. View Basic Foods\n";
    std::cout << "2. View Composite Foods\n";
    std::cout << "3. Add Basic Food\n";
    std::cout << "4. Add Composite Food\n";
    std::cout << "5. Add Component to Composite Food\n";
    std::cout << "6. Back to Main Menu\n";
    std::cout << "Choose an option: ";
}

void CLIManager::showLogMenu() {
    std::cout << "\n=== Daily Log ===\n";
    std::cout << "1. Log Food Consumption\n";
    std::cout << "2. View Daily Log\n";
    std::cout << "3. Back to Main Menu\n";
    std::cout << "Choose an option: ";
}

void CLIManager::showProfileMenu() {
    std::cout << "\n=== Diet Profile ===\n";
    std::cout << "1. View Diet Profile\n";
    std::cout << "2. Change Calorie Calculation Method\n";
    std::cout << "3. Back to Main Menu\n";
    std::cout << "Choose an option: ";
}

// void CLIManager::showMenu() {
//     std::cout << "\n=== YADA: Yet Another Diet Assistant ===\n";
//     std::cout << "1. View Basic Foods\n";
//     std::cout << "2. View Composite Foods\n";
//     std::cout << "3. Add Basic Food\n";
//     std::cout << "4. Add Composite Food\n";
//     std::cout << "5. Add Component to Composite Food\n"; // Add this line
//     std::cout << "6. Log Food Consumption\n";
//     std::cout << "7. View Daily Log\n";
//     std::cout << "8. View Diet Profile\n"; // Update the number
//     std::cout << "9. Save Database\n";
//     std::cout << "0. Exit\n";
//     std::cout << "Choose an option: ";
// }

void CLIManager::start() {
    int choice;
    do {
        showMainMenu();
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice) {
            case 1: {
                // Food Management Sub-Menu
                int foodChoice;
                do {
                    showFoodMenu();
                    std::cin >> foodChoice;
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    
                    switch (foodChoice) {
                        case 1: handleViewBasicFoods(); break;
                        case 2: handleViewCompositeFoods(); break;
                        case 3: handleAddBasicFood(); break;
                        case 4: handleAddCompositeFood(); break;
                        case 5: handleAddComponentToCompositeFood(); break;
                        case 6: break; // Back to main menu
                        default: std::cout << "Invalid option.\n"; pause(); break;
                    }
                } while (foodChoice != 6);
                break;
            }
            
            case 2: {
                // Daily Log Sub-Menu
                int logChoice;
                do {
                    showLogMenu();
                    std::cin >> logChoice;
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    
                    switch (logChoice) {
                        case 1: handleAddLogEntry(); break;
                        case 2: handleViewLog(); break;
                        case 3: break; // Back to main menu
                        default: std::cout << "Invalid option.\n"; pause(); break;
                    }
                } while (logChoice != 3);
                break;
            }
            
            case 3: {
                // Diet Profile Sub-Menu
                int profileChoice;
                do {
                    showProfileMenu();
                    std::cin >> profileChoice;
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    
                    switch (profileChoice) {
                        case 1: handleViewProfile(); break;
                        case 2: handleCalorieMethodChange(); break;
                        case 3: break; // Back to main menu
                        default: std::cout << "Invalid option.\n"; pause(); break;
                    }
                } while (profileChoice != 3);
                break;
            }
            
            case 4: handleSaveDatabase(); break;
            case 5: break; // Exit
            default: std::cout << "Invalid option.\n"; pause(); break;
        }
    } while (choice != 5);

    // Always save on exit
    db.saveDatabase();
    log.saveLog();
    std::cout << "Data saved. Goodbye!\n";
}

// void CLIManager::start() {
//     int choice;
//     do {
//         showMenu();
//         std::cin >> choice;
//         std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

//         switch (choice) {
//             case 1: handleViewBasicFoods(); break;
//             case 2: handleViewCompositeFoods(); break;
//             case 3: handleAddBasicFood(); break;
//             case 4: handleAddCompositeFood(); break;
//             case 5: handleAddComponentToCompositeFood(); break; // Add this line
//             case 6: handleAddLogEntry(); break;
//             case 7: handleViewLog(); break;
//             case 8: handleViewProfile(); break; // Update the number
//             case 9: handleSaveDatabase(); break;
//             case 0: break;
//             default: std::cout << "Invalid option.\n"; pause(); break;
//         }
//     } while (choice != 0);

//     db.saveDatabase();
//     log.saveLog();
//     std::cout << "Data saved. Goodbye!\n";
// }

void CLIManager::handleViewBasicFoods() {
    std::cout << "\n--- Basic Foods ---\n";
    for (const auto& food : db.getBasicFoods()) {
        food->display();
    }
    pause();
};

void CLIManager::handleViewCompositeFoods() {
    std::cout << "\n--- Composite Foods ---\n";
    for (const auto& food : db.getCompositeFoods()) {
        food->display();
    }
    pause();
}

void CLIManager::handleAddBasicFood() {
    std::string name;
    std::cout << "Enter food name: ";
    std::getline(std::cin, name);

    auto keywords = getKeywordsInput();

    double calories, protein, carbs, fat, satFat, fiber;
    std::string vitamins, minerals;

    std::cout << "Calories: "; std::cin >> calories;
    std::cout << "Protein (g): "; std::cin >> protein;
    std::cout << "Carbs (g): "; std::cin >> carbs;
    std::cout << "Fat (g): "; std::cin >> fat;
    std::cout << "Saturated Fat (g): "; std::cin >> satFat;
    std::cout << "Fiber (g): "; std::cin >> fiber;
    std::cin.ignore();

    std::cout << "Vitamins (comma-separated): ";
    std::getline(std::cin, vitamins);
    std::cout << "Minerals (comma-separated): ";
    std::getline(std::cin, minerals);

    std::string id = db.generateBasicFoodId();

    auto food = std::make_shared<BasicFood>(
        id, name, keywords, calories, protein, carbs, fat, satFat, fiber, vitamins, minerals
    );
    db.addBasicFood(food);

    std::cout << "Basic food added with ID: " << id << "\n";
    
    // Add this line to save based on preference
    saveIfAutoSave();
    
    pause();
}

void CLIManager::handleAddCompositeFood() {
    std::string name;
    std::cout << "Enter composite food name: ";
    std::getline(std::cin, name);

    auto keywords = getKeywordsInput();
    std::string id = db.generateCompositeFoodId();

    auto comp = std::make_shared<CompositeFood>(id, name, keywords);

    std::string compId;
    while (true) {
        std::cout << "Enter component food ID (or 'done'): ";
        std::getline(std::cin, compId);
        if (compId == "done") break;

        auto food = db.findFoodById(compId);
        if (!food) {
            std::cout << "Food ID not found.\n";
            continue;
        }

        double servings;
        std::cout << "Enter servings: ";
        std::cin >> servings;
        std::cin.ignore();

        comp->addComponent(food, servings);
    }

    db.addCompositeFood(comp);
    std::cout << "Composite food added with ID: " << id << "\n";
    
    // Add this line to save based on preference
    saveIfAutoSave();
    
    pause();
}

void CLIManager::handleAddLogEntry() {
    std::string date, id;
    double servings;

    std::cout << "Enter date (YYYY-MM-DD): ";
    std::getline(std::cin, date);
    std::cout << "Enter food ID: ";
    std::getline(std::cin, id);
    std::cout << "Servings: ";
    std::cin >> servings;
    std::cin.ignore();

    if (db.findFoodById(id)) {
        log.addEntry({date, id, servings});
        std::cout << "Log entry added.\n";
        
        // Also save the log if auto-save is enabled
        if (autoSaveEnabled) {
            log.saveLog();
            std::cout << "Log changes saved.\n";
        }
    } else {
        std::cout << "Food not found.\n";
    }
    pause();
}

void CLIManager::handleViewLog() {
    std::cout << "\n--- Daily Log ---\n";
    log.displayLog();
    pause();
}

void CLIManager::handleViewProfile() {
    profile.displayProfile();
    pause();
}

std::vector<std::string> CLIManager::getKeywordsInput() {
    std::string line;
    std::vector<std::string> keywords;

    std::cout << "Enter comma-separated keywords: ";
    std::getline(std::cin, line);
    std::stringstream ss(line);
    std::string keyword;
    while (std::getline(ss, keyword, ',')) {
        keywords.push_back(keyword);
    }

    return keywords;
}

// Add this function implementation
void CLIManager::handleAddComponentToCompositeFood() {
    // Helper function to trim whitespace - defined at the top for use throughout the function
    auto trim = [](const std::string& str) {
        size_t first = str.find_first_not_of(" \t");
        if (first == std::string::npos) return std::string();
        size_t last = str.find_last_not_of(" \t");
        return str.substr(first, last - first + 1);
    };

    // Display available composite foods
    std::cout << "\n--- Available Composite Foods ---\n";
    const auto& compositeFoods = db.getCompositeFoods();
    if (compositeFoods.empty()) {
        std::cout << "No composite foods available. Create a composite food first.\n";
        pause();
        return;
    }
    
    for (const auto& food : compositeFoods) {
        std::cout << "ID: " << food->getId() << ", Name: " << trim(food->getName()) << "\n";
    }
    
    // Ask if user wants to enter ID or name for the composite food
    std::string compositeInputChoice;
    std::cout << "\nDo you want to enter ID or name for the composite food? (id/name): ";
    std::getline(std::cin, compositeInputChoice);
    
    std::shared_ptr<Food> compositeFood = nullptr;
    
    if (compositeInputChoice == "id") {
        // Get composite food by ID
        std::string compositeId;
        std::cout << "Enter the ID of the composite food to modify: ";
        std::getline(std::cin, compositeId);
        
        compositeFood = db.findFoodById(compositeId);
    } else {
        // Get composite food by name
        std::string compositeName;
        std::cout << "Enter the name of the composite food to modify: ";
        std::getline(std::cin, compositeName);
        
        // Find food by name with trimming
        compositeName = trim(compositeName);
        for (const auto& food : compositeFoods) {
            if (trim(food->getName()) == compositeName) {
                compositeFood = food;
                break;
            }
        }
    }
    
    // Check if a valid composite food was found
    if (!compositeFood || compositeFood->getType() != "composite") {
        std::cout << "Invalid composite food selection.\n";
        pause();
        return;
    }
    
    std::shared_ptr<CompositeFood> compFood = std::dynamic_pointer_cast<CompositeFood>(compositeFood);
    
    // Display all available foods (both basic and composite)
    std::cout << "\n--- Available Foods to Add as Components ---\n";
    std::cout << "Basic foods:\n";
    const auto& basicFoods = db.getBasicFoods();
    for (const auto& food : basicFoods) {
        std::cout << "ID: " << food->getId() << ", Name: " << trim(food->getName()) << "\n";
    }
    
    std::cout << "\nComposite foods:\n";
    for (const auto& food : compositeFoods) {
        // Skip the composite food we're currently editing
        if (food->getId() != compFood->getId()) {
            std::cout << "ID: " << food->getId() << ", Name: " << trim(food->getName()) << "\n";
        }
    }
    
    // Ask if user wants to enter ID or name for the component
    std::string componentInputChoice;
    std::cout << "\nDo you want to enter ID or name for the component? (id/name): ";
    std::getline(std::cin, componentInputChoice);
    
    std::shared_ptr<Food> componentFood = nullptr;
    
    if (componentInputChoice == "id") {
        // Get component by ID
        std::string componentId;
        std::cout << "Enter the ID of the food to add as component: ";
        std::getline(std::cin, componentId);
        
        componentFood = db.findFoodById(componentId);
    } else {
        // Get component by name
        std::string componentName;
        std::cout << "Enter the name of the food to add as component: ";
        std::getline(std::cin, componentName);
        
        // Find food by name in both basic and composite foods - with improved matching
        componentFood = nullptr;
        
        // Trim the input name
        componentName = trim(componentName);
        
        // Search through basic foods with trimmed comparison
        for (const auto& food : basicFoods) {
            if (trim(food->getName()) == componentName) {
                componentFood = food;
                break;
            }
        }
        
        // If not found in basic foods, check composite foods
        if (!componentFood) {
            for (const auto& food : compositeFoods) {
                if (trim(food->getName()) == componentName && food->getId() != compFood->getId()) {
                    componentFood = food;
                    break;
                }
            }
        }
    } // FIXED: Proper closing brace for the else block
    
    // Check if component food was found
    if (!componentFood) {
        std::cout << "Food not found.\n";
        pause();
        return;
    }
    
    // Check for circular references
    if (componentFood->getId() == compFood->getId()) {
        std::cout << "Cannot add a composite food to itself.\n";
        pause();
        return;
    }
    
    // If component is a composite food, check for circular references
    if (componentFood->getType() == "composite") {
        // We need to check if adding this component would create a circular reference
        // This is complex without a helper function, so let's implement a simple version
        std::shared_ptr<CompositeFood> componentCompFood = 
            std::dynamic_pointer_cast<CompositeFood>(componentFood);
            
        // Simple check: see if the component contains the target composite food
        // We'd ideally want a recursive function here for deep checking
        for (const auto& pair : componentCompFood->getComponents()) {
            if (pair.first->getId() == compFood->getId()) {
                std::cout << "Cannot add this food as it would create a circular reference.\n";
                pause();
                return;
            }
        }
    }
    
    // Get quantity
    double quantity;
    std::cout << "Enter quantity: ";
    std::cin >> quantity;
    std::cin.ignore();
    
    if (quantity <= 0) {
        std::cout << "Quantity must be positive.\n";
        pause();
        return;
    }
    
    // Add the component
    compFood->addComponent(componentFood, quantity);
    
    // The database should be saved when exiting the program
    
    saveIfAutoSave(); // Save if auto-save is enabled

    std::cout << "Successfully added " << trim(componentFood->getName()) << " (ID: " << componentFood->getId() 
              << ") to " << trim(compFood->getName()) << "\n";
    pause();
}

void CLIManager::pause() {
    std::cout << "Press ENTER to continue...";
    std::cin.get();
}

// void CLIManager::promptSavePreference() {
//     char choice;
//     std::cout << "\n=== Database Save Preference ===\n";
//     std::cout << "Would you like to save changes immediately after each modification? (y/n): ";
//     std::cin >> choice;
//     std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
//     autoSaveEnabled = (choice == 'y' || choice == 'Y');
    
//     if (autoSaveEnabled) {
//         std::cout << "Auto-save enabled. Changes will be saved immediately.\n";
//     } else {
//         std::cout << "Auto-save disabled. Changes will be saved when the program exits.\n";
//     }
// }

void CLIManager::saveIfAutoSave() {
    if (autoSaveEnabled) {
        db.saveDatabase();
        std::cout << "Database changes saved.\n";
    }
}

void CLIManager::handleSaveDatabase() {
    db.saveDatabase();
    log.saveLog();
    std::cout << "Database and log successfully saved.\n";
    pause();
}

void CLIManager::handleCalorieMethodChange() {
    std::cout << "\n=== Calorie Calculation Methods ===\n";
    
    // Get all available methods from the DietProfile class
    auto availableMethods = DietProfile::getAvailableCalculationMethods();
    
    // Display all available methods
    for (const auto& method : availableMethods) {
        std::cout << method.first << ". " << method.second << "\n";
    }
    
    // Ask the user to select a method by ID
    std::cout << "\nSelect method ID: ";
    
    int methodChoice;
    std::cin >> methodChoice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    // Check if the chosen method exists in the available methods
    if (availableMethods.find(methodChoice) != availableMethods.end()) {
        profile.setCalculationMethod(methodChoice);
        std::cout << "Calorie calculation method changed to " 
                  << availableMethods[methodChoice] << ".\n";
    } else {
        std::cout << "Invalid method ID. No changes made.\n";
    }
    pause();
}