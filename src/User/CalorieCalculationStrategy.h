#ifndef CALORIE_CALCULATION_STRATEGY_H
#define CALORIE_CALCULATION_STRATEGY_H

#include <string>

// Base Strategy class for calorie calculation
class CalorieCalculationStrategy {
public:
    virtual ~CalorieCalculationStrategy() = default;
    virtual double calculateBMR(const std::string& gender, double weight, double height, int age) const = 0;
};

// Mifflin-St Jeor Equation Strategy
class MifflinStJeorStrategy : public CalorieCalculationStrategy {
public:
    double calculateBMR(const std::string& gender, double weight, double height, int age) const override {
        if (gender == "male") {
            return 10 * weight + 6.25 * height - 5 * age + 5;
        } else {
            return 10 * weight + 6.25 * height - 5 * age - 161;
        }
    }
};

// Harris-Benedict Equation Strategy
class HarrisBenedictStrategy : public CalorieCalculationStrategy {
public:
    double calculateBMR(const std::string& gender, double weight, double height, int age) const override {
        if (gender == "male") {
            return 66 + (13.7 * weight) + (5 * height) - (6.8 * age);
        } else {
            return 655 + (9.6 * weight) + (1.8 * height) - (4.7 * age);
        }
    }
};

// you can add new calorie calculation methods by:

// Adding a new concrete strategy class in CalorieCalculationStrategy.h
// Adding a new case in the setCalculationMethod() method in DietProfile.cpp and do it's calculation in a function calculateBMR()


#endif // CALORIE_CALCULATION_STRATEGY_H