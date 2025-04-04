#ifndef CLI_MANAGER_H
#define CLI_MANAGER_H

#include "../Database/FoodDatabase.h"
#include "../Database/DailyLog.h"
#include "../User/DietProfile.h"

class CLIManager {
private:
    FoodDatabase db;
    DailyLog log;
    DietProfile profile;

    // Menu + handlers
    void showMenu();
    void handleViewBasicFoods();       // Show all basic foods
    void handleViewCompositeFoods();   // Show all composite foods
    void handleAddBasicFood();         // Add a basic food (name + nutrition)
    void handleAddCompositeFood();     // Add a composite food (name + components)
    void handleAddLogEntry();          // Log a food entry
    void handleViewLog();              // Display food log
    void handleViewProfile();          // Show profile info

    // Helpers
    std::vector<std::string> getKeywordsInput();
    void pause();

public:
    CLIManager();
    void start();                      // Start main loop
};

#endif // CLI_MANAGER_H
