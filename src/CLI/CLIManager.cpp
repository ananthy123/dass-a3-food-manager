#include "CLIManager.h"
#include "../Food/BasicFood.h"
#include "../Food/CompositeFood.h"
#include <iostream>
#include <sstream>
#include <limits>

const std::string BASIC_FOODS_FILE = "../data/basic_foods.txt";
const std::string COMPOSITE_FOODS_FILE = "../data/composite_foods.txt";
const std::string DAILY_LOG_FILE = "../data/daily_log.txt";

CLIManager::CLIManager()
    : db(BASIC_FOODS_FILE , COMPOSITE_FOODS_FILE),
      log(DAILY_LOG_FILE),
      profile("male", 180, 25, 75, 1.55) {
    db.loadDatabase();
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
    std::string id;
    double calories;
    std::cout << "Enter food ID: ";
    std::getline(std::cin, id);
    auto keywords = getKeywordsInput();
    std::cout << "Enter calories: ";
    std::cin >> calories;
    std::cin.ignore();
    db.addBasicFood(std::make_shared<BasicFood>(id, keywords, calories));
    std::cout << "Basic food added.\n";
    pause();
}

void CLIManager::handleAddCompositeFood() {
    std::string id;
    std::cout << "Enter composite food ID: ";
    std::getline(std::cin, id);
    auto keywords = getKeywordsInput();
    auto comp = std::make_shared<CompositeFood>(id, keywords);

    std::string compId;
    while (true) {
        std::cout << "Enter component ID (or 'done'): ";
        std::getline(std::cin, compId);
        if (compId == "done") break;
        auto food = db.findFoodById(compId);
        if (!food) {
            std::cout << "Not found.\n";
            continue;
        }
        double servings;
        std::cout << "Servings: ";
        std::cin >> servings;
        std::cin.ignore();
        comp->addComponent(food, servings);
    }

    db.addCompositeFood(comp);
    std::cout << "Composite food added.\n";
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
