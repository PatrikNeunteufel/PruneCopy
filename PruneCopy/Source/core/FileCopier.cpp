/*****************************************************************//**
 * @file   FileCopier.cpp
 * @brief  Implements file filtering and copying logic for PruneCopy
 * 
 * @author Patrik Neunteufel
 * @date   April 2025
 *********************************************************************/

#include "core/FileCopier.hpp"
#include "util/PatternUtils.hpp"
#include "log/LogManager.hpp"

#include <iostream>

namespace fs = std::filesystem;

namespace FileCopier {

    static bool isExcludedDir(const fs::path& dir, const std::vector<std::string>& excludeDirs) {
        for (const auto& ex : excludeDirs) {
            if (dir.filename().string().find(ex) != std::string::npos) {
                return true;
            }
        }
        return false;
    }

    void copyFiltered(
        const std::vector<std::regex>& typePatterns,
        const std::vector<std::regex>& excludeFilePatterns,
        const PruneOptions& options,
        std::ofstream* logFile
    ) {
        for (const auto& entry : fs::recursive_directory_iterator(options.src)) {
            if (entry.is_directory() && isExcludedDir(entry.path(), options.excludeDirs)) {
                if (!options.quiet) std::cout << "[Skip dir ] " << entry.path() << "\n";
                continue;
            }

            if (!entry.is_regular_file()) continue;

            const std::string filename = entry.path().filename().string();
            if (!typePatterns.empty() && !PatternUtils::matchesPattern(filename, typePatterns)) continue;
            if (!excludeFilePatterns.empty() && PatternUtils::matchesPattern(filename, excludeFilePatterns)) continue;

            fs::path relPath = fs::relative(entry.path(), options.src);
            fs::path targetFile;

            if (options.flatten) {
                std::string targetName = options.flattenWithSuffix
                    ? relPath.parent_path().string() + "_" + entry.path().filename().string()
                    : entry.path().filename().string();
                std::replace(targetName.begin(), targetName.end(), '\\', '_');
                std::replace(targetName.begin(), targetName.end(), '/', '_');
                targetFile = options.dst / targetName;
            }
            else {
                targetFile = options.dst / relPath;
            }

            if (fs::exists(targetFile)) {
                if (options.noOverwrite) continue;
                bool skipThisFile = false;

                if (!options.forceOverwrite) {
                    while (true) {
                        std::cout << "[Exists    ] " << relPath
                            << " - overwrite? [y]es / [n]o / [a]ll / [s]kip all / [c]ancel: ";
                        std::string input;
                        std::getline(std::cin, input);
                        if (input.empty()) continue;

                        char answer = std::tolower(input[0]);
                        switch (answer) {
                        case 'y': break;
                        case 'n':
                            skipThisFile = true;
                            break;
                        case 'a':
                            const_cast<PruneOptions&>(options).forceOverwrite = true;
                            break;
                        case 's':
                            const_cast<PruneOptions&>(options).noOverwrite = true;
                            return;
                        case 'c':
                            std::cout << "[Aborted] Operation cancelled by user.\n";
                            exit(0);
                        default: continue;
                        }
                        break; // aus while
                    }

                    if (skipThisFile) continue; // ✅ jetzt wirklich raus aus der for-Schleife
                }

            }

            if (!options.dryRun) {
                fs::create_directories(targetFile.parent_path());
                fs::copy_file(entry.path(), targetFile, fs::copy_options::overwrite_existing);
            }

            if (!options.quiet) std::cout << "[Copied    ] " << relPath << "\n";
            if (logFile) *logFile << "[Copied    ] " << relPath << "\n";
        }
    }

} // namespace FileCopier
