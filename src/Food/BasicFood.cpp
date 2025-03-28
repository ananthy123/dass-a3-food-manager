#include "BasicFood.h"
#include <iostream>

BasicFood::BasicFood(const std::string& id, const std::vector<std::string>& keywords, double calories)
    : Food(id, keywords), calories(calories) {}

double BasicFood::getCalories() const {
    return calories;
}

void BasicFood::display() const {
    std::cout << "BasicFood: " << id << ", Calories: " << calories << std::endl;
}
