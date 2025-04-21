#pragma execution_character_set("utf-8")


#include "cli/ArgumentParser.hpp"
#include "cli/Console.hpp"
#include "cli/PresetLoader.hpp"
#include "util/PatternUtils.hpp"
#include "core/FileCopier.hpp"
#include "log/LogManager.hpp"
#include "core/PruneOptions.hpp"
#include "test/TestRunner.hpp"



#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <sstream>
#ifdef _WIN32
#include <windows.h> //only under windows
#endif

int main(int argc, char* argv[]) {
    // Set the console output to UTF-8 encoding on Windows
#ifdef _WIN32
    if (!SetConsoleOutputCP(CP_UTF8)) {
        std::cerr << "[Warning] Could not set UTF-8 output encoding.\n";
    }
#endif

    // Handle CLI control flags: help, info, test, or argument validation
    if (!ArgumentParser::checkArguments(argc, argv)) {
        return 1;
    }
    else if (ArgumentParser::checkInfo(argc, argv)) {
        return 0;
    }
    else if (ArgumentParser::checkTests(argc, argv)) {
        TestRunner::runAllTests();
        return 0;
    }

    try {
        // Parse arguments into PruneOptions and controlFlags and configure logging
        PruneOptions options;
		ParsedCliControl controlFlags;
        ArgumentParser::parse(argc, argv, options, controlFlags);
        LogManager::setConsoleLogLevel(options.logLevel);
        LogManager::enableAnsiColorsIfSupported(options.colorMode);
        ArgumentParser::emitDeprecatedWarnings(); // now colored output
        if (!controlFlags.normalMode) {
            // Nur ein spezieller CLI-Befehl wurde gegeben, normale Ausführung wird übersprungen

            if (controlFlags.listPresets) {
                auto presets = PresetLoader::listPresets();
                if (presets.empty()) {
                    LogManager::log(LogLevel::Info, "No presets found in: " + PresetLoader::getPresetDir().string());
                }
                else {
                    LogManager::log(LogLevel::Info, "Available presets:");
                    for (const auto& name : presets) {
                        LogManager::log(LogLevel::Info, "  " + name);
                    }
                }
                return 0;
            }

            if (controlFlags.showPreset) {
                auto cliString = PresetLoader::showPreset(controlFlags.presetName);
                if (cliString.empty()) {
                    LogManager::log(LogLevel::Error, "Preset \"" + controlFlags.presetName + "\" could not be found or loaded.");
                    return 2;
                }
                LogManager::log(LogLevel::Info, cliString);
                return 0;
            }

            if (controlFlags.usePreset) {
                auto loaded = PresetLoader::loadPreset(controlFlags.presetName);
                if (!loaded.has_value()) {
                    LogManager::log(LogLevel::Error, "Failed to load preset: " + controlFlags.presetName);
                    return 2;
                }
                options = loaded.value(); // ersetzt die vorher parsierten Optionen
                LogManager::log(LogLevel::Info, "Loaded preset: " + controlFlags.presetName);
            }
        }

        // Preprocess patterns for filtering
        auto typePatterns = PatternUtils::wildcardsToRegex(options.types);
        auto excludeFilePatterns = PatternUtils::wildcardsToRegex(options.excludeFiles);

        // Set up logging to file, if enabled
        std::ofstream logFile;
        std::string logFilePath;
        if (options.enableLogging) {
            std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            std::tm tm{};
            localtime_s(&tm, &now);
            std::ostringstream filename;
            filename << options.logDir.string() << "/PruneCopy_" << std::put_time(&tm, "%Y-%m-%d_%H-%M-%S") << ".log";
            fs::create_directories(options.logDir);
            logFilePath = filename.str();
            logFile.open(logFilePath);

            if (logFile.is_open()) {
                LogManager::setLogFile(&logFile);
            }
            else {
                std::cerr << "Logfile could not be opened!\n";
            }
        }

        // Start main process
        LogManager::log(LogLevel::Info, "Starting PruneCopy");

        // Log all source paths
        int srcCounter = 1;
        for (fs::path src : options.sources) {
            LogManager::log(LogLevel::Info,
                "Source " + ((options.sources.size() > 1) ? "(" + std::to_string(srcCounter++) + ")" : "") +
                ": " + src.string());
        }

        // Log all destination paths
        int destCounter = 1;
        for (fs::path dst : options.destinations) {
            LogManager::log(LogLevel::Info,
                "Destination " + ((options.destinations.size() > 1) ? "(" + std::to_string(destCounter++) + ")" : "") +
                ": " + dst.string());
        }

        // Delete destination directory if requested
        if (options.dryRun) LogManager::log(LogLevel::Info, "Dry run enabled – no files will be copied.");
        for (fs::path dst : options.destinations) {
            if (options.deleteTargetFirst) {
                LogManager::log(LogLevel::Warning, "Deleting target directory before copy.");
                if (!options.dryRun) fs::remove_all(dst);
            }
        }


        // Log included file patterns (types)
        if (!options.types.empty()) {
            std::string joinedTypes;
            for (const auto& type : options.types) {
                if (!joinedTypes.empty()) joinedTypes += ", ";
                joinedTypes += type;
            }
            LogManager::log(LogLevel::Info, "Included file patterns: " + joinedTypes);
        }

        // Log excluded directories
        if (!options.excludeDirs.empty()) {
            std::string joinedDirs;
            for (const auto& dir : options.excludeDirs) {
                if (!joinedDirs.empty()) joinedDirs += ", ";
                joinedDirs += dir;
            }
            LogManager::log(LogLevel::Info, "Excluded directories: " + joinedDirs);
        }

        // Log excluded file patterns
        if (!options.excludeFiles.empty()) {
            std::string joinedFiles;
            for (const auto& pattern : options.excludeFiles) {
                if (!joinedFiles.empty()) joinedFiles += ", ";
                joinedFiles += pattern;
            }
            LogManager::log(LogLevel::Info, "Excluded file patterns: " + joinedFiles);
        }

        LogManager::log(LogLevel::Info, "Copying files...");


        // Run the file copy based on selected mode
        switch (options.parallelMode) {
        case ParallelMode::None:
            FileCopier::copyFiltered(options,
                options.enableLogging ? &logFile : nullptr);
            break;
        case ParallelMode::Async:
            LogManager::log(LogLevel::Error, "Parallel async with user prompts not implemented.");
            return 3;
        case ParallelMode::Thread:
        case ParallelMode::OpenMP:
            LogManager::log(LogLevel::Error, "Parallel mode not yet implemented.");
            return 2;
        }

        LogManager::log(LogLevel::Info, "Copy process completed successfully.");

        // Open log file in file browser (if enabled)
        if (options.enableLogging && options.openLog) {
            if (logFile.is_open()) {
                logFile.close();
            }
#ifdef _WIN32
            std::string command = "start \"\" \"" + logFilePath + "\"";
#else
            std::string command = "xdg-open \"" + logFilePath + "\"";
#endif
            system(command.c_str());
        }

        // Save current settings to preset (if requested)
        if (controlFlags.savePreset) {
            bool success = PresetLoader::savePreset(controlFlags.presetName, options);
            if (success) {
                LogManager::log(LogLevel::Info, "Preset saved: " + controlFlags.presetName);
            }
            else {
                LogManager::log(LogLevel::Error, "Failed to save preset: " + controlFlags.presetName);
            }
        }

        return 0;
    }
    catch (const std::exception& e) {
        LogManager::log(LogLevel::Error, std::string("Fatal error: ") + e.what());
        return 1;
    }
}
