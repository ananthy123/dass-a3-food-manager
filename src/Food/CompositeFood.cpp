#include "CompositeFood.h"
#include <iostream>

CompositeFood::CompositeFood(const std::string& id, const std::string& name,
                             const std::vector<std::string>& keywords)
    : Food(id, name, keywords) {}

void CompositeFood::addComponent(const std::shared_ptr<Food>& food, double servings) {
    components.push_back(std::make_pair(food, servings));
}

double CompositeFood::getCalories() const {
    double total = 0;
    for (const auto& comp : components) {
        total += comp.first->getCalories() * comp.second;
    }
    return total;
}

void CompositeFood::display() const {
    std::cout << "CompositeFood: " << name << " (" << id << "), Total Calories: "
              << getCalories() << " kcal" << std::endl;

    for (const auto& comp : components) {
        std::cout << "  Component: ";
        comp.first->display();
        std::cout << "  Servings: " << comp.second << std::endl;
    }
}

const std::vector<std::pair<std::shared_ptr<Food>, double>>& CompositeFood::getComponents() const {
    return components;
}
