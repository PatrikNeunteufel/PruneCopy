/*****************************************************************//**
 * @file   LogManager.cpp
 * @brief  Implements logging functionality for PruneCopy
 *         with optional colored output on supported terminals
 * 
 * @author Patrik Neunteufel
 * @date   April 2025
 *********************************************************************/


#include "log/LogManager.hpp"

#include <map>
#include <unordered_map>
#include <sstream>
#include <iomanip>
#include <algorithm>


#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#endif

 // Sets the minimum log level for console output
void LogManager::setConsoleLogLevel(LogLevel level) {
    s_consoleLogLevel = level;
}

// Sets the output file stream for logging to a file
void LogManager::setLogFile(std::ofstream* file) {
    s_logFile = file;
}

// Enables ANSI color support for console output, if supported and allowed by the given mode
void LogManager::enableAnsiColorsIfSupported(ColorMode mode) {
    // Explicitly disable color if set to Never
    if (mode == ColorMode::Never) {
        s_ansiColorEnabled = false;
        return;
    }

#ifdef _WIN32
    // Attempt to enable virtual terminal processing (for ANSI colors) on Windows
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) return;

    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode)) return;

    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    if (SetConsoleMode(hOut, dwMode)) {
        s_ansiColorEnabled = true;
    }
#else
    // Assume ANSI support on non-Windows systems (Linux/macOS)
    s_ansiColorEnabled = true;
#endif

    // Force enable if explicitly requested (overrides detection)
    if (mode == ColorMode::Always) {
        s_ansiColorEnabled = true;
    }
}

// Returns a map that associates each LogType with its string label for output formatting
static const std::unordered_map<LogType, std::string>& getLogLabelMap() {
    static const std::unordered_map<LogType, std::string> labelMap = {
        { LogType::Info,        "Info" },
        { LogType::Success,     "Success" },
        { LogType::Copied,      "Copied" },
        { LogType::Overwritten, "Overwritten" },
        { LogType::Skipped,     "Skipped" },
        { LogType::Deleted,     "Deleted" },
        { LogType::Conflict,    "Conflict" },
        { LogType::Aborted,     "Aborted" },
        { LogType::Error,       "Error" },
        { LogType::UserInput,   "UserInput" },
        { LogType::Custom,      "Log" }
    };
    return labelMap;
}


// Returns the length of the longest log label, used for output alignment
static size_t getMaxLogLabelLength() {
    const auto& labels = getLogLabelMap();
    size_t maxLen = 0;
    for (const auto& [key, label] : labels) {
        maxLen = std::max(maxLen, label.size());
    }
    return maxLen;
}


// Maps a LogType to its corresponding LogLevel for filtering purposes
LogLevel LogManager::logLevelFromType(LogType type) {
    static const std::unordered_map<LogType, LogLevel> typeToLevel = {
        { LogType::Info,        LogLevel::Info },       // Generic information
        { LogType::Success,     LogLevel::Standard },   // Successful operation messages
        { LogType::Copied,      LogLevel::Standard },   // File copied
        { LogType::Overwritten, LogLevel::Standard },   // File overwritten
        { LogType::Skipped,     LogLevel::Standard },   // File skipped
        { LogType::Deleted,     LogLevel::Standard },   // File deleted
        { LogType::Conflict,    LogLevel::Warning },    // User decision required
        { LogType::Aborted,     LogLevel::Standard },   // Operation aborted
        { LogType::Error,       LogLevel::Error },      // Critical error
        { LogType::UserInput,   LogLevel::Info },       // Log of user-entered input
        { LogType::Custom,      LogLevel::Info }        // User-defined/custom messages
    };

    auto it = typeToLevel.find(type);
    return it != typeToLevel.end() ? it->second : LogLevel::Info; // Fallback to Info if unknown
}


// Returns a left-aligned tag string for a given LogType, e.g. "[Copied   ]"
std::string LogManager::tagFromType(LogType type) {
    const auto& map = getLogLabelMap();
    std::string label = map.contains(type) ? map.at(type) : "Info";

    std::ostringstream oss;
    oss << "[" << std::left << std::setw(getMaxLogLabelLength()) << label << "]";
    return oss.str();
}

// Applies ANSI color codes to the log tag based on the log level (if enabled)
std::string LogManager::applyColor(LogLevel level, const std::string& tag) {
    if (!s_ansiColorEnabled) return tag;

    switch (level) {
    case LogLevel::Error:    return "\033[1;31m" + tag + "\033[0m"; // red
    case LogLevel::Warning:  return "\033[1;33m" + tag + "\033[0m"; // yellow
    case LogLevel::Standard: return "\033[1;34m" + tag + "\033[0m"; // blue
    case LogLevel::Info:     return "\033[1;36m" + tag + "\033[0m"; // cyan
    default:                 return tag; // No color for LogLevel::None or others
    }
}


// Determines whether a message at the given level should be logged to the console
bool LogManager::shouldLog(LogLevel level) {
    return level >= s_consoleLogLevel;
}

// Logs a message using a LogType, applying color for console output.
// Console output is filtered by log level; log file always receives full output.
void LogManager::log(LogType type, const std::string& message, std::ostream* stream) {
    LogLevel level = logLevelFromType(type);

    std::string rawTag = tagFromType(type);              // Aligned tag (e.g., [Copied   ])
    std::string coloredTag = applyColor(level, rawTag);  // Apply color for terminal output

    // Only show on console if log level permits
    if (stream && stream == &std::cout && shouldLog(level)) {
        *stream << coloredTag << " " << message << std::endl;
    }

    // Always write to log file if enabled
    if (s_logFile && s_logFile->is_open()) {
        *s_logFile << rawTag << " " << message << std::endl;
    }
}



// Logs a message using a raw LogLevel (fallback or custom cases).
// Console output is filtered by configured log level; file output is always written.
void LogManager::log(LogLevel level, const std::string& message, std::ostream* stream) {
    std::string label;
    switch (level) {
    case LogLevel::Info:     label = "Info"; break;
    case LogLevel::Warning:  label = "Warning"; break;
    case LogLevel::Error:    label = "Error"; break;
    case LogLevel::Standard: label = "Standard"; break;
    default:                 label = "Log"; break;
    }

    std::ostringstream oss;
    oss << "[" << std::left << std::setw(getMaxLogLabelLength()) << label << "]";
    std::string rawTag = oss.str();
    std::string coloredTag = applyColor(level, rawTag);

    // Log to console if level is allowed
    if (stream && stream == &std::cout && shouldLog(level)) {
        *stream << coloredTag << " " << message << std::endl;
    }

    // Always log to file
    if (s_logFile && s_logFile->is_open()) {
        *s_logFile << rawTag << " " << message << std::endl;
    }
}


// Logs a message directly to the console, regardless of current log level (using LogType)
void LogManager::logAlwaysToConsole(LogType type, const std::string& message) {
    LogLevel level = logLevelFromType(type);
    std::string tag = tagFromType(type);
    std::string colored = applyColor(level, tag);
    std::cout << colored << " " << message << std::endl;
}

// Logs a message directly to the console, regardless of log level (using LogLevel)
void LogManager::logAlwaysToConsole(LogLevel level, const std::string& message) {
    std::string tag = tagFromType(LogType::Info); // Default label for level-based logging
    std::string colored = applyColor(level, tag);
    std::cout << colored << " " << message << std::endl;
}

