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
    bool autoSaveEnabled;

    // Main menu and sub-menus
    void showMainMenu();
    void showFoodMenu();
    void showLogMenu();
    void showProfileMenu();
    
    // Food menu handlers
    void handleViewBasicFoods();
    void handleViewCompositeFoods();
    void handleAddBasicFood();
    void handleAddCompositeFood();
    void handleAddComponentToCompositeFood();
    
    // Log menu handlers
    void handleAddLogEntry();
    void handleViewLog();
    
    // Profile menu handlers
    void handleViewProfile();
    
    // Database handlers
    void handleSaveDatabase();
    
    // Helpers
    std::vector<std::string> getKeywordsInput();
    void pause();
    void saveIfAutoSave();

public:
    CLIManager();
    void start();
};

#endif // CLI_MANAGER_H