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
 * @brief Supported parallelization strategies.
 */
enum class ParallelMode {
    None,
    Async,
    Thread,
    OpenMP
};

enum class LogLevel {
    Info,
    Warning,
    Error
};

enum class ColorMode {
    Auto,
    Always,
    Never
};
/**
 * @brief Central configuration for the PruneCopy application.
 */
struct PruneOptions {
    fs::path src;
    fs::path dst;
    std::vector<std::string> types;
    std::vector<std::string> excludeDirs;
    std::vector<std::string> excludeFiles;
    std::vector<std::regex> typePatterns;
    std::vector<std::regex> excludeFilePatterns;
    fs::path logDir;
    bool enableLogging = false;
    bool openLog = false;
    bool quiet = false;
    bool deleteTargetFirst = false;
    bool dryRun = false;
    bool noOverwrite = false;
    bool forceOverwrite = false;
    bool flatten = false;
    bool flattenWithSuffix = false;
    ParallelMode parallelMode = ParallelMode::None;
    ColorMode colorMode = ColorMode::Auto;
};
