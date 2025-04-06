#include "FoodDatabase.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

namespace diet {

// DatabaseException implementation
FoodDatabase::DatabaseException::DatabaseException(const std::string& message)
    : std::runtime_error(message) {}

// Constructor
FoodDatabase::FoodDatabase(const std::string& basicFile, const std::string& compositeFile)
    : basicFoodsFile(basicFile), compositeFoodsFile(compositeFile) {}

// Helper method to parse keywords
std::vector<std::string> FoodDatabase::parseKeywords(const std::string& keywordStr) const {
    std::vector<std::string> keywords;
    std::stringstream ks(keywordStr);
    std::string kw;
    while (std::getline(ks, kw, ',')) {
        // Trim whitespace
        kw.erase(0, kw.find_first_not_of(" \t\r\n"));
        kw.erase(kw.find_last_not_of(" \t\r\n") + 1);
        if (!kw.empty()) {
            keywords.push_back(kw);
        }
    }
    return keywords;
}

std::shared_ptr<Food> FoodDatabase::findFoodById(const std::string& id) const {
    // Check basic foods first
    for (const auto& food : basicFoods) {
        if (food->getId() == id) {
            return food;
        }
    }
    
    // Then check composite foods
    for (const auto& food : compositeFoods) {
        if (food->getId() == id) {
            return food;
        }
    }
    
    return nullptr; // Not found
}

std::vector<std::shared_ptr<Food>> FoodDatabase::findFoodsByKeyword(const std::string& keyword) const {
    std::vector<std::shared_ptr<Food>> results;
    std::string lowerKeyword = keyword;
    std::transform(lowerKeyword.begin(), lowerKeyword.end(), lowerKeyword.begin(), ::tolower);
    
    // Lambda to check if a food's keywords contain our search term
    auto hasKeyword = [&lowerKeyword](const std::shared_ptr<Food>& food) {
        const auto& keywords = food->getKeywords();
        for (const auto& kw : keywords) {
            std::string lowerKw = kw;
            std::transform(lowerKw.begin(), lowerKw.end(), lowerKw.begin(), ::tolower);
            if (lowerKw.find(lowerKeyword) != std::string::npos) {
                return true;
            }
        }
        // Also check the name
        std::string lowerName = food->getName();
        std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);
        return lowerName.find(lowerKeyword) != std::string::npos;
    };
    
    // Search in basic foods
    for (const auto& food : basicFoods) {
        if (hasKeyword(food)) {
            results.push_back(food);
        }
    }
    
    // Search in composite foods
    for (const auto& food : compositeFoods) {
        if (hasKeyword(food)) {
            results.push_back(food);
        }
    }
    
    return results;
}

void FoodDatabase::loadDatabase() {
    // Clear existing data
    basicFoods.clear();
    compositeFoods.clear();
    
    // Load basic foods
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
            auto keywords = parseKeywords(keywordStr);
    
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

    // Load composite foods
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

        auto keywords = parseKeywords(keywordStr);

        if (id.rfind("c_", 0) == 0) {
            int num = std::stoi(id.substr(2));
            compositeIdCounter = std::max(compositeIdCounter, num);
        }

        auto compFood = std::make_shared<CompositeFood>(id, name, keywords);
        
        // Parse components (format: "foodId:servings,foodId:servings,...")
        std::stringstream cs(compStr);
        std::string comp;
        while (std::getline(cs, comp, ',')) {
            if (comp.empty()) continue;
            auto pos = comp.find(':');
            if (pos != std::string::npos) {
                std::string fid = comp.substr(0, pos);
                // Trim whitespace
                fid.erase(0, fid.find_first_not_of(" \t\r\n"));
                fid.erase(fid.find_last_not_of(" \t\r\n") + 1);
                
                try {
                    double servings = std::stod(comp.substr(pos + 1));
                    auto component = findFoodById(fid);
                    if (component) {
                        compFood->addComponent(component, servings);
                    } else {
                        std::cerr << "Component food with ID " << fid
                                << " not found for composite food " << id << std::endl;
                    }
                } catch (const std::exception& e) {
                    std::cerr << "Error parsing component servings: " << comp << " - " << e.what() << std::endl;
                }
            }
        }
        compositeFoods.push_back(compFood);
    }
    inComp.close();
}

void FoodDatabase::saveDatabase() const {
    // Save basic foods
    std::ofstream outBasic(basicFoodsFile);
    if (!outBasic) {
        throw DatabaseException("Failed to open file for writing: " + basicFoodsFile);
    }
    
    outBasic << "# Format: id;name;keywords;calories;protein;carbs;fat;saturatedFat;fiber;vitamins;minerals\n";
    
    for (const auto& food : basicFoods) {
        auto basic = std::dynamic_pointer_cast<BasicFood>(food);
        if (basic) {
            outBasic << basic->getId() << ";" << basic->getName() << ";";

            // Write keywords as comma-separated list
            const auto& keywords = basic->getKeywords();
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

    // Save composite foods
    std::ofstream outComp(compositeFoodsFile);
    if (!outComp) {
        throw DatabaseException("Failed to open file for writing: " + compositeFoodsFile);
    }
    
    outComp << "# Format: id;name;keywords;components\n";
    outComp << "# Components format: foodId:servings,foodId:servings,...\n";
    
    for (const auto& food : compositeFoods) {
        auto comp = std::dynamic_pointer_cast<CompositeFood>(food);
        if (comp) {
            outComp << comp->getId() << ";" << comp->getName() << ";";

            // Write keywords
            const auto& keywords = comp->getKeywords();
            for (size_t i = 0; i < keywords.size(); ++i) {
                outComp << keywords[i];
                if (i < keywords.size() - 1) outComp << ",";
            }

            outComp << ";";
            
            // Write components
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
    if (!food) {
        throw DatabaseException("Cannot add null food");
    }
    
    // Verify it's actually a BasicFood
    if (!std::dynamic_pointer_cast<BasicFood>(food)) {
        throw DatabaseException("Food is not a BasicFood instance");
    }
    
    // Check for ID conflicts
    if (findFoodById(food->getId())) {
        throw DatabaseException("A food with ID " + food->getId() + " already exists");
    }
    
    basicFoods.push_back(food);
}

void FoodDatabase::addCompositeFood(const std::shared_ptr<Food>& food) {
    if (!food) {
        throw DatabaseException("Cannot add null food");
    }
    
    // Verify it's actually a CompositeFood
    if (!std::dynamic_pointer_cast<CompositeFood>(food)) {
        throw DatabaseException("Food is not a CompositeFood instance");
    }
    
    // Check for ID conflicts
    if (findFoodById(food->getId())) {
        throw DatabaseException("A food with ID " + food->getId() + " already exists");
    }
    
    compositeFoods.push_back(food);
}

bool FoodDatabase::removeFood(const std::string& id) {
    // First check if this food is used as a component in any composite food
    for (const auto& food : compositeFoods) {
        auto comp = std::dynamic_pointer_cast<CompositeFood>(food);
        if (comp) {
            for (const auto& component : comp->getComponents()) {
                if (component.first->getId() == id) {
                    throw DatabaseException("Cannot remove food " + id + " because it is used in " + comp->getId());
                }
            }
        }
    }
    
    // Now try to remove from basic foods
    for (auto it = basicFoods.begin(); it != basicFoods.end(); ++it) {
        if ((*it)->getId() == id) {
            basicFoods.erase(it);
            return true;
        }
    }
    
    // Then try composite foods
    for (auto it = compositeFoods.begin(); it != compositeFoods.end(); ++it) {
        if ((*it)->getId() == id) {
            compositeFoods.erase(it);
            return true;
        }
    }
    
    return false; // Not found
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

} // namespace diet
