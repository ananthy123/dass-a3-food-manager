#include "FoodDatabase.h"
#include "../Food/BasicFood.h"
#include "../Food/CompositeFood.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

FoodDatabase::FoodDatabase(const std::string& compositeFile, 
                         const std::vector<std::shared_ptr<FoodSource>>& sources)
    : compositeFoodsFile(compositeFile), foodSources(sources) {}

void FoodDatabase::addFoodSource(const std::shared_ptr<FoodSource>& source) {
    foodSources.push_back(source);
}

void FoodDatabase::loadDatabase() {
    // Clear existing basic foods (in case we're reloading)
    basicFoods.clear();
    basicIdCounter = 0;
    
    // Load basic foods from all registered sources
    for (const auto& source : foodSources) {
        std::cout << "Loading foods from " << source->getSourceName() << "..." << std::endl;
        auto sourceFoods = source->loadBasicFoods();
        
        for (const auto& food : sourceFoods) {
            // Update the ID counter if needed
            if (food->getId().rfind("b_", 0) == 0) {
                try {
                    int num = std::stoi(food->getId().substr(2));
                    basicIdCounter = std::max(basicIdCounter, num);
                } catch (const std::exception& e) {
                    std::cerr << "Invalid ID format: " << food->getId() << std::endl;
                }
            }
            basicFoods.push_back(food);
        }
    }
    
    // Load composite foods
    compositeFoods.clear();
    compositeIdCounter = 0;
    
    std::ifstream inComp(compositeFoodsFile);
    if (!inComp) {
        std::cerr << "Failed to open composite foods file: " << compositeFoodsFile << std::endl;
        return;
    }
    
    std::string line;
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
            kw.erase(0, kw.find_first_not_of(" \n\r\t"));
            kw.erase(kw.find_last_not_of(" \n\r\t")+1);
            keywords.push_back(kw);
        }
        
        if (id.rfind("c_", 0) == 0) {
            try {
                int num = std::stoi(id.substr(2));
                compositeIdCounter = std::max(compositeIdCounter, num);
            } catch (const std::exception& e) {
                std::cerr << "Invalid ID format: " << id << std::endl;
            }
        }
        
        auto compFood = std::make_shared<CompositeFood>(id, name, keywords);
        std::stringstream cs(compStr);
        std::string comp;
        while (std::getline(cs, comp, ',')) {
            if (comp.empty()) continue;
            auto pos = comp.find(':');
            if (pos != std::string::npos) {
                std::string fid = comp.substr(0, pos);
                fid.erase(0, fid.find_first_not_of(" \n\r\t"));
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
    // Save basic foods to all writable sources
    for (const auto& source : foodSources) {
        bool success = source->saveBasicFoods(basicFoods);
        if (success) {
            std::cout << "Successfully saved basic foods to " << source->getSourceName() << std::endl;
        } else {
            std::cerr << "Failed to save basic foods to " << source->getSourceName() << std::endl;
        }
    }
    
    // Save composite foods
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

std::shared_ptr<Food> FoodDatabase::findFoodById(const std::string& id) const {
    for (const auto& food : basicFoods) {
        if (food->getId() == id) return food;
    }
    for (const auto& food : compositeFoods) {
        if (food->getId() == id) return food;
    }
    return nullptr;
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
    
    // Helper function to check if a food matches the given keywords
    auto matches = [&](const std::shared_ptr<Food>& food) {
        const auto& foodKeywords = food->getKeywords();
        if (keywords.empty()) return true;
        
        for (const auto& keyword : keywords) {
            bool found = false;
            for (const auto& foodKeyword : foodKeywords) {
                if (foodKeyword.find(keyword) != std::string::npos) {
                    found = true;
                    break;
                }
            }
            if (matchAll && !found) return false;
            if (!matchAll && found) return true;
        }
        return matchAll;
    };
    
    for (const auto& food : basicFoods) {
        if (matches(food)) {
            results.push_back(food);
        }
    }
    
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
