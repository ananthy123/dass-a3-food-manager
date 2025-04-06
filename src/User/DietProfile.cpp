#include "DietProfile.h"
#include <iostream>
#include <iomanip>
#include <cmath>

namespace diet {

// ProfileException implementation
DietProfile::ProfileException::ProfileException(const std::string& message)
    : std::runtime_error(message) {}

// Constructor with validation
DietProfile::DietProfile(const std::string& name, const std::string& gender, 
                         double height, int age, double weight, 
                         double activityLevel, CalorieCalculationMethod method)
    : name(name), gender(gender), height(height), age(age), 
      weight(weight), activityLevel(activityLevel), method(method) {
    
    // Validate parameters
    if (height <= 0) {
        throw ProfileException("Height must be positive");
    }
    if (age <= 0) {
        throw ProfileException("Age must be positive");
    }
    if (weight <= 0) {
        throw ProfileException("Weight must be positive");
    }
    if (activityLevel < 1.2 || activityLevel > 2.0) {
        throw ProfileException("Activity level multiplier must be between 1.2 and 2.0");
    }
    if (gender != "male" && gender != "female") {
        throw ProfileException("Gender must be 'male' or 'female'");
    }
}

// Getters
std::string DietProfile::getName() const { return name; }
std::string DietProfile::getGender() const { return gender; }
double DietProfile::getHeight() const { return height; }
int DietProfile::getAge() const { return age; }
double DietProfile::getWeight() const { return weight; }
double DietProfile::getActivityLevel() const { return activityLevel; }
CalorieCalculationMethod DietProfile::getMethod() const { return method; }

// Setters with validation
void DietProfile::setName(const std::string& name) {
    this->name = name;
}

void DietProfile::setGender(const std::string& gender) {
    if (gender != "male" && gender != "female") {
        throw ProfileException("Gender must be 'male' or 'female'");
    }
    this->gender = gender;
}

void DietProfile::setHeight(double height) {
    if (height <= 0) {
        throw ProfileException("Height must be positive");
    }
    this->height = height;
}

void DietProfile::setAge(int age) {
    if (age <= 0) {
        throw ProfileException("Age must be positive");
    }
    this->age = age;
}

void DietProfile::setWeight(double weight) {
    if (weight <= 0) {
        throw ProfileException("Weight must be positive");
    }
    this->weight = weight;
}

void DietProfile::setActivityLevel(double level) {
    if (level < 1.2 || level > 2.0) {
        throw ProfileException("Activity level multiplier must be between 1.2 and 2.0");
    }
    this->activityLevel = level;
}

void DietProfile::setMethod(CalorieCalculationMethod method) {
    this->method = method;
}

// Helper methods
double DietProfile::getBMR() const {
    if (method == CalorieCalculationMethod::MIFFLIN_ST_JEOR) {
        // Mifflin-St Jeor Equation
        if (gender == "male") {
            return (10 * weight) + (6.25 * height) - (5 * age) + 5;
        } else {
            return (10 * weight) + (6.25 * height) - (5 * age) - 161;
        }
    } else {
        // Harris-Benedict Equation
        if (gender == "male") {
            return 66 + (13.7 * weight) + (5 * height) - (6.8 * age);
        } else {
            return 655 + (9.6 * weight) + (1.8 * height) - (4.7 * age);
        }
    }
}

double DietProfile::getActivityMultiplier() const {
    return activityLevel;
}

// Calculate daily target calories
double DietProfile::calculateTargetCalories() const {
    return round(getBMR() * getActivityMultiplier());
}

// Calculate macronutrient recommendations
DietProfile::MacroNutrients DietProfile::calculateMacroNutrients() const {
    // Using a balanced macronutrient distribution (40% carbs, 30% protein, 30% fat)
    double calories = calculateTargetCalories();
    
    MacroNutrients macros;
    macros.carbs = round((calories * 0.4) / 4.0); // 4 calories per gram of carbs
    macros.protein = round((calories * 0.3) / 4.0); // 4 calories per gram of protein
    macros.fat = round((calories * 0.3) / 9.0); // 9 calories per gram of fat
    
    return macros;
}

// Display profile information
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
    std::cout << "Method: " << (method == CalorieCalculationMethod::MIFFLIN_ST_JEOR ? 
                               "Mifflin-St Jeor" : "Harris-Benedict") << "\n";
    
    std::cout << "Target Calories: " << calculateTargetCalories() << " kcal/day\n";
    
    auto macros = calculateMacroNutrients();
    std::cout << "Recommended Macros: \n";
    std::cout << "  Protein: " << macros.protein << " g\n";
    std::cout << "  Carbs: " << macros.carbs << " g\n";
    std::cout << "  Fat: " << macros.fat << " g\n";

    std::cout << "---------------------------------\n";
}

} // namespace diet
