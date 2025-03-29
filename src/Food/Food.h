#ifndef FOOD_H
#define FOOD_H

#include <string>
#include <vector>

class Food {
protected:
    std::string id;
    std::vector<std::string> keywords;
    public:
    std::string name;
    Food(const std::string& id, const std::string& name, const std::vector<std::string>& keywords);
    // virtual ~Food();
    std::string getId() const;
    std::string getName() const;
    std::vector<std::string> getKeywords() const;
    virtual double getCalories() const = 0;
    virtual void display() const = 0;
};

#endif // FOOD_H
