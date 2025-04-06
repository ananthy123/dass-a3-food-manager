#ifndef CLI_MANAGER_H
#define CLI_MANAGER_H

#include "../Database/FoodDatabase.h"
#include "../Database/DailyLog.h"
#include "../User/DietProfile.h"
#include <string>
#include <vector>
#include <functional>
#include <memory>
#include <unordered_map>
#include <limits>

namespace diet {

// Forward declarations
class MenuItem;

// Command pattern for menu actions
class Command {
public:
    virtual ~Command() = default;
    virtual void execute() = 0;
    virtual std::string getDescription() const = 0;
};

class CLIManager {
private:
    FoodDatabase db;
    DailyLog log;
    DietProfile profile;
    bool running;
    
    // Menu items
    std::vector<std::unique_ptr<Command>> menuCommands;
    
    // File paths as const members
    const std::string basicFoodsFile;
    const std::string compositeFoodsFile;
    const std::string dailyLogFile;

    // Menu handling
    void initializeMenu();
    void showMenu() const;

    // Helper methods
    std::vector<std::string> getKeywordsInput() const;
    void pause() const;
    bool validateInput(const std::string& input, const std::function<bool(const std::string&)>& validator) const;
    double getNumericInput(const std::string& prompt, double min = 0.0, double max = std::numeric_limits<double>::max()) const;
    int getIntInput(const std::string& prompt, int min = 0, int max = std::numeric_limits<int>::max()) const;

public:
    // Constructor with explicit file paths
    CLIManager(const std::string& basicFoodsPath = "../data/basic_foods.txt",
               const std::string& compositeFoodsPath = "../data/composite_foods.txt", 
               const std::string& logPath = "../data/daily_log.txt");
               
    // Main entry point
    void start();

    // Command implementations
    void handleViewBasicFoods();
    void handleViewCompositeFoods();
    void handleAddBasicFood();
    void handleAddCompositeFood();
    void handleAddLogEntry();
    void handleViewLog();
    void handleViewProfile();
    void handleSearchFoods();
    void handleExit();

    // Validator for basic foods file format
    bool validateBasicFoodsFile(const std::string& filename);
};

} // namespace diet

#endif // CLI_MANAGER_H
