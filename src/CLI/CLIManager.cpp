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
      profile("Ananth", "male", 180, 25, 75, 1.55) {

    // Validate file before loading
    if (!validateBasicFoodsFile(BASIC_FOODS_FILE)) {
        std::cerr << "🚫 Invalid format in basic_foods.txt. Fix it and rerun.\n";
        exit(1); // 🔁 STOP program from continuing
    }
    
    db.loadDatabase(); // ✅ only called if validation passed
    

    log.loadLog();
}


void CLIManager::start() {
    int choice;
    do {
        showMenu();
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice) {
            case 1: handleViewBasicFoods(); break;
            case 2: handleViewCompositeFoods(); break;
            case 3: handleAddBasicFood(); break;
            case 4: handleAddCompositeFood(); break;
            case 5: handleAddLogEntry(); break;
            case 6: handleViewLog(); break;
            case 7: handleViewProfile(); break;
            case 0: break;
            default: std::cout << "Invalid option.\n"; pause(); break;
        }
    } while (choice != 0);

    db.saveDatabase();
    log.saveLog();
    std::cout << "Data saved. Goodbye!\n";
}

void CLIManager::showMenu() {
    std::cout << "\n=== YADA: Yet Another Diet Assistant ===\n";
    std::cout << "1. View Basic Foods\n";
    std::cout << "2. View Composite Foods\n";
    std::cout << "3. Add Basic Food\n";
    std::cout << "4. Add Composite Food\n";
    std::cout << "5. Log Food Consumption\n";
    std::cout << "6. View Daily Log\n";
    std::cout << "7. View Diet Profile\n";
    std::cout << "0. Exit\n";
    std::cout << "Choose an option: ";
}

void CLIManager::handleViewBasicFoods() {
    std::cout << "\n--- Basic Foods ---\n";
    for (const auto& food : db.getBasicFoods()) {
        food->display();
    }
    pause();
}

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

void CLIManager::pause() {
    std::cout << "Press ENTER to continue...";
    std::cin.get();
}
