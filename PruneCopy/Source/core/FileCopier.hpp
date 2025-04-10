/*****************************************************************//**
 * @file   FileCopier.hpp
 * @brief  Provides file filtering and copying logic for PruneCopy
 * 
 * @author Patrik Neunteufel
 * @date   April 2025
 *********************************************************************/

#pragma once
#include <string>
#include <vector>
#include <regex>
#include <fstream>
#include <filesystem>
#include "cli/ArgumentParser.hpp"


namespace FileCopier {
    bool isExcludedDir(const fs::path& dir, const std::vector<std::string>& excludeDirs);
    /**
     * @brief Recursively copies files that match inclusion/exclusion patterns.
     *
     *
     * @param typePatterns Regex patterns for file types to include (e.g., *.h, *.hpp).
     * @param excludeDirs Directories to exclude from copying.
     * @param optrions Options for copying behavior (e.g., dry run, overwrite).
     * @param logFile Optional log file stream for logging operations.
     */
    void copyFiltered(
        const PruneOptions& options,
        std::ofstream* logFile = nullptr
    );

} // namespace FileCopier