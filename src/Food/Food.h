#ifndef FOOD_H
#define FOOD_H

#include <string>
#include <vector>

class Food {
protected:
    std::string id;
    std::vector<std::string> keywords;
public:
    Food(const std::string& id, const std::vector<std::string>& keywords);
    virtual ~Food();
    std::string getId() const;
    std::vector<std::string> getKeywords() const;
    virtual double getCalories() const = 0;
    virtual void display() const = 0;
};

#endif // FOOD_H
