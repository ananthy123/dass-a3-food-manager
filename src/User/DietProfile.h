#ifndef DIET_PROFILE_H
#define DIET_PROFILE_H

#include <string>
#include <memory>
#include <map>
#include <functional>
#include "CalorieCalculationStrategy.h"

using namespace std;

class DietProfile {
private:
    string name;
    string gender;
    double height; // in centimeters
    int age;
    double weight; // in kilograms
    double activityLevel; // activity multiplier
    int method; // method to calculate target calories
    
    // Strategy object
    unique_ptr<CalorieCalculationStrategy> calorieStrategy;
    
    // Store a map of methods to their respective strategy classes
    // For example: 1 for Mifflin-St Jeor, 2 for Harris-Benedict, etc.
    static map<int, function<unique_ptr<CalorieCalculationStrategy>()>> strategyFactories;
    
    // Initialize the strategy factories map
    static void initializeStrategyFactories();

public:
    DietProfile(const string& name, const string& gender, double height, int age, double weight, double activityLevel, int method = 1);
    
    // Method to set the calculation strategy
    void setCalculationMethod(int methodId);
    
    // Calculate target calories using the strategy
    double calculateTargetCalories() const;
    
    // Display profile information
    void displayProfile() const;
    
    // Add new calculation method to the available methods
    static void registerCalculationMethod(int methodId, function<unique_ptr<CalorieCalculationStrategy>()> factory);
    
    // Get all available calculation methods
    static std::map<int, std::string> getAvailableCalculationMethods();

    void editProfile();  

    // Load profile information from a file
    void loadProfileFromFile(const std::string& filePath);
};

#endif // DIET_PROFILE_H
