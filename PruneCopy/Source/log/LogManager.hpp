/*****************************************************************//**
 * @file   LogManager.hpp
 * @brief  Provides logging utilities for console and file output
 * 
 * @author Patrik Neunteufel
 * @date   April 2025
 *********************************************************************/

#pragma once

#include <string>
#include <fstream>
#include <iostream>

#include "core/PruneOptions.hpp"

/**
 * @brief Represents the type of a log message, used for categorizing output
 */
enum class LogType {
    Info,         // General informational message
    Success,      // Successfully completed operation
    Copied,       // File was copied
    Overwritten,  // Existing file was overwritten
    Skipped,      // File or operation was intentionally skipped
    Deleted,      // File or folder was deleted
    Conflict,     // User intervention required (e.g. overwrite prompt)
    Aborted,      // Operation was aborted (by user or error)
    Error,        // Error encountered during execution
    UserInput,    // Log user-provided input (e.g. for overwrite prompt)
    Custom        // Custom or application-specific log entry
};



/**
 * @brief Represents the verbosity level for logging output
 */
class LogManager {
public:
	/**
	 * @brief Sets the verbosity level for console logging
	 * @param level The desired log level (e.g. LogLevel::Info)
	 */
    static void setConsoleLogLevel(LogLevel level);


	/**
	* @brief Sets the logfile for logging output
	* @param file The output file stream to use for logging
    */
    static void setLogFile(std::ofstream* file);

	/**
	* @brief enables or disables ANSI color codes for console output
	* @param mode The desired color mode (e.g. ColorMode::Auto)
    */
    static void enableAnsiColorsIfSupported(ColorMode mode);

    /** 
	 * @brief log the message to the console and the log file deopending on the logtype
     * 
	 * @param type log type (Info, Success, Copied, Overwritten, Skipped, Deleted, Conflict, Aborted, Error, UserInput)
	 * @param message the message to log
	 * @param stream the output stream to log to (default is std::cout)
     */
    static void log(LogType type, const std::string& message, std::ostream* stream = &std::cout);
    
	/** 
	 * @brief log the message to the console and the log file depending on the loglevel
	 * 
	 * @param level log level (All, Standard, Info, Warning, Error, None)
	 * @param message the message to log
	 * @param stream the output stream to log to (default is std::cout)
	 */
    static void log(LogLevel level, const std::string& message, std::ostream* stream = &std::cout);
    
    /** 
	 * @brief log the message to the console and the log file in any case
     * 
	 * @param type log type (Info, Success, Copied, Overwritten, Skipped, Deleted, Conflict, Aborted, Error, UserInput)
	 * @param msg the message to log
	 * @param logFile the output file stream to log to (default is nullptr)
     */
    static void logToAll(LogType type, const std::string& msg, std::ostream* logFile);
    
    /**
     * @brief log the message to the console and the log file in any case
     *
	 * @param level log level (All, Standard, Info, Warning, Error, None)
     * @param msg the message to log
     * @param logFile the output file stream to log to (default is nullptr)
     */
    static void logToAll(LogLevel level, const std::string& msg, std::ostream* logFile);
	
    /** 
	 * @brief log the message to the console
     * 
	 * @param type log type (Info, Success, Copied, Overwritten, Skipped, Deleted, Conflict, Aborted, Error, UserInput)
	 * @param message the message to log
     */
    static void logAlwaysToConsole(LogType type, const std::string& message);
    
    /** 
	 * @brief log the message to the console
     * 
	 * @param level log level (All, Standard, Info, Warning, Error, None)
	 * @param message the message to log
     */
    static void logAlwaysToConsole(LogLevel level, const std::string& message);
private:
    
    /** 
	 * @brief Convert a LogType to a LogLevel
     * 
	 * @param type The log type to convert
	 * @return The corresponding log level
     */
    static LogLevel logLevelFromType(LogType type);
    
    /** 
	 * @brief Convert a LogType to a string representation
     * 
	 * @param type The log type to convert
	 * @return The string representation of the log type
     */
    static std::string tagFromType(LogType type);
    
    /** 
	 * @brief Apply color to a log message based on its log level and tag
     * 
	 * @param level The log level of the message
	 * @param tag The tag associated with the log message
	 * @return The colored log message
     */
    static std::string applyColor(LogLevel level, const std::string& tag);
    
    /** 
	 * @brief Check if the log level is enabled for logging
     * 
	 * @param level The log level to check
	 * @return True if the log level is enabled, false otherwise
     */
    static bool shouldLog(LogLevel level);

	static inline LogLevel s_consoleLogLevel = LogLevel::Info;  ///< Current log level for console output
	static inline std::ofstream* s_logFile = nullptr;           ///< Output file stream for logging
	static inline bool s_ansiColorEnabled = false;              ///< Flag to indicate if ANSI color codes are enabled
};
