#ifndef DAILY_LOG_H
#define DAILY_LOG_H

#include <string>
#include <vector>
#include <stack> // For undo functionality

// Simple structure for a log entry
struct LogEntry {
    std::string date;
    std::string foodId;
    double servings;
};

class DailyLog {
private:
    std::vector<LogEntry> entries;
    std::string logFile;
    std::stack<std::vector<LogEntry>> undoStack; // Stack to store undo states
public:
    DailyLog(const std::string& file);
    void loadLog();
    void saveLog() const;
    void addEntry(const LogEntry& entry);
    void removeEntry(int index);
    void displayLog() const;
    void undoLastAction(); // Undo the last action
    std::vector<LogEntry> getEntriesByDate(const std::string& date) const; // Get entries for a specific date
    void updateEntry(int index, const LogEntry& newEntry); // Update an entry
    
    // New method to return all entries
    const std::vector<LogEntry>& getEntries() const { return entries; }
};

#endif // DAILY_LOG_H
