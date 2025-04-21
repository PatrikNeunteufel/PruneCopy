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

// Constructor
// Initializes the FileCopier with given options and optional log file
FileCopier::FileCopier(const PruneOptions& options, std::ofstream* logFile)
    : m_options(options), m_logFile(logFile) {
}

// Main execution method
// Iterates through sources and applies filtering, copying and logging logic
void FileCopier::execute() {
    for (const auto& src : m_options.sources) {
        for (auto it = fs::recursive_directory_iterator(src); it != fs::recursive_directory_iterator(); ++it) {
            const auto& entry = *it;

            // Skip excluded directories
            if (entry.is_directory() && PatternUtils::isExcludedDir(entry.path(), m_options.excludeDirs)) {
                LogManager::log(LogType::Skipped, entry.path().string(), m_logFile);
                it.disable_recursion_pending();
                continue;
            }

            // Only process regular files
            if (!entry.is_regular_file()) continue;

            const std::string filename = entry.path().filename().string();

            // Filter by allowed file types
            if (!m_options.typePatterns.empty() &&
                !PatternUtils::matchesPattern(filename, m_options.typePatterns)) {
                continue;
            }

            // Filter excluded files
            if (!m_options.excludeFilePatterns.empty() &&
                PatternUtils::matchesPattern(filename, m_options.excludeFilePatterns)) {
                LogManager::log(LogType::Skipped, entry.path().string(), m_logFile);
                continue;
            }

            // Copy file to all destinations
            for (const auto& dst : m_options.destinations) {
                fs::path targetFile = resolveTargetPath(src, entry.path(), dst);

                // File exists → resolve based on overwrite flags
                if (fs::exists(targetFile)) {
                    if (m_options.noOverwrite) {
                        continue; // skip silently without any prompt
                    }
                    else if (m_options.forceOverwrite) {
                        // Do nothing: overwrite directly
                    }
                    else if (m_options.flatten) {
                        if (!handleFlattenConflictPrompt(targetFile)) {
                            continue; // user skipped or canceled
                        }
                    }
                    else {
                        // Normal mode → prompt via classic handler
                        if (!handleOverwritePrompt(targetFile)) continue;
                    }
                }

                // Perform copy unless dry-run is active
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

// Resolves the destination path for a file, considering flatten options
fs::path FileCopier::resolveTargetPath(const fs::path& srcRoot, const fs::path& currentFile, const fs::path& destRoot) const {
    fs::path relPath = fs::relative(currentFile, srcRoot);

    // Flatten mode discards folder structure
    if (m_options.flatten) {
        std::string filename = currentFile.filename().string();

        // Add flattened suffix from folder structure
        if (m_options.flattenWithSuffix) {
            std::string prefix;
            if (!relPath.parent_path().empty()) {
                prefix = relPath.parent_path().string();
                std::replace(prefix.begin(), prefix.end(), '\\', '_');
                std::replace(prefix.begin(), prefix.end(), '/', '_');
                filename = prefix + "_" + filename;
            }
        }

        return destRoot / filename;
    }

    // Normal mode: preserve relative path under destination
    return destRoot / relPath;
}

// Prompts the user to confirm file overwrite unless forced globally
bool FileCopier::handleOverwritePrompt(const fs::path& targetFile) {
    while (true) {
		std::string msg = targetFile.string() + " already exists. [y]es / [n]o / [a]ll / [s]kip all / [c]ancel:";
        LogManager::logAlwaysToConsole(LogType::Conflict, msg);
        LogManager::log(LogType::Conflict, msg, m_logFile); 
        std::string input;
        std::getline(std::cin, input);
        LogManager::log(LogType::UserInput, "User entered: " + input, m_logFile);

        if (input.empty()) continue;

        char answer = std::tolower(input[0]);

        switch (answer) {
        case 'y': return true;
        case 'n': return false;
        case 'a':
            const_cast<PruneOptions&>(m_options).forceOverwrite = true;
            return true;
        case 's':
            const_cast<PruneOptions&>(m_options).noOverwrite = true;
            return false;
        case 'c':
            LogManager::log(LogType::Aborted, "Operation cancelled by user.", m_logFile);
            exit(0);
        default:
            continue;
        }
    }
}

// Prompts user for flatten conflict resolution with suggested name and extended options
bool FileCopier::handleFlattenConflictPrompt(std::filesystem::path& targetFile) {
    const fs::path suggested = resolveFileNameConflict(targetFile);

    // Auto-rename mode: no interaction
    if (m_options.flattenAutoRename) {
        targetFile = suggested;
        return true;
    }

    while (true) {
        std::string msg = targetFile.string() +
            " already exists. [o]verwrite / [r]ename / [s]kip / [c]ancel / [a]lways overwrite / [m] Auto-rename all\n" +
            "Suggested rename: " + suggested.filename().string();

        LogManager::logAlwaysToConsole(LogType::Conflict, msg);
        LogManager::log(LogType::Conflict, msg, m_logFile); 
        std::string input;
        std::getline(std::cin, input);
        LogManager::log(LogType::UserInput, "User entered: " + input, m_logFile);

        if (input.empty()) continue;
        char choice = std::tolower(input[0]);

        switch (choice) {
        case 'o':
            return true; // Overwrite
        case 'r': {
            LogManager::logAlwaysToConsole(LogType::Conflict, "Enter new filename (leave blank to use suggested):");
            std::string newName;
            std::getline(std::cin, newName);
            if (!newName.empty()) {
                fs::path candidate = targetFile.parent_path() / newName;
                // Recursively check if new name also exists
                while (fs::exists(candidate)) {
                    LogManager::logAlwaysToConsole(LogType::Conflict, candidate.string() + " also exists. Enter different name:");
                    std::getline(std::cin, newName);
                    if (newName.empty()) break; // fallback to suggestion
                    candidate = targetFile.parent_path() / newName;
                }
                targetFile = candidate;
            }
            else {
                targetFile = suggested;
            }
            return true;
        }
        case 's':
            return false; // Skip file
        case 'c':
            LogManager::log(LogType::Aborted, "Operation cancelled by user.", m_logFile);
            exit(0);
        case 'a':
            const_cast<PruneOptions&>(m_options).forceOverwrite = true;
            return true;
        case 'm':
            const_cast<PruneOptions&>(m_options).flattenAutoRename = true;
            targetFile = suggested;
            return true;
        default:
            continue; // Repeat on invalid input
        }
    }
}



// Logs a successful copy operation to log file and/or console
void FileCopier::logCopy(const fs::path& path) {
    LogManager::log(LogType::Copied, path.string(), m_logFile);
}

// Resolves name conflict by appending (1), (2), ... to filename until free
fs::path FileCopier::resolveFileNameConflict(const fs::path& originalPath) const {
    fs::path base = originalPath.parent_path();
    std::string stem = originalPath.stem().string();
    std::string ext = originalPath.extension().string();

    int counter = 1;
    fs::path newPath;
    do {
        newPath = base / fs::path(stem + "(" + std::to_string(counter) + ")" + ext);
        ++counter;
    } while (fs::exists(newPath));

    return newPath;
}

// Wrapper for directory exclusion check
bool FileCopier::isExcludedDir(const fs::path& dir, const std::vector<std::string>& excludeDirs) {
    return PatternUtils::isExcludedDir(dir, excludeDirs);
}

// Static legacy interface to perform copy operation
void FileCopier::copyFiltered(const PruneOptions& options, std::ofstream* logFile) {
    FileCopier copier(options, logFile);
    copier.execute();
}
