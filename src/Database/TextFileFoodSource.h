#ifndef TEXT_FILE_FOOD_SOURCE_H
#define TEXT_FILE_FOOD_SOURCE_H

#include "FoodSource.h"
#include <string>

/**
 * Implements the FoodSource interface for text file-based food data.
 * This class handles reading/writing food information from/to text files
 * in the format: id;name;keywords;calories;protein;carbs;fat;satfat;fiber;vitamins;minerals
 */
class TextFileFoodSource : public FoodSource {
private:
    std::string filename;
    
public:
    explicit TextFileFoodSource(const std::string& filename);
    
    std::vector<std::shared_ptr<BasicFood>> loadBasicFoods() override;
    std::string getSourceName() const override;
    bool saveBasicFoods(const std::vector<std::shared_ptr<Food>>& foods) override;
};

#endif // TEXT_FILE_FOOD_SOURCE_H