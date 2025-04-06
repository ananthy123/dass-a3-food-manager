#include "CompositeFood.h"
#include <iostream>
#include <iomanip>

namespace diet {

CompositeFood::CompositeFood(const std::string& id, const std::string& name,
                             const std::vector<std::string>& keywords)
    : Food(id, name, keywords) {}

void CompositeFood::addComponent(const std::shared_ptr<Food>& food, double servings) {
    if (food && servings > 0) {
        components.push_back(std::make_pair(food, servings));
    } else {
        throw std::invalid_argument("Invalid food component or servings amount");
    }
}

double CompositeFood::getCalories() const {
    double total = 0.0;
    for (const auto& comp : components) {
        total += comp.first->getCalories() * comp.second;
    }
    return total;
}

void CompositeFood::display() const {
    std::cout << "CompositeFood: " << name << " (" << id << ")" << std::endl;
    std::cout << "  Total Calories: " << std::fixed << std::setprecision(1) 
              << getCalories() << " kcal" << std::endl;

    if (!components.empty()) {
        std::cout << "  Components:" << std::endl;
        for (const auto& comp : components) {
            std::cout << "    - " << comp.first->getName() 
                      << " (ID: " << comp.first->getId() << ")"
                      << ", Servings: " << comp.second << std::endl;
        }
    } else {
        std::cout << "  No components added yet." << std::endl;
    }
}

const std::vector<std::pair<std::shared_ptr<Food>, double>>& CompositeFood::getComponents() const {
    return components;
}

} // namespace diet
