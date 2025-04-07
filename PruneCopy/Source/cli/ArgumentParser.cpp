/*****************************************************************//**
 * @file   ArgumentParser.cpp
 * @brief  Implements argument parsing for the PruneCopy tool
 *
 * @author Patrik Neunteufel
 * @date   April 2025
 *********************************************************************/

#include "cli/ArgumentParser.hpp"
#include "util/PatternUtils.hpp"
#include "cli/Console.hpp"

#include <iostream>
#include <map>
#include <stdexcept>
#include <unordered_set>

std::vector<Flag> required = {
	{"", "", FlagType::Info, FlagValueType::No_Value, "<source> <destination>", "Source and destination directory"}
};
std::vector<Flag> infoFlags = {
    {"--help", "-h", FlagType::Info, FlagValueType::No_Value, "", "Show this help message"},
    {"--usage", "", FlagType::Info, FlagValueType::No_Value, "", "Show usage information"},
    {"--version", "", FlagType::Info, FlagValueType::No_Value, "", "Show version information"},
    {"--about", "", FlagType::Info, FlagValueType::No_Value, "", "Show about information"},
    {"--contact-dev", "", FlagType::Info, FlagValueType::No_Value, "", "Contact the developer"},
    {"--donate", "", FlagType::Info, FlagValueType::No_Value, "", "Support the project"},
    {"--sponsors", "", FlagType::Info, FlagValueType::No_Value, "", "See the list of supporters"}
};
std::vector<Flag> optionFlags = {
    {"--additional-dest", "", FlagType::Option, FlagValueType::Multi_Value, "<paths>", "(comming feature) Copy to multiple destination directories"},
    {"--no-network", "", FlagType::Option, FlagValueType::No_Value, "", "Disable network access (e.g. for sponsors list)"},
    {"--types", "", FlagType::Option, FlagValueType::Multi_Value, "<patterns>", "Include only files matching given patterns (e.g. *.h *.hpp)"},
    {"--exclude-dirs", "", FlagType::Option, FlagValueType::Multi_Value, "<dirs>", "Exclude directories by name"},
    {"--exclude-files", "", FlagType::Option, FlagValueType::Multi_Value, "<patterns>", "Exclude files matching patterns (e.g. *Impl.hpp)"},
    {"--delete-target-first", "", FlagType::Option, FlagValueType::No_Value, "", "Delete the entire target folder before copying"},
    {"--no-overwrite", "", FlagType::Option, FlagValueType::No_Value, "", "Skip files that already exist"},
    {"--force-overwrite", "", FlagType::Option, FlagValueType::No_Value, "", "Overwrite existing files without asking"},
    {"--only-newer", "", FlagType::Option, FlagValueType::No_Value, "", "(comming feature) only copy, when source file is newer than the destination file"},
    {"--cmdln-out-off", "", FlagType::Option, FlagValueType::No_Value, "", "Suppress console output"},
    {"--log-dir", "", FlagType::Option, FlagValueType::Value,"<path>", "Write operations to a log file in the specified folder"},
    {"--log-open", "",FlagType::Option ,FlagValueType::No_Value,"","Open the log file after the operation (only when --log-dir <path> is set )"},
    {"--log-level", "", FlagType::Option, FlagValueType::Value, "<level>", "Set console log level: info (default), warning, error"},
    {"--flatten", "", FlagType::Option, FlagValueType::No_Value, "", "(comming feature) Copy all files into a single target directory"},
    {"--flatten-suffix", "", FlagType::Option, FlagValueType::No_Value, "", "(comming feature) Same as --flatten but adds suffixes(e.g.folders) to prevent name clashes"},
    {"--parallel-async", "", FlagType::Option, FlagValueType::No_Value, "", "(comming feature) Use async-based parallel file copying"},
    {"--parallel-thread", "", FlagType::Option, FlagValueType::No_Value, "", "(comming feature) Use threaded parallel file copying"},
    {"--parallel-openMP", "", FlagType::Option, FlagValueType::No_Value, "", "(comming feature) Use OpenMP for parallel copying"},
    {"--color", "", FlagType::Option, FlagValueType::Value,"<mode>", "Console color output: auto (default), always, never"},
	{"--dry-run", "", FlagType::Option, FlagValueType::No_Value, "", "Show what would be copied without doing it"}
};

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

bool ArgumentParser::checkArguments(int argc, char* argv[]) {
    //Console::printMessage(MessageType::Error, "test");
    //return true;
	std::string message;
    if (argc <= 1) {
        message ="No arguments provided.\nuse --help or -h for help\n";
        LogManager::log(LogLevel::Error, message);
        return false;
    }

    auto isRecognizedFlag = [](const std::string& arg) -> const Flag* {
        auto it = std::find_if(infoFlags.begin(), infoFlags.end(), [&](const Flag& f) {
            return arg == f.name || (!f.shortName.empty() && arg == f.shortName);
            });
        if (it != infoFlags.end()) return &(*it);

        it = std::find_if(optionFlags.begin(), optionFlags.end(), [&](const Flag& f) {
            return arg == f.name || (!f.shortName.empty() && arg == f.shortName);
            });
        if (it != optionFlags.end()) return &(*it);

        return nullptr;
        };

    std::unordered_set<std::string> usedFlags;
    int positionalArgs = 0;

    for (int i = 1; i < argc;) {
        std::string currentArg = argv[i];

        // Nicht-Flag → Positionsargument (z.B. Quelle/Ziel)
        if (currentArg.empty() || currentArg[0] != '-') {
            ++positionalArgs;
            ++i;
            continue;
        }

        const Flag* flag = isRecognizedFlag(currentArg);
        if (!flag) {
            message = "Unknown flag \"" + currentArg + "\"\nuse --help or -h for help\n";
            LogManager::log(LogLevel::Error, message);
            return false;
        }

        usedFlags.insert(flag->name);

        switch (flag->valueType) {
        case FlagValueType::No_Value:
            ++i;
            break;

        case FlagValueType::Value:
            if (i + 1 >= argc || argv[i + 1][0] == '-') {
                message = "Flag \"" + currentArg + "\" requires a value.\nuse --help or -h for help\n"; 
                LogManager::log(LogLevel::Error, message);
                return false;
            }
            i += 2;
            break;

        case FlagValueType::Multi_Value:
            ++i;
            if (i >= argc || argv[i][0] == '-') {
                message = "Flag \"" + currentArg + "\" requires at least one value.\nuse --help or -h for help\n"; 
                LogManager::log(LogLevel::Error, message);
                return false;
            }
            while (i < argc && argv[i][0] != '-') {
                ++i;
            }
            break;
        }
    }

    // Wenn nur ein Argument vorhanden ist, muss es ein Info-Flag sein
    if (argc == 2) {
        const std::string arg = argv[1];
        const Flag* flag = isRecognizedFlag(arg);
        if (!flag || flag->type != FlagType::Info) {
            message = "Single argument must be an Info flag (e.g., --help).\nuse --help or -h for help\n";
            LogManager::log(LogLevel::Error, message);
            return false;
        }
    }

    // Wenn kein Info-Flag vorhanden ist, müssen mindestens zwei Positionsargumente vorhanden sein (Source und Destination)
    bool onlyInfoFlags = std::all_of(usedFlags.begin(), usedFlags.end(), [](const std::string& name) {
        auto it = std::find_if(infoFlags.begin(), infoFlags.end(), [&](const Flag& f) {
            return f.name == name;
            });
        return it != infoFlags.end();
        });

    if (!onlyInfoFlags && positionalArgs < 2) {
        message = "Source and destination paths are required.\nuse --help or -h for help\n";
        LogManager::log(LogLevel::Error, message);
        return false;
    }

    return true;
}
