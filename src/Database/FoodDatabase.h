#ifndef FOOD_DATABASE_H
#define FOOD_DATABASE_H

#include "../Food/Food.h"
#include "../Food/BasicFood.h"
#include "../Food/CompositeFood.h"
#include <vector>
#include <memory>
#include <string>
#include <stdexcept>

namespace diet {

class FoodDatabase {
private:
    std::vector<std::shared_ptr<Food>> basicFoods;
    std::vector<std::shared_ptr<Food>> compositeFoods;
    std::string basicFoodsFile;
    std::string compositeFoodsFile;

    // Track the last used ID number for basic & composite foods
    int basicIdCounter = 0;
    int compositeIdCounter = 0;
    
    // Helper methods for parsing
    std::vector<std::string> parseKeywords(const std::string& keywordStr) const;

public:
    // Exception class for database errors
    class DatabaseException : public std::runtime_error {
    public:
        explicit DatabaseException(const std::string& message);
    };

    // Constructor
    FoodDatabase(const std::string& basicFile, const std::string& compositeFile);

    // File operations
    void loadDatabase();
    void saveDatabase() const;

    // Food management
    void addBasicFood(const std::shared_ptr<Food>& food);
    void addCompositeFood(const std::shared_ptr<Food>& food);
    bool removeFood(const std::string& id);

    // Food access
    const std::vector<std::shared_ptr<Food>>& getBasicFoods() const;
    const std::vector<std::shared_ptr<Food>>& getCompositeFoods() const;
    std::shared_ptr<Food> findFoodById(const std::string& id) const;
    std::vector<std::shared_ptr<Food>> findFoodsByKeyword(const std::string& keyword) const;

    // ID Generation
    std::string generateBasicFoodId();
    std::string generateCompositeFoodId();
};

} // namespace diet

#endif // FOOD_DATABASE_H
