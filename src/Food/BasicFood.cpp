#include "BasicFood.h"
#include <iostream>
#include <iomanip>

namespace diet {

BasicFood::BasicFood(const std::string& id, const std::string& name,
                     const std::vector<std::string>& keywords, double calories,
                     double protein, double carbs, double fat,
                     double saturatedFat, double fiber,
                     const std::string& vitamins, const std::string& minerals)
    : Food(id, name, keywords),
      calories(calories),
      protein(protein),
      carbs(carbs),
      fat(fat),
      saturatedFat(saturatedFat),
      fiber(fiber),
      vitamins(vitamins),
      minerals(minerals) {}

double BasicFood::getCalories() const { return calories; }
double BasicFood::getProtein() const { return protein; }
double BasicFood::getCarbs() const { return carbs; }
double BasicFood::getFat() const { return fat; }
double BasicFood::getSaturatedFat() const { return saturatedFat; }
double BasicFood::getFiber() const { return fiber; }
std::string BasicFood::getVitamins() const { return vitamins; }
std::string BasicFood::getMinerals() const { return minerals; }

void BasicFood::display() const {
    std::cout << "BasicFood: " << name << " (" << id << ")\n"
              << "  Calories: " << calories << " kcal\n"
              << "  Protein: " << std::fixed << std::setprecision(1) << protein << "g, "
              << "Carbs: " << carbs << "g, "
              << "Fat: " << fat << "g, "
              << "Sat Fat: " << saturatedFat << "g, "
              << "Fiber: " << fiber << "g\n"
              << "  Vitamins: " << vitamins << ", Minerals: " << minerals << "\n";
}

} // namespace diet
