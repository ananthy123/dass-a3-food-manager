#ifndef BASIC_FOOD_H
#define BASIC_FOOD_H

#include "Food.h"

namespace diet {

class BasicFood : public Food {
private:
    double calories;
    double protein;
    double carbs;
    double fat;
    double saturatedFat;
    double fiber;
    std::string vitamins;
    std::string minerals;

public:
    BasicFood(const std::string& id, 
              const std::string& name,
              const std::vector<std::string>& keywords, 
              double calories,
              double protein, 
              double carbs, 
              double fat,
              double saturatedFat, 
              double fiber,
              const std::string& vitamins, 
              const std::string& minerals);

    // Override virtual methods from base class
    double getCalories() const override;
    void display() const override;
    
    // Class-specific getters with const correctness
    double getProtein() const;
    double getCarbs() const;
    double getFat() const;
    double getSaturatedFat() const;
    double getFiber() const;
    std::string getVitamins() const;
    std::string getMinerals() const;
};

} // namespace diet

#endif // BASIC_FOOD_H
