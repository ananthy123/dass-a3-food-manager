#include "Food.h"

namespace diet {

Food::Food(const std::string& id, const std::string& name, const std::vector<std::string>& keywords)
    : id(id), name(name), keywords(keywords) {}

std::string Food::getId() const { return id; }
std::string Food::getName() const { return name; }
std::vector<std::string> Food::getKeywords() const { return keywords; }

} // namespace diet

