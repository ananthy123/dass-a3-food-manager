#ifndef FOOD_H
#define FOOD_H

#include <string>
#include <vector>

namespace diet {

class Food {
protected:
    std::string id;
    std::string name;
    std::vector<std::string> keywords;

public:
    // Constructor with member initialization list
    Food(const std::string& id, const std::string& name, const std::vector<std::string>& keywords);
    
    // Virtual destructor for proper polymorphic deletion
    virtual ~Food() = default;
    
    // Getters with const correctness
    std::string getId() const;
    std::string getName() const;
    std::vector<std::string> getKeywords() const;
    
    // Pure virtual methods for the interface
    virtual double getCalories() const = 0;
    virtual void display() const = 0;
};

} // namespace diet

#endif // FOOD_H
