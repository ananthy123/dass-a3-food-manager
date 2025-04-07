#ifndef DIET_PROFILE_H
#define DIET_PROFILE_H

#include <string>
#include <memory>
#include "CalorieCalculationStrategy.h"

class DietProfile {
private:
    std::string name;
    std::string gender;
    double height; // in centimeters
    int age;
    double weight; // in kilograms
    double activityLevel; // activity multiplier
    int method; // method to calculate target calories
    
    // Strategy object
    std::unique_ptr<CalorieCalculationStrategy> calorieStrategy;

public:
    DietProfile(const std::string& name, const std::string& gender, double height, int age, double weight, double activityLevel, int method = 1);
    
    // Method to set the calculation strategy
    void setCalculationMethod(int methodId);
    
    // Calculate target calories using the strategy
    double calculateTargetCalories() const;
    
    // Display profile information
    void displayProfile() const;
};

#endif // DIET_PROFILE_H
