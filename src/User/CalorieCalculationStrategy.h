#ifndef CALORIE_CALCULATION_STRATEGY_H
#define CALORIE_CALCULATION_STRATEGY_H

#include <string>

// Base Strategy class for calorie calculation
class CalorieCalculationStrategy {
public:
    virtual ~CalorieCalculationStrategy() = default;
    virtual double calculateBMR(const std::string& gender, double weight, double height, int age) const = 0;
    virtual std::string getName() const = 0;
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
    
    std::string getName() const override {
        return "Mifflin-St Jeor";
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
    
    std::string getName() const override {
        return "Harris-Benedict";
    }
};

/*
 * HOW TO ADD NEW CALORIE CALCULATION METHODS:
 * 
 * 1. Create a new concrete strategy class that inherits from CalorieCalculationStrategy
 *    and implements both calculateBMR and getName methods.
 * 
 * 2. Register your new strategy class in your application by calling the static method in the static initialization function {DietProfile::initializeStrategyFactories}:
 *    DietProfile::registerCalculationMethod(methodId, []() { 
 *        return make_unique<YourNewStrategy>(); 
 *    });
 *    
 *    Where methodId is the next available integer (e.g., 3 for the next method)
 * 
 * Example for a new Katch-McArdle formula:
 * 
 * Step 1: Create the strategy class
 * 
 * class KatchMcArdleStrategy : public CalorieCalculationStrategy {
 * public:
 *     double calculateBMR(const std::string& gender, double weight, double height, int age) const override {
 *         // Simplified implementation - real formula would use body fat percentage
 *         double lbm = 0.407 * weight + 0.267 * height - 19.2;
 *         return 370 + (21.6 * lbm);
 *     }
 *     
 *     std::string getName() const override {
 *         return "Katch-McArdle";
 *     }
 * };
 * 
 * Step 2: Register the new method in your application initialization code
 * 
 * DietProfile::registerCalculationMethod(3, []() { 
 *     return make_unique<KatchMcArdleStrategy>(); 
 * });
 */

#endif // CALORIE_CALCULATION_STRATEGY_H