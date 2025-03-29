#ifndef FOOD_DATABASE_H
#define FOOD_DATABASE_H

#include "../Food/Food.h"
#include <vector>
#include <memory>
#include <string>

class FoodDatabase {
private:
    std::vector<std::shared_ptr<Food>> basicFoods;
    std::vector<std::shared_ptr<Food>> compositeFoods;
    std::string basicFoodsFile;
    std::string compositeFoodsFile;

    // Track the last used ID number for basic & composite foods
    int basicIdCounter = 0;
    int compositeIdCounter = 0;

public:
    FoodDatabase(const std::string& basicFile, const std::string& compositeFile);

    void loadDatabase();       // Load from text files
    void saveDatabase() const; // Save to text files

    void addBasicFood(const std::shared_ptr<Food>& food);
    void addCompositeFood(const std::shared_ptr<Food>& food);

    const std::vector<std::shared_ptr<Food>>& getBasicFoods() const;
    const std::vector<std::shared_ptr<Food>>& getCompositeFoods() const;

    std::shared_ptr<Food> findFoodById(const std::string& id) const;

    // 🔁 ID Generation
    std::string generateBasicFoodId();     // returns "b_1", "b_2", etc.
    std::string generateCompositeFoodId(); // returns "c_1", "c_2", etc.
};

#endif // FOOD_DATABASE_H
