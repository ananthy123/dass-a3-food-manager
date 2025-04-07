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
      profile("", "", 0, 0, 0, 0),
      autoSaveEnabled(false) { // Initialize with dummy values

    // Validate file before loading
    if (!validateBasicFoodsFile(BASIC_FOODS_FILE)) {
        std::cerr << "🚫 Invalid format in basic_foods.txt. Fix it and rerun.\n";
        exit(1);
    }

    db.loadDatabase();
    log.loadLog();

    // Load profile information from file
    try {
        profile.loadProfileFromFile("../data/profile_info.txt");
    } catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
        exit(1);
    }
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
    std::cout << "5. Edit Composite Food\n";
    std::cout << "6. Back to Main Menu\n";
    std::cout << "Choose an option: ";
}

void CLIManager::showLogMenu() {
    std::cout << "\n=== Daily Log ===\n";
    std::cout << "1. Log Food Consumption\n";
    std::cout << "2. View Daily Log\n";
    std::cout << "3. View Log by Date\n"; 
    std::cout << "4. Undo Last Action\n"; 
    std::cout << "5. Back to Main Menu\n";
    std::cout << "Choose an option: ";
}

void CLIManager::showProfileMenu() {
    std::cout << "\n=== Diet Profile ===\n";
    std::cout << "1. View Diet Profile\n";
    std::cout << "2. Edit Diet Profile\n";
    std::cout << "3. Change Calorie Calculation Method\n";
    std::cout << "4. View Calorie Summary\n";
    std::cout << "5. Back to Main Menu\n";
    std::cout << "Choose an option: ";
}

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
                        case 3: handleViewLogByDate(); break; // Handle viewing log by date
                        case 4: handleUndo(); break; // Handle undoing last action
                        case 5: break; // Back to main menu
                        default: std::cout << "Invalid option.\n"; pause(); break;
                    }
                } while (logChoice != 5);
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
                        case 2: handleEditProfile(); break;
                        case 3: handleCalorieMethodChange(); break;
                        case 4: handleCalorieSummary(); break; 
                        case 5: break; // Back to main menu
                        default: std::cout << "Invalid option.\n"; pause(); break;
                    }
                } while (profileChoice != 5);
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

    while (true) {
        std::cout << "Enter component food ID (or leave blank to finish): ";
        std::string compId;
        std::getline(std::cin, compId);

        if (compId.empty()) break;

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

    std::cout << "Search foods by keywords? (y/n): ";
    char searchChoice;
    std::cin >> searchChoice;
    std::cin.ignore();

    std::vector<std::shared_ptr<Food>> searchResults;
    if (searchChoice == 'y' || searchChoice == 'Y') {
        auto keywords = getKeywordsInput();
        std::cout << "Match (1) all or (2) any keywords? ";
        int matchType;
        std::cin >> matchType;
        std::cin.ignore();

        searchResults = db.searchFoodsByKeywords(keywords, matchType == 1);
    } else {
        searchResults = db.getAllFoods();
    }

    for (size_t i = 0; i < searchResults.size(); ++i) {
        std::cout << i + 1 << ". " << searchResults[i]->getName() << "\n";
    }

    std::cout << "Select food by number: ";
    int foodIndex;
    std::cin >> foodIndex;
    std::cin.ignore();

    if (foodIndex < 1 || foodIndex > searchResults.size()) {
        std::cout << "Invalid selection.\n";
        return;
    }

    auto selectedFood = searchResults[foodIndex - 1];

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

void CLIManager::handleViewLogByDate() {
    std::cout << "Enter date (YYYY-MM-DD): ";
    std::string date;
    std::cin >> date;

    auto entries = log.getEntriesByDate(date);
    if (entries.empty()) {
        std::cout << "No entries for this date.\n";
        return;
    }

    for (size_t i = 0; i < entries.size(); ++i) {
        std::cout << i + 1 << ". Food ID: " << entries[i].foodId
                  << ", Servings: " << entries[i].servings << "\n";
    }
}

void CLIManager::handleUndo() {
    log.undoLastAction();
    std::cout << "Last action undone.\n";
}

void CLIManager::handleViewProfile() {
    profile.displayProfile();
    pause();
}

std::vector<std::string> CLIManager::getKeywordsInput() {
    std::string line;
    std::vector<std::string> keywords;

    // std::cout << "Enter comma-separated keywords: ";
    std::getline(std::cin, line);
    std::stringstream ss(line);
    std::string keyword;
    while (std::getline(ss, keyword, ',')) {
        keywords.push_back(keyword);
    }

    return keywords;
}

void CLIManager::handleAddComponentToCompositeFood() {
    std::cout << "\n--- Add Component to Composite Food ---\n";

    // Display available composite foods
    const auto& compositeFoods = db.getCompositeFoods();
    if (compositeFoods.empty()) {
        std::cout << "No composite foods available. Create a composite food first.\n";
        pause();
        return;
    }

    std::cout << "Available Composite Foods:\n";
    for (size_t i = 0; i < compositeFoods.size(); ++i) {
        std::cout << i + 1 << ". " << compositeFoods[i]->getName() << "\n";
    }

    // Select composite food by number
    int compositeIndex;
    std::cout << "Select a composite food by number (or 0 to go back): ";
    std::cin >> compositeIndex;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (compositeIndex == 0) return;
    if (compositeIndex < 1 || compositeIndex > compositeFoods.size()) {
        std::cout << "Invalid selection.\n";
        pause();
        return;
    }

    auto compFood = std::dynamic_pointer_cast<CompositeFood>(compositeFoods[compositeIndex - 1]);

    // Search for components by keyword
    std::cout << "Enter keywords to search for components (comma-separated): ";
    auto keywords = getKeywordsInput();
    auto searchResults = db.searchFoodsByKeywords(keywords, true);

    if (searchResults.empty()) {
        std::cout << "No foods found matching the keywords.\n";
        pause();
        return;
    }

    // Display search results
    std::cout << "Search Results:\n";
    size_t index = 1;
    for (const auto& food : searchResults) {
        std::cout << index << ". " << food->getName() << " (" << food->getType() << ")\n";
        index++;
    }

    // Select component by number
    int componentIndex;
    std::cout << "Select a component by number (or 0 to go back): ";
    std::cin >> componentIndex;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (componentIndex == 0) return;
    if (componentIndex < 1 || componentIndex > searchResults.size()) {
        std::cout << "Invalid selection.\n";
        pause();
        return;
    }

    auto componentFood = searchResults[componentIndex - 1];

    // Check for circular references
    if (componentFood->getId() == compFood->getId()) {
        std::cout << "Cannot add a composite food to itself.\n";
        pause();
        return;
    }

    if (componentFood->getType() == "composite") {
        auto componentCompFood = std::dynamic_pointer_cast<CompositeFood>(componentFood);
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
    saveIfAutoSave();

    std::cout << "Successfully added " << componentFood->getName() << " to " << compFood->getName() << ".\n";
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

void CLIManager::handleEditProfile() {
    profile.editProfile();
}

void CLIManager::handleCalorieSummary() {
    // Ask the date for which the user wants to see the calorie summary
    std::string date;
    std::cout << "Enter date (YYYY-MM-DD) for calorie summary: ";
    std::getline(std::cin, date);
    // Check if the date is valid
    if (date.empty()) {
        std::cout << "Selecting today's date.\n";
        time_t now = time(0);
        struct tm* timeinfo = localtime(&now);
        char buffer[80];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d", timeinfo);
        date = buffer;
        cout << date << endl ;
    }

    // Getting the target Calories from the profile
    double targetCalories = profile.calculateTargetIntakeCalories();
    std::cout << "Your target calorie intake is: " << targetCalories << " kcal\n";

    // Get the log entries for the specified date.
    


    pause();
}