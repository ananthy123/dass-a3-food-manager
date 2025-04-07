#ifndef FOOD_DATABASE_H
#define FOOD_DATABASE_H

#include "../Food/Food.h"
#include "FoodSource.h"
#include <vector>
#include <memory>
#include <string>

class FoodDatabase {
private:
    std::vector<std::shared_ptr<Food>> basicFoods;
    std::vector<std::shared_ptr<Food>> compositeFoods;
    std::string compositeFoodsFile;
    
    // Track the last used ID number for basic & composite foods
    int basicIdCounter = 0;
    int compositeIdCounter = 0;
    
    // Food sources collection
    std::vector<std::shared_ptr<FoodSource>> foodSources;

public:
    // Constructor takes a composite foods file and optional vector of food sources
    FoodDatabase(const std::string& compositeFile, 
                 const std::vector<std::shared_ptr<FoodSource>>& sources = {});
    
    // Add a new food source
    void addFoodSource(const std::shared_ptr<FoodSource>& source);
    
    // Load foods from all registered sources
    void loadDatabase();
    
    // Save foods to all registered sources
    void saveDatabase() const;
    
    void addBasicFood(const std::shared_ptr<Food>& food);
    void addCompositeFood(const std::shared_ptr<Food>& food);
    
    const std::vector<std::shared_ptr<Food>>& getBasicFoods() const;
    const std::vector<std::shared_ptr<Food>>& getCompositeFoods() const;
    
    std::shared_ptr<Food> findFoodById(const std::string& id) const;
    
    // ID Generation
    std::string generateBasicFoodId();
    std::string generateCompositeFoodId();
    
    // Search
    std::vector<std::shared_ptr<Food>> searchFoodsByKeywords(const std::vector<std::string>& keywords, bool matchAll) const;
    
    std::vector<std::shared_ptr<Food>> getAllFoods() const;
};

#endif // FOOD_DATABASE_H
