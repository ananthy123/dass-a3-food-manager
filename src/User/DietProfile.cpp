#include "DietProfile.h"
#include <iostream>

DietProfile::DietProfile(const std::string& gender, double height, int age, double weight, double activityLevel, int method)
    : gender(gender), height(height), age(age), weight(weight), activityLevel(activityLevel), method(method) {}

double DietProfile::calculateTargetCalories() const {
    // Method 1: Mifflin-St Jeor Equation
    double bmr;
    if (gender == "male") {
        bmr = 10 * weight + 6.25 * height - 5 * age + 5;
    } else {
        bmr = 10 * weight + 6.25 * height - 5 * age - 161;
    }
    if (method == 1) {
        return bmr * activityLevel;
    } else {
        // Method 2: Harris-Benedict Equation
        if (gender == "male") {
            bmr = 66 + (13.7 * weight) + (5 * height) - (6.8 * age);
        } else {
            bmr = 655 + (9.6 * weight) + (1.8 * height) - (4.7 * age);
        }
        return bmr * activityLevel;
    }
}

void DietProfile::displayProfile() const {
    std::cout << "Gender: " << gender << ", Height: " << height 
              << " cm, Age: " << age << ", Weight: " << weight 
              << " kg, Activity Level: " << activityLevel 
              << ", Target Calories: " << calculateTargetCalories() << std::endl;
}
