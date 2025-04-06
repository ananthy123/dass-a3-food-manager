#ifndef COMPOSITE_FOOD_H
#define COMPOSITE_FOOD_H

#include "Food.h"
#include <vector>
#include <memory>
#include <utility>

namespace diet {

class CompositeFood : public Food {
private:
    // Each component is a pair: (Food pointer, servings)
    std::vector<std::pair<std::shared_ptr<Food>, double>> components;
    
public:
    CompositeFood(const std::string& id, const std::string& name, const std::vector<std::string>& keywords);
    
    // Add a component food with specified servings
    void addComponent(const std::shared_ptr<Food>& food, double servings);
    
    // Override base class methods
    double getCalories() const override;
    void display() const override;
    
    // Accessor for components (needed for serialization)
    const std::vector<std::pair<std::shared_ptr<Food>, double>>& getComponents() const;
};

} // namespace diet

#endif // COMPOSITE_FOOD_H
