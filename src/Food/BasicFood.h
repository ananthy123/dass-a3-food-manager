#ifndef BASIC_FOOD_H
#define BASIC_FOOD_H

#include "Food.h"

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
    BasicFood(const std::string& id, const std::string& name,
              const std::vector<std::string>& keywords, double calories,
              double protein, double carbs, double fat,
              double saturatedFat, double fiber,
              const std::string& vitamins, const std::string& minerals);

    double getCalories() const override;
    double getProtein() const;
    double getCarbs() const;
    double getFat() const;
    double getSaturatedFat() const;
    double getFiber() const;
    std::string getVitamins() const;
    std::string getMinerals() const;
    std::string getType() const override {
        return "basic";
    }
    void display() const override;
};

#endif // BASIC_FOOD_H
