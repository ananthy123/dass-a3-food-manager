#include <iostream>
#include <memory>
#include "Database/FoodDatabase.h"
#include "Database/DailyLog.h"
#include "User/DietProfile.h"
#include "Food/BasicFood.h"
#include "Food/CompositeFood.h"

int main() {
    std::string basicFoodDB = "../data/basic_foods.txt";
    std::string compositeFoodDB = "../data/composite_foods.txt";
    std::string dailyLogDB = "../data/daily_log.txt";
    
    // Initialize the food database with text files
    FoodDatabase database(basicFoodDB, compositeFoodDB);
    database.loadDatabase();

    // Debug
    std::cout << "Food Database initialized." << std::endl;

    // Initialize the daily log from text file
    DailyLog log(dailyLogDB);
    log.loadLog();

    // Debug
    std::cout << "Daily Log initialized." << std::endl;

    // Initialize the diet profile (example values)
    DietProfile profile("male", 180, 25, 75, 1.55);
    profile.displayProfile();

    // Debug
    std::cout << "Diet Profile initialized." << std::endl;

    // Example: Add a new basic food (e.g., Apple)
    std::shared_ptr<BasicFood> apple = std::make_shared<BasicFood>(
        "Apple", std::vector<std::string>{"fruit", "apple", "red"}, 95);
    database.addBasicFood(apple);

    // Display all basic foods
    std::cout << "\nBasic Foods:" << std::endl;
    for (const auto& food : database.getBasicFoods()) {
        food->display();
    }

    // Example: Create a composite food (e.g., Fruit Salad)
    std::shared_ptr<CompositeFood> fruitSalad = std::make_shared<CompositeFood>(
        "Fruit Salad", std::vector<std::string>{"salad", "fruit"});
    // For demonstration, add Apple twice
    fruitSalad->addComponent(apple, 2);
    database.addCompositeFood(fruitSalad);

    // Display all composite foods
    std::cout << "\nComposite Foods:" << std::endl;
    for (const auto& food : database.getCompositeFoods()) {
        food->display();
    }

    // Example: Add an entry to the daily log
    LogEntry entry = {"2025-03-28", "Apple", 1.0};
    log.addEntry(entry);

    // Display daily log
    std::cout << "\nDaily Log:" << std::endl;
    log.displayLog();

    // Save database and daily log before exiting
    database.saveDatabase();
    log.saveLog();

    return 0;
}
