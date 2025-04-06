#include "DailyLog.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <regex>
#include <ctime>

namespace diet {

// LogException implementation
DailyLog::LogException::LogException(const std::string& message)
    : std::runtime_error(message) {}

// LogEntry implementation with validation
LogEntry::LogEntry(const std::string& date, const std::string& foodId, double servings)
    : date(date), foodId(foodId), servings(servings) {
    if (servings <= 0) {
        throw DailyLog::LogException("Servings must be a positive number");
    }
    if (foodId.empty()) {
        throw DailyLog::LogException("Food ID cannot be empty");
    }
    if (!DailyLog::isValidDateFormat(date)) {
        throw DailyLog::LogException("Invalid date format. Use YYYY-MM-DD");
    }
}

// DailyLog implementation
DailyLog::DailyLog(const std::string& file) : logFile(file) {}

bool DailyLog::isValidDateFormat(const std::string& date) {
    std::regex datePattern(R"(\d{4}-\d{2}-\d{2})");
    return std::regex_match(date, datePattern);
}

void DailyLog::loadLog() {
    std::ifstream inFile(logFile);
    if (!inFile) {
        std::cerr << "Warning: Could not open log file: " << logFile << std::endl;
        return;
    }
    
    entries.clear();
    std::string line;
    
    while (std::getline(inFile, line)) {
        if (line.empty() || line[0] == '#') continue;
        
        std::stringstream ss(line);
        std::string dateStr, foodIdStr, servingsStr;
        
        if (std::getline(ss, dateStr, ';') && 
            std::getline(ss, foodIdStr, ';') && 
            std::getline(ss, servingsStr)) {
            
            try {
                double servings = std::stod(servingsStr);
                entries.emplace_back(dateStr, foodIdStr, servings);
            } catch (const std::exception& e) {
                std::cerr << "Error parsing log entry: " << line << " - " << e.what() << std::endl;
            }
        } else {
            std::cerr << "Skipping invalid log entry: " << line << std::endl;
        }
    }
    
    inFile.close();
}

void DailyLog::saveLog() const {
    std::ofstream outFile(logFile);
    if (!outFile) {
        throw LogException("Failed to open daily log file for writing: " + logFile);
    }
    
    outFile << "# Format: date;foodId;servings\n";
    time_t now = std::time(nullptr);
    outFile << "# Generated on: " << std::put_time(std::localtime(&now), 
                                                 "%Y-%m-%d %H:%M:%S") << "\n";
    
    for (const auto& entry : entries) {
        outFile << entry.date << ";" << entry.foodId << ";" << entry.servings << "\n";
    }
    
    outFile.close();
}

void DailyLog::addEntry(const LogEntry& entry) {
    entries.push_back(entry);
}

bool DailyLog::removeEntry(int index) {
    if (index >= 0 && index < static_cast<int>(entries.size())) {
        entries.erase(entries.begin() + index);
        return true;
    }
    return false;
}

void DailyLog::displayLog() const {
    if (entries.empty()) {
        std::cout << "No log entries found.\n";
        return;
    }
    
    std::cout << "======= Food Consumption Log =======\n";
    std::cout << std::left << std::setw(12) << "Date" 
              << std::setw(10) << "Food ID" 
              << std::setw(10) << "Servings" << "\n";
    std::cout << "-----------------------------------\n";
    
    for (size_t i = 0; i < entries.size(); ++i) {
        const auto& entry = entries[i];
        std::cout << "[" << i << "] " 
                  << std::left << std::setw(12) << entry.date 
                  << std::setw(10) << entry.foodId 
                  << std::setw(10) << entry.servings << "\n";
    }
    std::cout << "===================================\n";
}

std::vector<LogEntry> DailyLog::getEntriesForDate(const std::string& date) const {
    std::vector<LogEntry> result;
    for (const auto& entry : entries) {
        if (entry.date == date) {
            result.push_back(entry);
        }
    }
    return result;
}

const std::vector<LogEntry>& DailyLog::getAllEntries() const {
    return entries;
}

} // namespace diet
