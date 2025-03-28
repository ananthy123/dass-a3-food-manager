#include "DailyLog.h"
#include <fstream>
#include <sstream>
#include <iostream>

DailyLog::DailyLog(const std::string& file) : logFile(file) {}

void DailyLog::loadLog() {
    std::ifstream inFile(logFile);
    if (!inFile) {
        std::cerr << "Failed to open daily log file: " << logFile << std::endl;
        return;
    }
    std::string line;
    while (std::getline(inFile, line)) {
        if (line.empty() || line[0] == '#') continue;
        std::stringstream ss(line);
        std::string date, foodId, servingsStr;
        std::getline(ss, date, ';');
        std::getline(ss, foodId, ';');
        std::getline(ss, servingsStr);
        LogEntry entry;
        entry.date = date;
        entry.foodId = foodId;
        entry.servings = std::stod(servingsStr);
        entries.push_back(entry);
    }
    inFile.close();
}

void DailyLog::saveLog() const {
    std::ofstream outFile(logFile);
    if (!outFile) {
        std::cerr << "Failed to open daily log file for writing: " << logFile << std::endl;
        return;
    }
    for (const auto& entry : entries) {
        outFile << entry.date << ";" << entry.foodId << ";" << entry.servings << "\n";
    }
    outFile.close();
}

void DailyLog::addEntry(const LogEntry& entry) {
    entries.push_back(entry);
}

void DailyLog::removeEntry(int index) {
    if (index >= 0 && index < entries.size()) {
        entries.erase(entries.begin() + index);
    }
}

void DailyLog::displayLog() const {
    for (const auto& entry : entries) {
        std::cout << "Date: " << entry.date 
                  << ", Food ID: " << entry.foodId 
                  << ", Servings: " << entry.servings << std::endl;
    }
}
