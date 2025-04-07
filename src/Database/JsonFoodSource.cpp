
#include "JsonFoodSource.h"
#include <fstream>
#include <iostream>
#include <sstream>
// Note: In a real implementation, we would include a proper JSON library
// like nlohmann/json, jsoncpp, or rapidjson

JsonFoodSource::JsonFoodSource(const std::string& filename)
    : filename(filename) {}

std::vector<std::shared_ptr<BasicFood>> JsonFoodSource::loadBasicFoods() {
    std::vector<std::shared_ptr<BasicFood>> foods;
    std::ifstream inFile(filename);
    
    if (!inFile) {
        std::cerr << "Failed to open JSON foods file: " << filename << std::endl;
        return foods;
    }
    
    // In a real implementation, we would use a JSON library to parse the file
    // This is just a simple example to demonstrate the concept
    std::cout << "JSON Food Source would parse " << filename << " here" << std::endl;
    std::cout << "For demonstration purposes only - no actual JSON parsing implemented" << std::endl;
    
    // Example of how we would create food items from JSON data
    // In a real implementation, we would iterate through the JSON array
    // and create BasicFood objects from each JSON object
    /*
    // Example JSON structure:
    [
      {
        "id": "b_20",
        "name": "Avocado",
        "keywords": ["fruit", "avocado", "healthy"],
        "calories": 160,
        "protein": 2,
        "carbs": 8.5,
        "fat": 14.7,
        "saturatedFat": 2.1,
        "fiber": 6.7,
        "vitamins": "C,E,K",
        "minerals": "potassium,magnesium"
      },
      // more food items...
    ]
    */
    
    // Just adding a sample food for demonstration
    auto sampleFood = std::make_shared<BasicFood>(
        "b_20", "Avocado", 
        std::vector<std::string>{"fruit", "avocado", "healthy"},
        160, 2, 8.5, 14.7, 2.1, 6.7,
        "C,E,K", "potassium,magnesium"
    );
    
    foods.push_back(sampleFood);
    
    inFile.close();
    return foods;
}

std::string JsonFoodSource::getSourceName() const {
    return "JSON File (" + filename + ")";
}

bool JsonFoodSource::saveBasicFoods(const std::vector<std::shared_ptr<Food>>& foods) {
    std::ofstream outFile(filename);
    if (!outFile) {
        std::cerr << "Failed to open JSON file for writing: " << filename << std::endl;
        return false;
    }
    
    // In a real implementation, we would use a JSON library to create the JSON data
    // For demonstration purposes, we'll just print a message
    std::cout << "JSON Food Source would save to " << filename << " here" << std::endl;
    std::cout << "For demonstration purposes only - no actual JSON writing implemented" << std::endl;
    
    outFile << "[" << std::endl;
    
    for (size_t i = 0; i < foods.size(); ++i) {
        auto basic = std::dynamic_pointer_cast<BasicFood>(foods[i]);
        if (basic) {
            outFile << "  {" << std::endl;
            outFile << "    \"id\": \"" << basic->getId() << "\"," << std::endl;
            outFile << "    \"name\": \"" << basic->getName() << "\"," << std::endl;
            
            // Keywords array
            outFile << "    \"keywords\": [";
            auto keywords = basic->getKeywords();
            for (size_t j = 0; j < keywords.size(); ++j) {
                outFile << "\"" << keywords[j] << "\"";
                if (j < keywords.size() - 1) outFile << ", ";
            }
            outFile << "]," << std::endl;
            
            // Nutritional values
            outFile << "    \"calories\": " << basic->getCalories() << "," << std::endl;
            outFile << "    \"protein\": " << basic->getProtein() << "," << std::endl;
            outFile << "    \"carbs\": " << basic->getCarbs() << "," << std::endl;
            outFile << "    \"fat\": " << basic->getFat() << "," << std::endl;
            outFile << "    \"saturatedFat\": " << basic->getSaturatedFat() << "," << std::endl;
            outFile << "    \"fiber\": " << basic->getFiber() << "," << std::endl;
            outFile << "    \"vitamins\": \"" << basic->getVitamins() << "\"," << std::endl;
            outFile << "    \"minerals\": \"" << basic->getMinerals() << "\"" << std::endl;
            
            outFile << "  }";
            if (i < foods.size() - 1) outFile << ",";
            outFile << std::endl;
        }
    }
    
    outFile << "]" << std::endl;
    
    outFile.close();
    return true;
}