/*****************************************************************//**
 * @file   ArgumentParser.cpp
 * @brief  Implements argument parsing for the PruneCopy tool
 *
 * @author Patrik Neunteufel
 * @date   April 2025
 *********************************************************************/

#include "cli/ArgumentParser.hpp"

#include <iostream>
#include <map>
#include <stdexcept>


void ArgumentParser::parse(int argc, char* argv[], PruneOptions& options) {
    if (argc < 3 || argv[1][0] == '-' || argv[2][0] == '-') {
        throw std::runtime_error("Missing required <source> and <target> directories before options.\nUse --help or -h to display available options.");
    }

    options.src = fs::absolute(argv[1]);
    options.dst = fs::absolute(argv[2]);

    std::map<std::string, std::vector<std::string>*> flags = {
        {"--types", &options.types},
        {"--exclude-dirs", &options.excludeDirs},
        {"--exclude-files", &options.excludeFiles},
    };

    std::vector<std::string>* current = nullptr;
    for (int i = 3; i < argc; ++i) {
        std::string arg = argv[i];

        if (flags.count(arg)) {
            current = flags[arg];
        }
        else if (arg == "--log-dir") {
            if (i + 1 >= argc) throw std::runtime_error("--log-dir requires a path argument");
            options.logDir = fs::absolute(argv[++i]);
            options.enableLogging = true;
            current = nullptr;
        }
        else if (arg == "--log-open") {
            options.openLog = true;
        }
        else if (arg == "--cmdln-out-off") {
            options.quiet = true;
        }
        else if (arg == "--parallel-async") {
            options.parallelMode = ParallelMode::Async;
        }
        else if (arg == "--parallel-thread") {
            options.parallelMode = ParallelMode::Thread;
        }
        else if (arg == "--parallel-openMP") {
            options.parallelMode = ParallelMode::OpenMP;
        }
        else if (arg == "--delete-target-first") {
            options.deleteTargetFirst = true;
        }
        else if (arg == "--dry-run") {
            options.dryRun = true;
        }
        else if (arg == "--no-overwrite") {
            options.noOverwrite = true;
        }
        else if (arg == "--force-overwrite") {
            options.forceOverwrite = true;
        }
        else if (arg == "--flatten") {
            options.flatten = true;
        }
        else if (arg == "--flatten-suffix") {
            options.flatten = true;
            options.flattenWithSuffix = true;
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
        else if (current) {
            current->push_back(arg);
        }
        else {
            throw std::runtime_error("Unexpected argument: " + arg + "\nUse --help or -h to display available options.");
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