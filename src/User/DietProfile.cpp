#include "DietProfile.h"
#include <iostream>

DietProfile::DietProfile(const std::string& name, const std::string& gender, double height, int age, double weight, double activityLevel, int method)
    : name(name), gender(gender), height(height), age(age), weight(weight), activityLevel(activityLevel), method(method) {
    
        std:: cout << "Method" << method ;
    // Initialize the appropriate strategy based on method
    setCalculationMethod(method);
}

void DietProfile::setCalculationMethod(int methodId) {
    method = methodId;
    
    // Create the appropriate strategy based on method ID
    if (method == 1) {
        calorieStrategy = std::make_unique<MifflinStJeorStrategy>();
    } else {
        calorieStrategy = std::make_unique<HarrisBenedictStrategy>();
    }

    // If you added new method, add a new if else case here to incorporate that.
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
    std::cout << "Method: " << (method == 1 ? "Mifflin-St Jeor" : "Harris-Benedict") << "\n";
    std::cout << "Target Calories: " << calculateTargetCalories() << " kcal\n";

    std::cout << "---------------------------------\n";
}
