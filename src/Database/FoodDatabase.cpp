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
    // Load basic foods from text file
    // Debug 
    std::cout << "Loading basic foods from: " << basicFoodsFile << std::endl;

    std::ifstream inBasic(basicFoodsFile);
    if (!inBasic) {
        std::cerr << "Failed to open basic foods file: " << basicFoodsFile << std::endl;
        return;
    }
    std::string line;
    while (std::getline(inBasic, line)) {
        if (line.empty() || line[0] == '#') continue;
        std::stringstream ss(line);
        std::string id, keywordStr, calStr;
        std::getline(ss, id, ';');
        std::getline(ss, keywordStr, ';');
        std::getline(ss, calStr);
        // Trim trailing whitespace from id
        id.erase(id.find_last_not_of(" \n\r\t")+1);
        // Parse keywords
        std::vector<std::string> keywords;
        std::stringstream ks(keywordStr);
        std::string kw;
        while (std::getline(ks, kw, ',')) {
            kw.erase(kw.find_last_not_of(" \n\r\t")+1);
            keywords.push_back(kw);
        }
        double calories = std::stod(calStr);
        basicFoods.push_back(std::make_shared<BasicFood>(id, keywords, calories));
    }
    inBasic.close();

    // Load composite foods from text file
    std::ifstream inComp(compositeFoodsFile);
    if (!inComp) {
        std::cerr << "Failed to open composite foods file: " << compositeFoodsFile << std::endl;
        return;
    }
    while (std::getline(inComp, line)) {
        if (line.empty() || line[0] == '#') continue;
        std::stringstream ss(line);
        std::string id, keywordStr, compStr;
        std::getline(ss, id, ';');
        std::getline(ss, keywordStr, ';');
        std::getline(ss, compStr);
        id.erase(id.find_last_not_of(" \n\r\t")+1);
        // Parse keywords
        std::vector<std::string> keywords;
        std::stringstream ks(keywordStr);
        std::string kw;
        while (std::getline(ks, kw, ',')) {
            kw.erase(kw.find_last_not_of(" \n\r\t")+1);
            keywords.push_back(kw);
        }
        auto compFood = std::make_shared<CompositeFood>(id, keywords);
        // Parse components: "food_id:servings,food_id:servings,..."
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
    // Save basic foods to text file
    std::ofstream outBasic(basicFoodsFile);
    if (!outBasic) {
        std::cerr << "Failed to open file for writing: " << basicFoodsFile << std::endl;
        return;
    }
    for (const auto& food : basicFoods) {
        auto basic = std::dynamic_pointer_cast<BasicFood>(food);
        if (basic) {
            outBasic << basic->getId() << ";";
            auto keywords = basic->getKeywords();
            for (size_t i = 0; i < keywords.size(); ++i) {
                outBasic << keywords[i];
                if (i < keywords.size() - 1)
                    outBasic << ",";
            }
            outBasic << ";" << basic->getCalories() << "\n";
        }
    }
    outBasic.close();

    // Save composite foods to text file
    std::ofstream outComp(compositeFoodsFile);
    if (!outComp) {
        std::cerr << "Failed to open file for writing: " << compositeFoodsFile << std::endl;
        return;
    }
    for (const auto& food : compositeFoods) {
        auto comp = std::dynamic_pointer_cast<CompositeFood>(food);
        if (comp) {
            outComp << comp->getId() << ";";
            auto keywords = comp->getKeywords();
            for (size_t i = 0; i < keywords.size(); ++i) {
                outComp << keywords[i];
                if (i < keywords.size() - 1)
                    outComp << ",";
            }
            outComp << ";";
            const auto& components = comp->getComponents();
            for (size_t i = 0; i < components.size(); ++i) {
                outComp << components[i].first->getId() << ":" << components[i].second;
                if (i < components.size() - 1)
                    outComp << ",";
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
