#include "DietProfile.h"
#include <iostream>
#include <limits> // Add this include for std::numeric_limits
#include <fstream>
#include <sstream>
#include <stdexcept>

// Initialize static member
map<int, function<unique_ptr<CalorieCalculationStrategy>()>> DietProfile::strategyFactories;

// Static initialization method
void DietProfile::initializeStrategyFactories() {
    if (strategyFactories.empty()) {
        // Register the default calculation methods
        registerCalculationMethod(1, []() { return make_unique<MifflinStJeorStrategy>(); });
        registerCalculationMethod(2, []() { return make_unique<HarrisBenedictStrategy>(); });
    }
}

// Static method to register new calculation methods
void DietProfile::registerCalculationMethod(int methodId, function<unique_ptr<CalorieCalculationStrategy>()> factory) {
    // Check if the methodId is already registered
    if (strategyFactories.find(methodId) != strategyFactories.end()) {
        std::cerr << "Error: Method ID " << methodId << " is already registered.\n";
        return;
    }
    // Register the new method
    strategyFactories[methodId] = factory;
}

DietProfile::DietProfile(const std::string& name, const std::string& gender, double height, int age, double weight, double activityLevel, int method)
    : name(name), gender(gender), height(height), age(age), weight(weight), activityLevel(activityLevel), method(method) {
    
    // Ensure strategy factories are initialized
    initializeStrategyFactories();
    
    // Initialize the appropriate strategy based on method
    setCalculationMethod(method);
}

void DietProfile::setCalculationMethod(int methodId) {
    // Check if the method exists in our map
    if (strategyFactories.find(methodId) != strategyFactories.end()) {
        method = methodId;
        calorieStrategy = strategyFactories[methodId]();
    } else {
        // Default to method 1 if invalid method is provided
        method = 1;
        calorieStrategy = strategyFactories[1]();
        std::cout << "Warning: Invalid calculation method specified. Using " 
                  << calorieStrategy->getName() << " by default.\n";
    }
}

double DietProfile::calculateTargetIntakeCalories() const {
    // Use the strategy to calculate BMR
    double bmr = calorieStrategy->calculateBMR(gender, weight, height, age);
    
    // Apply activity level multiplier
    return bmr * activityLevel;
}

void DietProfile::displayProfile() const {
    std::cout << "-------------------------------\n";
    std::cout << "\tDiet Profile\n";
    std::cout << "-------------------------------\n";

    std::cout << "Name: " << name << "\n";
    std::cout << "Gender: " << gender << "\n";
    std::cout << "Height: " << height << " cm\n";
    std::cout << "Age: " << age << " years\n";
    std::cout << "Weight: " << weight << " kg\n";
    std::cout << "Activity Level: " << activityLevel << "\n";
    
    // Get method name directly from the strategy object
    std::string methodName = calorieStrategy ? calorieStrategy->getName() : "Unknown";
    std::cout << "Method: " << methodName << "\n";
    std::cout << "Target Calories Intake: " << calculateTargetIntakeCalories() << " kcal\n";

    std::cout << "---------------------------------\n";
}

// Get all available calculation methods
std::map<int, std::string> DietProfile::getAvailableCalculationMethods() {
    // Make sure the factories are initialized
    initializeStrategyFactories();
    
    std::map<int, std::string> methodNames;
    
    // Get the name of each strategy
    for (const auto& pair : strategyFactories) {
        // Create a temporary instance of the strategy to get its name
        auto strategy = pair.second();
        methodNames[pair.first] = strategy->getName();
    }
    
    return methodNames;
}

void DietProfile::editProfile() {
    std::cout << "\n--- Current Profile Values ---\n";
    std::cout << "1. Name: " << name << "\n";
    std::cout << "2. Gender: " << gender << "\n";
    std::cout << "3. Height: " << height << " cm\n";
    std::cout << "4. Age: " << age << " years\n";
    std::cout << "5. Weight: " << weight << " kg\n";
    std::cout << "6. Activity Level: " << activityLevel << "\n";

    int fieldChoice;
    std::cout << "\nEnter the number of the field you want to edit (1-6, 0 to cancel): ";
    std::cin >> fieldChoice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (fieldChoice == 0) {
        std::cout << "Edit cancelled.\n";
        return;
    }

    switch (fieldChoice) {
        case 1: {
            std::string newName;
            std::cout << "Enter new name: ";
            std::getline(std::cin, newName);
            name = newName;
            break;
        }
        case 2: {
            std::string newGender;
            std::cout << "Enter new gender (male/female): ";
            std::getline(std::cin, newGender);
            gender = newGender;
            break;
        }
        case 3: {
            double newHeight;
            std::cout << "Enter new height (cm): ";
            std::cin >> newHeight;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            height = newHeight;
            break;
        }
        case 4: {
            int newAge;
            std::cout << "Enter new age: ";
            std::cin >> newAge;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            age = newAge;
            break;
        }
        case 5: {
            double newWeight;
            std::cout << "Enter new weight (kg): ";
            std::cin >> newWeight;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            weight = newWeight;
            break;
        }
        case 6: {
            double newActivityLevel;
            std::cout << "Enter new activity level:\n";
            std::cout << "1.2 = Sedentary (little to no exercise)\n";
            std::cout << "1.375 = Light activity (light exercise 1-3 days/week)\n";
            std::cout << "1.55 = Moderate activity (moderate exercise 3-5 days/week)\n";
            std::cout << "1.725 = Very active (hard exercise 6-7 days/week)\n";
            std::cout << "1.9 = Extra active (very hard exercise & physical job)\n";
            std::cout << "Enter value: ";
            std::cin >> newActivityLevel;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            activityLevel = newActivityLevel;
            break;
        }
        default:
            std::cout << "Invalid choice.\n";
            break;
    }

    std::cout << "Profile updated successfully!\n";
}

void DietProfile::loadProfileFromFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Error: Unable to open profile file: " + filePath);
    }

    std::string line;
    if (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string fields[7];
        for (int i = 0; i < 7; ++i) {
            if (!std::getline(iss, fields[i], ';')) {
                throw std::runtime_error("Error: Invalid profile file format.");
            }
        }

        name = fields[0];
        gender = fields[1];
        height = std::stod(fields[2]);
        age = std::stoi(fields[3]);
        weight = std::stod(fields[4]);
        activityLevel = std::stod(fields[5]);
        setCalculationMethod(std::stoi(fields[6]));
    } else {
        throw std::runtime_error("Error: Profile file is empty.");
    }
}
