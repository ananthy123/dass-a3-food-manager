/**
 * @file JsonFoodSource.h
 * 
 * This file demonstrates how the FoodSource architecture enables easy extensibility:
 * 
 * 1. ADDING NEW DATA SOURCES:
 *    To add a new food data source (e.g., database, API, XML, CSV):
 *    - Create a new class that inherits from FoodSource
 *    - Implement the required interface methods (loadBasicFoods, saveBasicFoods, getSourceName)
 *    - Register an instance with the FoodDatabase using db.addFoodSource()
 * 
 * 2. ZERO RIPPLE EFFECTS:
 *    - The FoodDatabase doesn't need to know the implementation details of how data is stored
 *    - No changes needed to existing code when adding new sources
 *    - All source-specific logic is encapsulated within the source class
 * 
 * 3. MULTIPLE SOURCES:
 *    - The system can load foods from multiple sources simultaneously
 *    - Each source's implementation is isolated from others
 *    - Makes it easy to implement functionality like "import from JSON"
 * 
 * 4. PLUGGABLE ARCHITECTURE:
 *    - Sources can be added/removed dynamically at runtime
 *    - New formats can be added in future releases without breaking changes
 *    - Follows Open/Closed Principle: open for extension, closed for modification
 * 
 * Example usage:
 *    auto jsonSource = std::make_shared<JsonFoodSource>("foods.json");
 *    auto dbSource = std::make_shared<DatabaseFoodSource>("connection_string");
 *    auto apiSource = std::make_shared<ApiBasedFoodSource>("api_key", "endpoint");
 *    
 *    foodDatabase.addFoodSource(jsonSource);
 *    foodDatabase.addFoodSource(dbSource);
 *    foodDatabase.addFoodSource(apiSource);
 */

#ifndef JSON_FOOD_SOURCE_H
#define JSON_FOOD_SOURCE_H

#include "FoodSource.h"
#include <string>

/**
 * Implements the FoodSource interface for JSON-based food data.
 * This is an example of how easily we can add new food data sources
 * to the system without modifying any existing code.
 */
class JsonFoodSource : public FoodSource {
private:
    std::string filename;
    
public:
    explicit JsonFoodSource(const std::string& filename);
    
    std::vector<std::shared_ptr<BasicFood>> loadBasicFoods() override;
    std::string getSourceName() const override;
    bool saveBasicFoods(const std::vector<std::shared_ptr<Food>>& foods) override;
};

#endif // JSON_FOOD_SOURCE_H