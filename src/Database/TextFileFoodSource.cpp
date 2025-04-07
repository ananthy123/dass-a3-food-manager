#include "TextFileFoodSource.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

TextFileFoodSource::TextFileFoodSource(const std::string& filename)
    : filename(filename) {}

std::vector<std::shared_ptr<BasicFood>> TextFileFoodSource::loadBasicFoods() {
    std::vector<std::shared_ptr<BasicFood>> foods;
    std::ifstream inFile(filename);
    
    if (!inFile) {
        std::cerr << "Failed to open basic foods file: " << filename << std::endl;
        return foods;
    }
    
    std::string line;
    while (std::getline(inFile, line)) {
        if (line.empty() || line[0] == '#') continue;
        
        std::stringstream ss(line);
        std::string id, name, keywordStr, calStr, proteinStr, carbStr, fatStr, satFatStr, fiberStr, vitaminStr, mineralStr;
        
        // Attempt to extract all fields
        if (!std::getline(ss, id, ';') || !std::getline(ss, name, ';') || !std::getline(ss, keywordStr, ';') ||
            !std::getline(ss, calStr, ';') || !std::getline(ss, proteinStr, ';') || !std::getline(ss, carbStr, ';') ||
            !std::getline(ss, fatStr, ';') || !std::getline(ss, satFatStr, ';') || !std::getline(ss, fiberStr, ';') ||
            !std::getline(ss, vitaminStr, ';') || !std::getline(ss, mineralStr)) {
            std::cerr << "❌ Skipping malformed line (not enough fields):\n  " << line << "\n";
            continue;
        }
        
        try {
            // Parse keywords
            std::vector<std::string> keywords;
            std::stringstream ks(keywordStr);
            std::string kw;
            while (std::getline(ks, kw, ',')) {
                kw.erase(0, kw.find_first_not_of(" \n\r\t"));
                kw.erase(kw.find_last_not_of(" \n\r\t") + 1);
                keywords.push_back(kw);
            }
            
            // Convert all numeric fields safely
            double calories = std::stod(calStr);
            double protein = std::stod(proteinStr);
            double carbs = std::stod(carbStr);
            double fat = std::stod(fatStr);
            double satFat = std::stod(satFatStr);
            double fiber = std::stod(fiberStr);
            
            auto food = std::make_shared<BasicFood>(
                id, name, keywords,
                calories, protein, carbs, fat, satFat, fiber,
                vitaminStr, mineralStr
            );
            
            foods.push_back(food);
            
        } catch (const std::exception& e) {
            std::cerr << "❌ Failed to load line:\n  " << line << "\n  → " << e.what() << "\n";
        }
    }
    
    inFile.close();
    return foods;
}

std::string TextFileFoodSource::getSourceName() const {
    return "Text File (" + filename + ")";
}

bool TextFileFoodSource::saveBasicFoods(const std::vector<std::shared_ptr<Food>>& foods) {
    std::ofstream outFile(filename);
    if (!outFile) {
        std::cerr << "Failed to open file for writing: " << filename << std::endl;
        return false;
    }
    
    for (const auto& food : foods) {
        auto basic = std::dynamic_pointer_cast<BasicFood>(food);
        if (basic) {
            outFile << basic->getId() << ";" << basic->getName() << ";";
            
            auto keywords = basic->getKeywords();
            for (size_t i = 0; i < keywords.size(); ++i) {
                outFile << keywords[i];
                if (i < keywords.size() - 1) outFile << ",";
            }
            
            outFile << ";" << basic->getCalories()
                   << ";" << basic->getProtein()
                   << ";" << basic->getCarbs()
                   << ";" << basic->getFat()
                   << ";" << basic->getSaturatedFat()
                   << ";" << basic->getFiber()
                   << ";" << basic->getVitamins()
                   << ";" << basic->getMinerals() << "\n";
        }
    }
    
    outFile.close();
    return true;
}