#include "FoodDatabase.h"
#include "../Food/BasicFood.h"
#include "../Food/CompositeFood.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

FoodDatabase::FoodDatabase(const std::string& basicFile, const std::string& compositeFile)
    : basicFoodsFile(basicFile), compositeFoodsFile(compositeFile) {}

std::shared_ptr<Food> FoodDatabase::findFoodById(const std::string& id) const {
    for (const auto& food : basicFoods) {
        if (food->getId() == id) return food;
    }
    for (const auto& food : compositeFoods) {
        if (food->getId() == id) return food;
    }
    return nullptr;
}

void FoodDatabase::loadDatabase() {
    std::ifstream inBasic(basicFoodsFile);
    if (!inBasic) {
        std::cerr << "Failed to open basic foods file: " << basicFoodsFile << std::endl;
        return;
    }

    std::string line;
    while (std::getline(inBasic, line)) {
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
                kw.erase(kw.find_last_not_of(" \n\r\t") + 1);
                keywords.push_back(kw);
            }
    
            // Track max ID for auto-ID generation
            if (id.rfind("b_", 0) == 0) {
                int num = std::stoi(id.substr(2));
                basicIdCounter = std::max(basicIdCounter, num);
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
    
            basicFoods.push_back(food);
    
        } catch (const std::invalid_argument& e) {
            std::cerr << "❌ Invalid numeric value in line:\n  " << line << "\n  → " << e.what() << "\n";
        } catch (const std::exception& e) {
            std::cerr << "❌ Failed to load line:\n  " << line << "\n  → " << e.what() << "\n";
        }
    }
    
    inBasic.close();

    std::ifstream inComp(compositeFoodsFile);
    if (!inComp) {
        std::cerr << "Failed to open composite foods file: " << compositeFoodsFile << std::endl;
        return;
    }

    while (std::getline(inComp, line)) {
        if (line.empty() || line[0] == '#') continue;
        std::stringstream ss(line);
        std::string id, name, keywordStr, compStr;

        std::getline(ss, id, ';');
        std::getline(ss, name, ';');
        std::getline(ss, keywordStr, ';');
        std::getline(ss, compStr);

        std::vector<std::string> keywords;
        std::stringstream ks(keywordStr);
        std::string kw;
        while (std::getline(ks, kw, ',')) {
            kw.erase(kw.find_last_not_of(" \n\r\t")+1);
            keywords.push_back(kw);
        }

        if (id.rfind("c_", 0) == 0) {
            int num = std::stoi(id.substr(2));
            compositeIdCounter = std::max(compositeIdCounter, num);
        }

        auto compFood = std::make_shared<CompositeFood>(id, name, keywords);
        std::stringstream cs(compStr);
        std::string comp;
        while (std::getline(cs, comp, ',')) {
            if (comp.empty()) continue;
            auto pos = comp.find(':');
            if (pos != std::string::npos) {
                std::string fid = comp.substr(0, pos);
                fid.erase(fid.find_last_not_of(" \n\r\t")+1);
                double servings = std::stod(comp.substr(pos + 1));
                auto component = findFoodById(fid);
                if (component) {
                    compFood->addComponent(component, servings);
                } else {
                    std::cerr << "Component food with ID " << fid
                              << " not found for composite food " << id << std::endl;
                }
            }
        }
        compositeFoods.push_back(compFood);
    }
    inComp.close();
}

void FoodDatabase::saveDatabase() const {
    std::ofstream outBasic(basicFoodsFile);
    if (!outBasic) {
        std::cerr << "Failed to open file for writing: " << basicFoodsFile << std::endl;
        return;
    }
    for (const auto& food : basicFoods) {
        auto basic = std::dynamic_pointer_cast<BasicFood>(food);
        if (basic) {
            outBasic << basic->getId() << ";" << basic->getName() << ";";

            auto keywords = basic->getKeywords();
            for (size_t i = 0; i < keywords.size(); ++i) {
                outBasic << keywords[i];
                if (i < keywords.size() - 1) outBasic << ",";
            }

            outBasic << ";" << basic->getCalories()
                     << ";" << basic->getProtein()
                     << ";" << basic->getCarbs()
                     << ";" << basic->getFat()
                     << ";" << basic->getSaturatedFat()
                     << ";" << basic->getFiber()
                     << ";" << basic->getVitamins()
                     << ";" << basic->getMinerals() << "\n";
        }
    }
    outBasic.close();

    std::ofstream outComp(compositeFoodsFile);
    if (!outComp) {
        std::cerr << "Failed to open file for writing: " << compositeFoodsFile << std::endl;
        return;
    }

    for (const auto& food : compositeFoods) {
        auto comp = std::dynamic_pointer_cast<CompositeFood>(food);
        if (comp) {
            outComp << comp->getId() << ";" << comp->getName() << ";";

            auto keywords = comp->getKeywords();
            for (size_t i = 0; i < keywords.size(); ++i) {
                outComp << keywords[i];
                if (i < keywords.size() - 1) outComp << ",";
            }

            outComp << ";";
            const auto& components = comp->getComponents();
            for (size_t i = 0; i < components.size(); ++i) {
                outComp << components[i].first->getId() << ":" << components[i].second;
                if (i < components.size() - 1) outComp << ",";
            }
            outComp << "\n";
        }
    }
    outComp.close();
}

void FoodDatabase::addBasicFood(const std::shared_ptr<Food>& food) {
    basicFoods.push_back(food);
}

void FoodDatabase::addCompositeFood(const std::shared_ptr<Food>& food) {
    compositeFoods.push_back(food);
}

const std::vector<std::shared_ptr<Food>>& FoodDatabase::getBasicFoods() const {
    return basicFoods;
}

const std::vector<std::shared_ptr<Food>>& FoodDatabase::getCompositeFoods() const {
    return compositeFoods;
}

std::string FoodDatabase::generateBasicFoodId() {
    return "b_" + std::to_string(++basicIdCounter);
}

std::string FoodDatabase::generateCompositeFoodId() {
    return "c_" + std::to_string(++compositeIdCounter);
}

std::vector<std::shared_ptr<Food>> FoodDatabase::searchFoodsByKeywords(const std::vector<std::string>& keywords, bool matchAll) const {
    std::vector<std::shared_ptr<Food>> results;

    auto matches = [&](const std::shared_ptr<Food>& food) {
        const auto& foodKeywords = food->getKeywords();
        int matchCount = 0;

        for (const auto& keyword : keywords) {
            // Convert search keyword to lowercase for case-insensitive comparison
            std::string keywordLower = keyword;
            std::transform(keywordLower.begin(), keywordLower.end(), keywordLower.begin(), 
                          [](unsigned char c){ return std::tolower(c); });
            
            // Check if any food keyword contains this search keyword
            bool foundMatch = false;
            for (const auto& foodKeyword : foodKeywords) {
                std::string foodKeywordLower = foodKeyword;
                std::transform(foodKeywordLower.begin(), foodKeywordLower.end(), foodKeywordLower.begin(), 
                              [](unsigned char c){ return std::tolower(c); });
                
                // Check for partial match (if one contains the other)
                if (foodKeywordLower.find(keywordLower) != std::string::npos || 
                    keywordLower.find(foodKeywordLower) != std::string::npos) {
                    foundMatch = true;
                    ++matchCount;
                    break;  // Found a match for this keyword, move to next one
                }
            }
        }

        return matchAll ? (matchCount == keywords.size()) : (matchCount > 0);
    };

    // Search in basic foods
    for (const auto& food : basicFoods) {
        if (matches(food)) {
            results.push_back(food);
        }
    }

    // Search in composite foods
    for (const auto& food : compositeFoods) {
        if (matches(food)) {
            results.push_back(food);
        }
    }

    return results;
}

std::vector<std::shared_ptr<Food>> FoodDatabase::getAllFoods() const {
    std::vector<std::shared_ptr<Food>> allFoods;
    allFoods.insert(allFoods.end(), basicFoods.begin(), basicFoods.end());
    allFoods.insert(allFoods.end(), compositeFoods.begin(), compositeFoods.end());
    return allFoods;
}
