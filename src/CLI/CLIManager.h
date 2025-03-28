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

    void showMenu();
    void handleViewBasicFoods();
    void handleViewCompositeFoods();
    void handleAddBasicFood();
    void handleAddCompositeFood();
    void handleAddLogEntry();
    void handleViewLog();
    void handleViewProfile();

    std::vector<std::string> getKeywordsInput();
    void pause();

public:
    CLIManager();
    void start();
};

#endif // CLI_MANAGER_H
