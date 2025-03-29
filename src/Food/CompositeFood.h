#ifndef COMPOSITE_FOOD_H
#define COMPOSITE_FOOD_H

#include "Food.h"
#include <vector>
#include <memory>
#include <utility>

class CompositeFood : public Food {
private:
    // Each component is a pair: (Food pointer, servings)
    std::vector<std::pair<std::shared_ptr<Food>, double>> components;
public:
    CompositeFood(const std::string& id, const std::string& name, const std::vector<std::string>& keywords);

    void addComponent(const std::shared_ptr<Food>& food, double servings);
    double getCalories() const override;
    void display() const override;
    // Expose components for saving
    const std::vector<std::pair<std::shared_ptr<Food>, double>>& getComponents() const;
};

#endif // COMPOSITE_FOOD_H
