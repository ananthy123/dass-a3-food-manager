#ifndef DAILY_LOG_H
#define DAILY_LOG_H

#include <string>
#include <vector>
#include <memory>
#include <stdexcept>

namespace diet
{

    // Define a LogEntry structure with strong typing
    struct LogEntry
    {
        std::string date;
        std::string foodId;
        double servings;

        // Constructor for validation
        LogEntry(const std::string &date, const std::string &foodId, double servings);
    };

    class DailyLog
    {
    private:
        std::vector<LogEntry> entries;
        std::string logFile;

    public:
        // Helper method to validate date format
        static bool isValidDateFormat(const std::string &date);
        // Constructor that takes the log file path
        explicit DailyLog(const std::string &file);

        // File operations
        void loadLog();
        void saveLog() const;

        // Log entry management
        void addEntry(const LogEntry &entry);
        bool removeEntry(int index);

        // Display and query methods
        void displayLog() const;
        std::vector<LogEntry> getEntriesForDate(const std::string &date) const;

        // Accessor for all entries
        const std::vector<LogEntry> &getAllEntries() const;

        // Exception class for log errors
        class LogException : public std::runtime_error
        {
        public:
            explicit LogException(const std::string &message);
        };
    };

} // namespace diet

#endif // DAILY_LOG_H
