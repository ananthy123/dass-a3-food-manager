#include "Food.h"

Food::Food(const std::string& id, const std::vector<std::string>& keywords)
    : id(id), keywords(keywords) {}

Food::~Food() {}

std::string Food::getId() const {
    return id;
}

std::vector<std::string> Food::getKeywords() const {
    return keywords;
}
