/*****************************************************************//**
 * @file   PruneOptions.hpp
 * @brief  
 * 
 * @author Patrik Neunteufel
 * @date   April 2025
 *********************************************************************/

#pragma once
#include <string>
#include <vector>
#include <filesystem>
#include <regex>

namespace fs = std::filesystem;
/**
 * @brief Defines the available parallelization strategies for file copying
 */
enum class ParallelMode {
    None,     // No parallelization (default, single-threaded)
    Async,    // Use async/futures (std::async) for parallel copy tasks
    Thread,   // Use manual thread management (e.g. std::thread or thread pool)
    OpenMP    // Use OpenMP parallel for-loops (requires compiler support)
};

/**
 * @brief Defines the verbosity level for console and file logging
 */
enum class LogLevel {
    All = 0,        // Show all messages (info, success, warning, error, etc.)
    Standard = 1,   // Show operational messages (copied, skipped, overwritten, etc.)
    Info = 2,       // Show informational messages only (e.g., version, settings)
    Warning = 3,    // Show warnings and errors only
    Error = 4,      // Show only critical error messages
    None = 5        // Suppress all log output
};

/**
 * @brief Defines the colorization mode for console output
 */
 // Controls whether colored console output is enabled
enum class ColorMode {
    Auto,    // Enable colors only if supported by the terminal (default)
    Always,  // Force color output, even if terminal support is uncertain
    Never    // Disable all color output (monochrome)
};

/**
 * @brief Central configuration for the PruneCopy application.
 * Represents all parsed CLI options and runtime configuration for a PruneCopy operation
 */
struct PruneOptions {
    std::vector<fs::path> sources;               // Source directories to copy from
    std::vector<fs::path> destinations;          // Destination directories to copy to
    std::vector<std::string> types;              // File patterns to include (e.g. *.h, *.cpp)
    std::vector<std::string> excludeDirs;        // Directory names to exclude
    std::vector<std::string> excludeFiles;       // File patterns to exclude (e.g. *Impl.hpp)

    std::vector<std::regex> typePatterns;        // Compiled regex filters for include patterns
    std::vector<std::regex> excludeFilePatterns; // Compiled regex filters for exclude patterns

    fs::path logDir;                             // Directory for writing log files
    bool enableLogging = false;                  // Whether to write logs to file
    bool openLog = false;                        // Whether to open the log file after copying

    bool quiet = false;                          // Deprecated: suppress output (use LogLevel::None instead)

    bool deleteTargetFirst = false;              // Whether to delete destination directory before copying
    bool dryRun = false;                         // Simulate copying without touching the filesystem
    bool noOverwrite = false;                    // Skip files that already exist
    bool forceOverwrite = false;                 // Overwrite files without prompting

    bool flatten = false;                        // Copy all files into a single target folder
    bool flattenWithSuffix = false;              // Flatten with path-based filename suffixes to prevent conflicts

    ParallelMode parallelMode = ParallelMode::None; // Selected parallelization strategy
    ColorMode colorMode = ColorMode::Auto;          // Console color output setting
    LogLevel logLevel = LogLevel::Info;             // Log verbosity level
};
