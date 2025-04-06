#include "CLIManager.h"
#include "../Food/BasicFood.h"
#include "../Food/CompositeFood.h"
#include <iostream>
#include <sstream>
#include <limits>
#include <fstream>
#include <regex>
#include <iomanip>

namespace diet {

// Command implementations for menu items
class ViewBasicFoodsCommand : public Command {
private:
    CLIManager& cli;
public:
    ViewBasicFoodsCommand(CLIManager& cli) : cli(cli) {}
    void execute() override { cli.handleViewBasicFoods(); }
    std::string getDescription() const override { return "View Basic Foods"; }
};

class ViewCompositeFoodsCommand : public Command {
private:
    CLIManager& cli;
public:
    ViewCompositeFoodsCommand(CLIManager& cli) : cli(cli) {}
    void execute() override { cli.handleViewCompositeFoods(); }
    std::string getDescription() const override { return "View Composite Foods"; }
};

class AddBasicFoodCommand : public Command {
private:
    CLIManager& cli;
public:
    AddBasicFoodCommand(CLIManager& cli) : cli(cli) {}
    void execute() override { cli.handleAddBasicFood(); }
    std::string getDescription() const override { return "Add Basic Food"; }
};

class AddCompositeFoodCommand : public Command {
private:
    CLIManager& cli;
public:
    AddCompositeFoodCommand(CLIManager& cli) : cli(cli) {}
    void execute() override { cli.handleAddCompositeFood(); }
    std::string getDescription() const override { return "Add Composite Food"; }
};

class AddLogEntryCommand : public Command {
private:
    CLIManager& cli;
public:
    AddLogEntryCommand(CLIManager& cli) : cli(cli) {}
    void execute() override { cli.handleAddLogEntry(); }
    std::string getDescription() const override { return "Log Food Consumption"; }
};

class ViewLogCommand : public Command {
private:
    CLIManager& cli;
public:
    ViewLogCommand(CLIManager& cli) : cli(cli) {}
    void execute() override { cli.handleViewLog(); }
    std::string getDescription() const override { return "View Daily Log"; }
};

class ViewProfileCommand : public Command {
private:
    CLIManager& cli;
public:
    ViewProfileCommand(CLIManager& cli) : cli(cli) {}
    void execute() override { cli.handleViewProfile(); }
    std::string getDescription() const override { return "View Diet Profile"; }
};

class SearchFoodsCommand : public Command {
private:
    CLIManager& cli;
public:
    SearchFoodsCommand(CLIManager& cli) : cli(cli) {}
    void execute() override { cli.handleSearchFoods(); }
    std::string getDescription() const override { return "Search Foods"; }
};

class ExitCommand : public Command {
private:
    CLIManager& cli;
public:
    ExitCommand(CLIManager& cli) : cli(cli) {}
    void execute() override { cli.handleExit(); }
    std::string getDescription() const override { return "Exit"; }
};

// CLIManager implementation
bool CLIManager::validateBasicFoodsFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "❌ File not found or cannot be accessed: " << filename << std::endl;
        std::cerr << "Please ensure the file exists and has proper read permissions." << std::endl;
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

CLIManager::CLIManager(const std::string& basicFoodsPath, 
                       const std::string& compositeFoodsPath, 
                       const std::string& logPath)
    : db(basicFoodsPath, compositeFoodsPath),
      log(logPath),
      profile("Ananth", "male", 180, 25, 75, 1.55),
      running(false),
      basicFoodsFile(basicFoodsPath),
      compositeFoodsFile(compositeFoodsPath),
      dailyLogFile(logPath) {

    // Check if all required files exist or are accessible
    bool filesOK = true;
    
    // Validate basic foods file before loading
    if (!validateBasicFoodsFile(basicFoodsFile)) {
        std::cerr << "🚫 Invalid format or inaccessible basic foods file: " << basicFoodsFile << std::endl;
        std::cerr << "Fix the file and rerun the application." << std::endl;
        filesOK = false;
    }
    
    // Check if composite foods file exists and is accessible
    std::ifstream compFile(compositeFoodsFile);
    if (!compFile) {
        std::cerr << "⚠️ Composite foods file not found or inaccessible: " << compositeFoodsFile << std::endl;
        std::cerr << "A new file will be created when saving data." << std::endl;
        // Not fatal, will create on save
    }
    
    // Check if log file exists and is accessible
    std::ifstream logFile(dailyLogFile);
    if (!logFile) {
        std::cerr << "⚠️ Log file not found or inaccessible: " << dailyLogFile << std::endl;
        std::cerr << "A new file will be created when saving data." << std::endl;
        // Not fatal, will create on save
    }
    
    if (!filesOK) {
        throw std::runtime_error("Critical files are missing or have invalid format");
    }
    
    try {
        db.loadDatabase(); // Only called if validation passed
        log.loadLog();
    } catch (const std::exception& e) {
        std::cerr << "Error loading data: " << e.what() << std::endl;
        throw std::runtime_error("Failed to load database: " + std::string(e.what()));
    }
    
    // Initialize menu commands
    initializeMenu();
}

void CLIManager::initializeMenu() {
    menuCommands.push_back(std::make_unique<ViewBasicFoodsCommand>(*this));
    menuCommands.push_back(std::make_unique<ViewCompositeFoodsCommand>(*this));
    menuCommands.push_back(std::make_unique<AddBasicFoodCommand>(*this));
    menuCommands.push_back(std::make_unique<AddCompositeFoodCommand>(*this));
    menuCommands.push_back(std::make_unique<AddLogEntryCommand>(*this));
    menuCommands.push_back(std::make_unique<ViewLogCommand>(*this));
    menuCommands.push_back(std::make_unique<ViewProfileCommand>(*this));
    menuCommands.push_back(std::make_unique<SearchFoodsCommand>(*this));
    menuCommands.push_back(std::make_unique<ExitCommand>(*this));
}

void CLIManager::start() {
    running = true;
    
    while (running) {
        showMenu();
        
        int choice = getIntInput("Choose an option: ", 0, menuCommands.size());
        
        if (choice >= 0 && choice < static_cast<int>(menuCommands.size())) {
            menuCommands[choice]->execute();
        } else {
            std::cout << "Invalid option.\n";
            pause();
        }
    }

    // Save data before exiting
    try {
        db.saveDatabase();
        log.saveLog();
        std::cout << "Data saved successfully. Goodbye!\n";
    } catch (const std::exception& e) {
        std::cerr << "Error saving data: " << e.what() << "\n";
    }
}

void CLIManager::showMenu() const {
    std::cout << "\n=== YADA: Yet Another Diet Assistant ===\n";
    
    for (size_t i = 0; i < menuCommands.size(); ++i) {
        std::cout << i << ". " << menuCommands[i]->getDescription() << "\n";
    }
}

void CLIManager::handleViewBasicFoods() {
    std::cout << "\n--- Basic Foods ---\n";
    const auto& foods = db.getBasicFoods();
    
    if (foods.empty()) {
        std::cout << "No basic foods found.\n";
    } else {
        for (const auto& food : foods) {
            food->display();
        }
    }
    pause();
}

void CLIManager::handleViewCompositeFoods() {
    std::cout << "\n--- Composite Foods ---\n";
    const auto& foods = db.getCompositeFoods();
    
    if (foods.empty()) {
        std::cout << "No composite foods found.\n";
    } else {
        for (const auto& food : foods) {
            food->display();
        }
    }
    pause();
}

void CLIManager::handleAddBasicFood() {
    std::string name;
    std::cout << "Enter food name: ";
    std::getline(std::cin, name);

    auto keywords = getKeywordsInput();

    double calories = getNumericInput("Calories: ");
    double protein = getNumericInput("Protein (g): ");
    double carbs = getNumericInput("Carbs (g): ");
    double fat = getNumericInput("Fat (g): ");
    double satFat = getNumericInput("Saturated Fat (g): ");
    double fiber = getNumericInput("Fiber (g): ");

    std::string vitamins, minerals;
    std::cout << "Vitamins (comma-separated): ";
    std::getline(std::cin, vitamins);
    std::cout << "Minerals (comma-separated): ";
    std::getline(std::cin, minerals);

    std::string id = db.generateBasicFoodId();

    try {
        auto food = std::make_shared<BasicFood>(
            id, name, keywords, calories, protein, carbs, fat, satFat, fiber, vitamins, minerals
        );
        db.addBasicFood(food);
        std::cout << "Basic food added with ID: " << id << "\n";
    } catch (const std::exception& e) {
        std::cerr << "Error adding food: " << e.what() << "\n";
    }
    
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

        double servings = getNumericInput("Enter servings: ", 0.1);
        
        try {
            comp->addComponent(food, servings);
        } catch (const std::exception& e) {
            std::cerr << "Error adding component: " << e.what() << "\n";
        }
    }

    try {
        db.addCompositeFood(comp);
        std::cout << "Composite food added with ID: " << id << "\n";
    } catch (const std::exception& e) {
        std::cerr << "Error adding composite food: " << e.what() << "\n";
    }
    
    pause();
}

void CLIManager::handleAddLogEntry() {
    std::string date, id;
    double servings;

    std::cout << "Enter date (YYYY-MM-DD): ";
    std::getline(std::cin, date);
    
    // Validate date format
    std::regex datePattern(R"(\d{4}-\d{2}-\d{2})");
    if (!std::regex_match(date, datePattern)) {
        std::cout << "Invalid date format. Use YYYY-MM-DD.\n";
        pause();
        return;
    }
    
    std::cout << "Enter food ID: ";
    std::getline(std::cin, id);
    
    auto food = db.findFoodById(id);
    if (!food) {
        std::cout << "Food not found with ID: " << id << "\n";
        pause();
        return;
    }
    
    servings = getNumericInput("Enter servings: ", 0.1);

    try {
        log.addEntry(LogEntry(date, id, servings));
        std::cout << "Log entry added for " << food->getName() << "\n";
    } catch (const std::exception& e) {
        std::cerr << "Error adding log entry: " << e.what() << "\n";
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

void CLIManager::handleSearchFoods() {
    std::string keyword;
    std::cout << "Enter search term: ";
    std::getline(std::cin, keyword);
    
    if (keyword.empty()) {
        std::cout << "Search term cannot be empty.\n";
        pause();
        return;
    }
    
    auto results = db.findFoodsByKeyword(keyword);
    
    if (results.empty()) {
        std::cout << "No foods found matching '" << keyword << "'.\n";
    } else {
        std::cout << "\n--- Search Results for '" << keyword << "' ---\n";
        std::cout << "Found " << results.size() << " matching foods:\n";
        
        for (const auto& food : results) {
            food->display();
        }
    }
    
    pause();
}

void CLIManager::handleExit() {
    running = false;
}

std::vector<std::string> CLIManager::getKeywordsInput() const {
    std::string line;
    std::vector<std::string> keywords;

    std::cout << "Enter comma-separated keywords: ";
    std::getline(std::cin, line);
    
    if (line.empty()) {
        return keywords;
    }
    
    std::stringstream ss(line);
    std::string keyword;
    while (std::getline(ss, keyword, ',')) {
        // Trim whitespace
        keyword.erase(0, keyword.find_first_not_of(" \t\r\n"));
        keyword.erase(keyword.find_last_not_of(" \t\r\n") + 1);
        
        if (!keyword.empty()) {
            keywords.push_back(keyword);
        }
    }

    return keywords;
}

void CLIManager::pause() const {
    std::cout << "Press ENTER to continue...";
    std::cin.get();
}

bool CLIManager::validateInput(const std::string& input, 
                               const std::function<bool(const std::string&)>& validator) const {
    try {
        return validator(input);
    } catch (...) {
        return false;
    }
}

double CLIManager::getNumericInput(const std::string& prompt, double min, double max) const {
    while (true) {
        std::cout << prompt;
        std::string input;
        std::getline(std::cin, input);
        
        try {
            double value = std::stod(input);
            if (value >= min && value <= max) {
                return value;
            }
            std::cout << "Value must be between " << min << " and " << max << "\n";
        } catch (...) {
            std::cout << "Invalid number. Please try again.\n";
        }
    }
}

int CLIManager::getIntInput(const std::string& prompt, int min, int max) const {
    while (true) {
        std::cout << prompt;
        std::string input;
        std::getline(std::cin, input);
        
        try {
            int value = std::stoi(input);
            if (value >= min && value <= max) {
                return value;
            }
            std::cout << "Value must be between " << min << " and " << max << "\n";
        } catch (...) {
            std::cout << "Invalid number. Please try again.\n";
        }
    }
}

} // namespace diet
