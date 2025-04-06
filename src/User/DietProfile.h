#ifndef DIET_PROFILE_H
#define DIET_PROFILE_H

#include <string>
#include <stdexcept>

namespace diet {

// Enumeration for calculation methods
enum class CalorieCalculationMethod {
    MIFFLIN_ST_JEOR = 1,
    HARRIS_BENEDICT = 2
};

// Enumeration for activity levels with descriptive names
enum class ActivityLevel {
    SEDENTARY = 1,         // 1.2, Little or no exercise
    LIGHTLY_ACTIVE = 2,    // 1.375, Light exercise 1-3 days/week
    MODERATELY_ACTIVE = 3, // 1.55, Moderate exercise 3-5 days/week
    VERY_ACTIVE = 4,       // 1.725, Hard exercise 6-7 days/week
    EXTRA_ACTIVE = 5       // 1.9, Very hard exercise & physical job or training twice a day
};

class DietProfile {
private:
    std::string name;
    std::string gender;
    double height;      // in centimeters
    int age;
    double weight;      // in kilograms
    double activityLevel; // activity multiplier
    CalorieCalculationMethod method;
    
    // Helper methods for calculations
    double getBMR() const;
    double getActivityMultiplier() const;
    
public:
    // Constructor with default method
    DietProfile(const std::string& name, 
                const std::string& gender, 
                double height, 
                int age, 
                double weight, 
                double activityLevel, 
                CalorieCalculationMethod method = CalorieCalculationMethod::MIFFLIN_ST_JEOR);
    
    // Getters
    std::string getName() const;
    std::string getGender() const;
    double getHeight() const;
    int getAge() const;
    double getWeight() const;
    double getActivityLevel() const;
    CalorieCalculationMethod getMethod() const;
    
    // Setters with validation
    void setName(const std::string& name);
    void setGender(const std::string& gender);
    void setHeight(double height);
    void setAge(int age);
    void setWeight(double weight);
    void setActivityLevel(double level);
    void setMethod(CalorieCalculationMethod method);
    
    // Calculate daily calorie needs
    double calculateTargetCalories() const;
    
    // Calculate macronutrient recommendations
    struct MacroNutrients {
        double protein;  // in grams
        double carbs;    // in grams
        double fat;      // in grams
    };
    
    MacroNutrients calculateMacroNutrients() const;
    
    // Display profile information
    void displayProfile() const;
    
    // Exception class for profile validation errors
    class ProfileException : public std::runtime_error {
    public:
        explicit ProfileException(const std::string& message);
    };
};

} // namespace diet

#endif // DIET_PROFILE_H
