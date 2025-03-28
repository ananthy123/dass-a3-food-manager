#ifndef DAILY_LOG_H
#define DAILY_LOG_H

#include <string>
#include <vector>

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
public:
    DailyLog(const std::string& file);
    void loadLog();
    void saveLog() const;
    void addEntry(const LogEntry& entry);
    void removeEntry(int index);
    void displayLog() const;
};

#endif // DAILY_LOG_H
