/*****************************************************************//**
 * @file   ArgumentParser.cpp
 * @brief  Implements argument parsing for the PruneCopy tool
 *
 * @author Patrik Neunteufel
 * @date   April 2025
 *********************************************************************/

#include "util/PatternUtils.hpp"
#include "util/ConvertUtils.hpp"
#include "cli/ArgumentParser.hpp"
#include "cli/Console.hpp"

#include <iostream>
#include <map>
#include <stdexcept>
#include <unordered_set>

std::vector<Flag> developerFlags = {
    {"--test-all", "", FlagType::Internal, FlagValueType::No_Value, "", "Run all tests"},
    {"--unit-test", "", FlagType::Internal, FlagValueType::No_Value, "", "Run internal unit test suite"},
    {"--benchmark", "", FlagType::Internal, FlagValueType::No_Value, "", "Run internal performance benchmarks"}
};

std::vector<Flag> legacy_required = {
    {"", "", FlagType::Info, FlagValueType::No_Value, "<source> <destination>", "Source and destination directory"}

}; 

std::vector<Flag> multi_required = {
    {"--source", "-s", FlagType::Option, FlagValueType::Multi_Value, "<paths>", "Copy from multiple source directories"},
    {"--destination", "-d", FlagType::Option, FlagValueType::Multi_Value, "<paths>", "Copy to multiple destination directories"},
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
    //{"--source", "-s", FlagType::Option, FlagValueType::Multi_Value, "<paths>", "Copy from multiple source directories"},
    //{"--destination", "-d", FlagType::Option, FlagValueType::Multi_Value, "<paths>", "Copy to multiple destination directories"},
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
    if (argc >= 3 && argv[1][0] != '-' && argv[2][0] != '-') {
        // ✅ Legacy-Modus
        options.sources.push_back(fs::absolute(argv[1]));
        options.destinations.push_back(fs::absolute(argv[2]));

    }
    else if (argc >= 3 && argv[1][0] != '-' && (hasFlag(argc, argv, "--destination") || hasFlag(argc, argv, "-d"))) {
        // ✅ Hybrid-Modus: positional <source> + --destination
        options.sources.push_back(fs::absolute(argv[1]));

        options.destinations = ConvertUtils::toPaths(getOptionValues(argc, argv, "--destination"));
        if (options.destinations.empty()) {
            options.destinations = ConvertUtils::toPaths(getOptionValues(argc, argv, "-d"));
        }
    }
    else {
        // ✅ Voller CLI-Modus
        if (hasFlag(argc, argv, "--source") || hasFlag(argc, argv, "-s")) {
            options.sources = ConvertUtils::toPaths(getOptionValues(argc, argv, "--source"));
            if (options.sources.empty()) {
                options.sources = ConvertUtils::toPaths(getOptionValues(argc, argv, "-s"));
            }
        }
        else {
            throw std::runtime_error("Missing required option: --source or -s");
        }

        if (hasFlag(argc, argv, "--destination") || hasFlag(argc, argv, "-d")) {
            options.destinations = ConvertUtils::toPaths(getOptionValues(argc, argv, "--destination"));
            if (options.destinations.empty()) {
                options.destinations = ConvertUtils::toPaths(getOptionValues(argc, argv, "-d"));
            }
        }
        else {
            throw std::runtime_error("Missing required option: --destination or -d");
        }
    }


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

std::string ArgumentParser::getOptionValue(int argc, char* argv[], const std::string& flag) {
    for (int i = 1; i < argc - 1; ++i) {
        if (std::string(argv[i]) == flag) {
            return argv[i + 1];
        }
    }
    return {};
}

bool ArgumentParser::checkArguments(int argc, char* argv[]) {

	std::string message;
	// check if there are no arguments
    if (argc <= 1) {                
        message ="No arguments provided.\nuse --help or -h for help\n";
        LogManager::log(LogLevel::Error, message);
        return false;
    }

    auto isRecognizedFlag = [](const std::string& arg) -> const Flag* {

        /* check developerFlags vector */
        auto it = std::find_if(developerFlags.begin(), developerFlags.end(), [&](const Flag& f) {
            return arg == f.name || (!f.shortName.empty() && arg == f.shortName);
            });
        if (it != developerFlags.end()) return &(*it);
        /* check multi_required vector */
        it = std::find_if(multi_required.begin(), multi_required.end(), [&](const Flag& f) {
            return arg == f.name || (!f.shortName.empty() && arg == f.shortName);
            });
        if (it != multi_required.end()) return &(*it);
        /* check infoFlags vector */
        it = std::find_if(infoFlags.begin(), infoFlags.end(), [&](const Flag& f) {
            return arg == f.name || (!f.shortName.empty() && arg == f.shortName);
            });
        if (it != infoFlags.end()) return &(*it);
        /* check optionFlags vector */
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
        if (!flag || !(flag->type == FlagType::Info || flag->type == FlagType::Internal)) {
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
    // oder kein Internal-Flag vorhanden ist, müssen mindestens zwei Positionsargumente vorhanden sein (Source und Destination)
    bool onlyInternalFlags = std::all_of(usedFlags.begin(), usedFlags.end(), [](const std::string& name) {
        auto it = std::find_if(developerFlags.begin(), developerFlags.end(), [&](const Flag& f) {
            return f.name == name;
            });
        return it != developerFlags.end();
        });

    if (!(onlyInfoFlags||onlyInternalFlags)) {
        bool hasSourceFlag = usedFlags.count("--source") || usedFlags.count("-s");
        bool hasDestFlag = usedFlags.count("--destination") || usedFlags.count("-d");

        bool validLegacy = (positionalArgs >= 2);
        bool validHybrid = (positionalArgs == 1 && hasDestFlag);
        bool validFullCLI = (positionalArgs == 0 && hasSourceFlag && hasDestFlag);

        if (!(validLegacy || validHybrid || validFullCLI)) {
            message = "Invalid argument combination.\n"
                "Expected:\n"
                "  prunecopy <source> <destination>\n"
                "  prunecopy <source> --destination ...\n"
                "  prunecopy --source ... --destination ...\n"
                "Use --help or -h for more information.\n";
            LogManager::log(LogLevel::Error, message);
            return false;
        }
    }


    return true;
}

bool ArgumentParser::checkInfo(int argc, char* argv[])
{
    if (ArgumentParser::hasFlag(argc, argv, "--help") || ArgumentParser::hasFlag(argc, argv, "-h")) {
        Console::printHelp();
        return true;
    }
    if (ArgumentParser::hasFlag(argc, argv, "--usage")) {
        Console::printUsage();
        return true;
    }
    if (ArgumentParser::hasFlag(argc, argv, "--version")) {
        Console::printVersion();
        return true;
    }
    if (ArgumentParser::hasFlag(argc, argv, "--about")) {
        Console::printAbout();
        Console::printRandomSupporter(!ArgumentParser::hasFlag(argc, argv, "--no-network"));
        return true;
    }
    if (ArgumentParser::hasFlag(argc, argv, "--contact-dev")) {
        Console::contactDev();
        return true;
    }
    if (ArgumentParser::hasFlag(argc, argv, "--donate")) {
        Console::printDonate();
        return true;
    }
    if (ArgumentParser::hasFlag(argc, argv, "--sponsors")) {
        Console::printAllSupporters(!ArgumentParser::hasFlag(argc, argv, "--no-network"));
        return true;
    }

    return false;
}


bool ArgumentParser::checkTests(int argc, char* argv[]) {
    if (hasFlag(argc, argv, "--test-all")) {
        bool success = TestRunner::runAllTests();
        return success ? 0 : 1;
    }
    // Weitere Test-Modi wie --unit-test oder --benchmark später hier
    return 0;
}