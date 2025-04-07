#ifndef FOOD_SOURCE_H
#define FOOD_SOURCE_H

#include <vector>
#include <memory>
#include <string>
#include "../Food/Food.h"
#include "../Food/BasicFood.h"

/**
 * Abstract interface for food data sources.
 * This allows adding new ways to load food information
 * (from different file formats, databases, or APIs)
 * without modifying the core FoodDatabase logic.
 */
class FoodSource {
public:
    virtual ~FoodSource() = default;
    
    // Load basic foods from the source
    virtual std::vector<std::shared_ptr<BasicFood>> loadBasicFoods() = 0;
    
    // Get the name of the source (for UI/logging)
    virtual std::string getSourceName() const = 0;
    
    // Save basic foods to the source (if supported)
    virtual bool saveBasicFoods(const std::vector<std::shared_ptr<Food>>& foods) = 0;
};

#endif // FOOD_SOURCE_H