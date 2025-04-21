/*****************************************************************//**
 * @file   FileCopier.cpp
 * @brief  Implements file filtering and copying logic for PruneCopy
 * 
 * @author Patrik Neunteufel
 * @date   April 2025
 *********************************************************************/

#include "core/FileCopier.hpp"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <algorithm>

#include "util/PatternUtils.hpp"
#include "log/LogManager.hpp"

namespace fs = std::filesystem;

// Constructs a FileCopier with given options and optional log file output
FileCopier::FileCopier(const PruneOptions& options, std::ofstream* logFile)
    : m_options(options), m_logFile(logFile) {
}


// Executes the file copying operation based on configured source, destination, and filters
void FileCopier::execute() {
    // Iterate over all source directories
    for (const auto& src : m_options.sources) {
        for (auto it = fs::recursive_directory_iterator(src); it != fs::recursive_directory_iterator(); ++it) {
            const auto& entry = *it;

            // Skip excluded directories (and prevent descent into them)
            if (entry.is_directory() && PatternUtils::isExcludedDir(entry.path(), m_options.excludeDirs)) {
                LogManager::log(LogType::Skipped, entry.path().string(), m_logFile);
                it.disable_recursion_pending();
                continue;
            }

            // Process only regular files
            if (!entry.is_regular_file()) continue;

            const std::string filename = entry.path().filename().string();

            // Skip non-matching types
            if (!m_options.typePatterns.empty() &&
                !PatternUtils::matchesPattern(filename, m_options.typePatterns)) {
                continue;
            }

            // Skip excluded files (and log once)
            if (!m_options.excludeFilePatterns.empty() &&
                PatternUtils::matchesPattern(filename, m_options.excludeFilePatterns)) {
                LogManager::log(LogType::Skipped, entry.path().string(), m_logFile);
                continue;
            }

            // Copy to all destinations
            for (const auto& dst : m_options.destinations) {
                fs::path targetFile = resolveTargetPath(src, entry.path(), dst);

                // Overwrite checks
                if (fs::exists(targetFile)) {
                    if (m_options.noOverwrite) continue;
                    if (!m_options.forceOverwrite && !handleOverwritePrompt(targetFile)) continue;
                }

                // Copy (unless dry-run)
                if (!m_options.dryRun) {
                    fs::create_directories(targetFile.parent_path());
                    fs::copy_file(entry.path(), targetFile, fs::copy_options::overwrite_existing);
                }

                // Log successful copy
                LogManager::log(LogType::Copied, targetFile.string(), m_logFile);
            }
        }
    }
}



// Resolves the final destination path for a given file, based on options and mode
fs::path FileCopier::resolveTargetPath(const fs::path& srcRoot, const fs::path& currentFile, const fs::path& destRoot) const {
    // Compute relative path from source root to current file
    fs::path relPath = fs::relative(currentFile, srcRoot);

    // If flattening is enabled, discard directory structure
    if (m_options.flatten) {
        std::string filename = currentFile.filename().string();

        if (m_options.flattenWithSuffix) {
            // Build prefix from original folder structure to avoid name collisions
            std::string prefix = relPath.parent_path().string();
            std::replace(prefix.begin(), prefix.end(), '\\', '_');
            std::replace(prefix.begin(), prefix.end(), '/', '_');
            filename = prefix + "_" + filename;
        }

        // Place all files directly into the destination folder
        return destRoot / filename;
    }

    // Preserve original directory structure under destination root
    return destRoot / relPath;
}


// Prompts the user how to handle an existing file when overwrite is not forced
bool FileCopier::handleOverwritePrompt(const fs::path& targetFile) {
    while (true) {
        // Display interactive conflict prompt
        LogManager::logAlwaysToConsole(LogType::Conflict, targetFile.string() + " - overwrite? [y]es / [n]o / [a]ll / [s]kip all / [c]ancel:");

        std::string input;
        std::getline(std::cin, input);
        LogManager::log(LogType::UserInput, "User entered: " + input, m_logFile);

        if (input.empty()) continue;

        // Convert first character to lowercase for comparison
        char answer = std::tolower(input[0]);

        switch (answer) {
        case 'y': return true;  // Overwrite this file
        case 'n': return false; // Skip this file
        case 'a':
            // Apply overwrite to all future conflicts
            const_cast<PruneOptions&>(m_options).forceOverwrite = true;
            return true;
        case 's':
            // Skip all future overwrites
            const_cast<PruneOptions&>(m_options).noOverwrite = true;
            return false;
        case 'c':
            // Abort the entire operation immediately
            LogManager::log(LogType::Aborted, "Operation cancelled by user.", m_logFile);
            exit(0);
        default:
            continue; // Invalid input → repeat prompt
        }
    }
}


// Logs a successful file copy operation to console and/or file
void FileCopier::logCopy(const fs::path& path) {
    //LogManager::log(LogType::Copied, path.string(), m_logFile); // legacy
    LogManager::log(LogType::Copied, path.string(), m_logFile);
}

// Wrapper for exclusion check (used for compatibility or testing) LEGACY
bool FileCopier::isExcludedDir(const fs::path& dir, const std::vector<std::string>& excludeDirs) {
    return PatternUtils::isExcludedDir(dir, excludeDirs);
}

// Static helper to execute a filtered copy operation using a FileCopier instance LEGACY
void FileCopier::copyFiltered(const PruneOptions& options, std::ofstream* logFile) {
    FileCopier copier(options, logFile);
    copier.execute();
}

