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
public:
    FoodDatabase(const std::string& basicFile, const std::string& compositeFile);
    void loadDatabase();
    void saveDatabase() const;
    void addBasicFood(const std::shared_ptr<Food>& food);
    void addCompositeFood(const std::shared_ptr<Food>& food);
    const std::vector<std::shared_ptr<Food>>& getBasicFoods() const;
    const std::vector<std::shared_ptr<Food>>& getCompositeFoods() const;
    std::shared_ptr<Food> findFoodById(const std::string& id) const;
};

#endif // FOOD_DATABASE_H
