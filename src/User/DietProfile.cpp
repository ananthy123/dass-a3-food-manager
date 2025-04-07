#include "DietProfile.h"
#include <iostream>

// Initialize static member
map<int, function<unique_ptr<CalorieCalculationStrategy>()>> DietProfile::strategyFactories;

// Static initialization method
void DietProfile::initializeStrategyFactories() {
    if (strategyFactories.empty()) {
        // Register the default calculation methods
        registerCalculationMethod(1, []() { return make_unique<MifflinStJeorStrategy>(); });
        registerCalculationMethod(2, []() { return make_unique<HarrisBenedictStrategy>(); });
    }
}

// Static method to register new calculation methods
void DietProfile::registerCalculationMethod(int methodId, function<unique_ptr<CalorieCalculationStrategy>()> factory) {
    // Check if the methodId is already registered
    if (strategyFactories.find(methodId) != strategyFactories.end()) {
        std::cerr << "Error: Method ID " << methodId << " is already registered.\n";
        return;
    }
    // Register the new method
    strategyFactories[methodId] = factory;
}

DietProfile::DietProfile(const std::string& name, const std::string& gender, double height, int age, double weight, double activityLevel, int method)
    : name(name), gender(gender), height(height), age(age), weight(weight), activityLevel(activityLevel), method(method) {
    
    // Ensure strategy factories are initialized
    initializeStrategyFactories();
    
    // Initialize the appropriate strategy based on method
    setCalculationMethod(method);
}

void DietProfile::setCalculationMethod(int methodId) {
    // Check if the method exists in our map
    if (strategyFactories.find(methodId) != strategyFactories.end()) {
        method = methodId;
        calorieStrategy = strategyFactories[methodId]();
    } else {
        // Default to method 1 if invalid method is provided
        method = 1;
        calorieStrategy = strategyFactories[1]();
        std::cout << "Warning: Invalid calculation method specified. Using " 
                  << calorieStrategy->getName() << " by default.\n";
    }
}

double DietProfile::calculateTargetCalories() const {
    // Use the strategy to calculate BMR
    double bmr = calorieStrategy->calculateBMR(gender, weight, height, age);
    
    // Apply activity level multiplier
    return bmr * activityLevel;
}

void DietProfile::displayProfile() const {
    std::cout << "-------------------------------\n";
    std::cout << "\tDiet Profile\n";
    std::cout << "-------------------------------\n";

    std::cout << "Name: " << name << "\n";
    std::cout << "Gender: " << gender << "\n";
    std::cout << "Height: " << height << " cm\n";
    std::cout << "Age: " << age << " years\n";
    std::cout << "Weight: " << weight << " kg\n";
    std::cout << "Activity Level: " << activityLevel << "\n";
    
    // Get method name directly from the strategy object
    std::string methodName = calorieStrategy ? calorieStrategy->getName() : "Unknown";
    std::cout << "Method: " << methodName << "\n";
    std::cout << "Target Calories: " << calculateTargetCalories() << " kcal\n";

    std::cout << "---------------------------------\n";
}

// Get all available calculation methods
std::map<int, std::string> DietProfile::getAvailableCalculationMethods() {
    // Make sure the factories are initialized
    initializeStrategyFactories();
    
    std::map<int, std::string> methodNames;
    
    // Get the name of each strategy
    for (const auto& pair : strategyFactories) {
        // Create a temporary instance of the strategy to get its name
        auto strategy = pair.second();
        methodNames[pair.first] = strategy->getName();
    }
    
    return methodNames;
}
