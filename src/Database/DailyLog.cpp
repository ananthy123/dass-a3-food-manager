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
    // Check if an entry with the same date and food ID already exists
    for (auto& existingEntry : entries) {
        if (existingEntry.date == entry.date && existingEntry.foodId == entry.foodId) {
            // Combine the servings
            undoStack.push(entries); // Save current state for undo
            existingEntry.servings += entry.servings;
            return;
        }
    }

    // If no matching entry is found, add a new entry
    undoStack.push(entries); // Save current state for undo
    entries.push_back(entry);
}

void DailyLog::removeEntry(int index) {
    if (index >= 0 && index < entries.size()) {
        undoStack.push(entries); // Save current state for undo
        entries.erase(entries.begin() + index);
    } else {
        std::cerr << "Invalid entry index.\n";
    }
}

void DailyLog::displayLog() const {
    for (const auto& entry : entries) {
        std::cout << "Date: " << entry.date 
                  << ", Food ID: " << entry.foodId 
                  << ", Servings: " << entry.servings << std::endl;
    }
}

void DailyLog::undoLastAction() {
    if (!undoStack.empty()) {
        entries = undoStack.top();
        undoStack.pop();
    } else {
        std::cerr << "No actions to undo.\n";
    }
}

std::vector<LogEntry> DailyLog::getEntriesByDate(const std::string& date) const {
    std::vector<LogEntry> result;
    for (const auto& entry : entries) {
        if (entry.date == date) {
            result.push_back(entry);
        }
    }
    return result;
}

void DailyLog::updateEntry(int index, const LogEntry& newEntry) {
    if (index >= 0 && index < entries.size()) {
        undoStack.push(entries); // Save current state for undo
        entries[index] = newEntry;
    } else {
        std::cerr << "Invalid entry index.\n";
    }
}
