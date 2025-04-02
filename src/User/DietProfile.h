#ifndef DIET_PROFILE_H
#define DIET_PROFILE_H

#include <string>

class DietProfile {
private:
    std::string name;
    std::string gender;
    double height; // in centimeters
    int age;
    double weight; // in kilograms
    double activityLevel; // activity multiplier
    int method; // method to calculate target calories
public:
    DietProfile(const std::string& name, const std::string& gender, double height, int age, double weight, double activityLevel, int method = 1);
    double calculateTargetCalories() const;
    void displayProfile() const;
};

#endif // DIET_PROFILE_H
