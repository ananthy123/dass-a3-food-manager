#ifndef BASIC_FOOD_H
#define BASIC_FOOD_H

#include "Food.h"

class BasicFood : public Food {
private:
    double calories;
public:
    BasicFood(const std::string& id, const std::vector<std::string>& keywords, double calories);
    double getCalories() const override;
    void display() const override;
};

#endif // BASIC_FOOD_H
