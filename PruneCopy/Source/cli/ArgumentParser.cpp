/*****************************************************************//**
 * @file   ArgumentParser.cpp
 * @brief  Implements argument parsing for the PruneCopy tool
 *
 * @author Patrik Neunteufel
 * @date   April 2025
 *********************************************************************/

#include "cli/ArgumentParser.hpp"
#include "util/PatternUtils.hpp"

#include <iostream>
#include <map>
#include <stdexcept>


void ArgumentParser::parse(int argc, char* argv[], PruneOptions& options) {
    if (argc < 3 || argv[1][0] == '-' || argv[2][0] == '-') {
        throw std::runtime_error("Missing required <source> and <target> directories before options.\nUse --help or -h to display available options.");
    }

    options.src = fs::absolute(argv[1]);
    options.dst = fs::absolute(argv[2]);

    // Wertebasierte Optionen
    options.types = getOptionValues(argc, argv, "--types");
    options.excludeDirs = getOptionValues(argc, argv, "--exclude-dirs");
    options.excludeFiles = getOptionValues(argc, argv, "--exclude-files");

    options.typePatterns = PatternUtils::convertToRegex(options.types);
    options.excludeFilePatterns = PatternUtils::convertToRegex(options.excludeFiles);

    // Einfache Schalter (boolesche Flags)
    options.dryRun = hasFlag(argc, argv, "--dry-run");
    options.noOverwrite = hasFlag(argc, argv, "--no-overwrite");
    options.forceOverwrite = hasFlag(argc, argv, "--force-overwrite");
    options.flatten = hasFlag(argc, argv, "--flatten") || hasFlag(argc, argv, "--flatten-suffix");
    options.flattenWithSuffix = hasFlag(argc, argv, "--flatten-suffix");
    options.deleteTargetFirst = hasFlag(argc, argv, "--delete-target-first");
    options.quiet = hasFlag(argc, argv, "--cmdln-out-off");
    options.openLog = hasFlag(argc, argv, "--log-open");

    // Parallelmodus
    if (hasFlag(argc, argv, "--parallel-thread")) {
        options.parallelMode = ParallelMode::Thread;
    }
    else if (hasFlag(argc, argv, "--parallel-openMP")) {
        options.parallelMode = ParallelMode::OpenMP;
    }
    else if (hasFlag(argc, argv, "--parallel-async")) {
        options.parallelMode = ParallelMode::Async;
    }
    else {
        options.parallelMode = ParallelMode::None;
    }

    // Wertbehaftete Optionen
    for (int i = 3; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "--log-dir") {
            if (i + 1 >= argc) throw std::runtime_error("--log-dir requires a path argument");
            options.logDir = fs::absolute(argv[++i]);
            options.enableLogging = true;
        }
        else if (arg == "--log-level") {
            if (i + 1 >= argc) throw std::runtime_error("--log-level requires a value (info|warning|error)");
            try {
                LogManager::setConsoleLogLevel(LogManager::parseLogLevel(argv[++i]));
            }
            catch (const std::exception& e) {
                throw std::runtime_error(std::string("Invalid log level: ") + e.what());
            }
        }
        else if (arg == "--color") {
            if (i + 1 >= argc) throw std::runtime_error("--color requires a value (auto|always|never)");
            std::string value = argv[++i];
            std::transform(value.begin(), value.end(), value.begin(), ::tolower);
            if (value == "auto")        options.colorMode = ColorMode::Auto;
            else if (value == "always") options.colorMode = ColorMode::Always;
            else if (value == "never")  options.colorMode = ColorMode::Never;
            else throw std::runtime_error("Invalid color mode: " + value);
        }
    }
}


bool ArgumentParser::hasFlag(int argc, char* argv[], const std::string& flag) {
    return std::any_of(argv + 1, argv + argc, [&](const char* arg) {
        return std::string(arg) == flag;
        });
}

std::vector<std::string> ArgumentParser::getOptionValues(int argc, char* argv[], const std::string& flag) {
    std::vector<std::string> values;
    bool collect = false;
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == flag) {
            collect = true;
            continue;
        }

        if (collect) {
            if (!arg.empty() && arg[0] == '-') break; // Nächstes Flag erreicht
            values.push_back(arg);
        }
    }
    return values;


}
std::string getOptionValue(int argc, char* argv[], const std::string& flag) {
    for (int i = 1; i < argc - 1; ++i) {
        if (std::string(argv[i]) == flag) {
            return argv[i + 1];
        }
    }
    return {};
}