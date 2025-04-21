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
#include "cli/PresetLoader.hpp"
#include "log/LogManager.hpp"

#include <iostream>
#include <map>
#include <stdexcept>
#include <unordered_set>


// List of developer - specific / internal - only flags(not shown in normal help)
std::vector<Flag> developerFlags = {
    {"--test-all", "", FlagType::Internal, FlagValueType::No_Value, "", "Run all tests"},
    {"--unit-test", "", FlagType::Internal, FlagValueType::No_Value, "", "Run internal unit test suite"},
    {"--benchmark", "", FlagType::Internal, FlagValueType::No_Value, "", "Run internal performance benchmarks"}
};

// Required positional arguments in legacy mode: <source> <destination>
std::vector<Flag> legacy_required = {
    {"", "", FlagType::Info, FlagValueType::No_Value, "<source> <destination>", "Source and destination directory"}
};

// Required named options for full CLI mode: --source and --destination
std::vector<Flag> multi_required = {
    {"--source", "-s", FlagType::Option, FlagValueType::Multi_Value, "<paths>", "Copy from multiple source directories"},
    {"--destination", "-d", FlagType::Option, FlagValueType::Multi_Value, "<paths>", "Copy to multiple destination directories"},
};

// Info/help-only flags (no impact on operation)
std::vector<Flag> infoFlags = {
    {"--help", "-h", FlagType::Info, FlagValueType::No_Value, "", "Show this help message"},
    {"--usage", "", FlagType::Info, FlagValueType::No_Value, "", "Show usage information"},
    {"--update", "", FlagType::Info, FlagValueType::No_Value, "", "check if newer version is available"},
    {"--version", "", FlagType::Info, FlagValueType::No_Value, "", "Show version information"},
    {"--about", "", FlagType::Info, FlagValueType::No_Value, "", "Show about information"},
    {"--contact-dev", "", FlagType::Info, FlagValueType::No_Value, "", "Contact the developer"},
    {"--donate", "", FlagType::Info, FlagValueType::No_Value, "", "Support the project"},
    {"--sponsors", "", FlagType::Info, FlagValueType::No_Value, "", "See the list of supporters"}
};

// Main CLI option flags that control behavior of the copy process
std::vector<Flag> optionFlags = {
    {"--no-network", "", FlagType::Option, FlagValueType::No_Value, "", "Disable network access (e.g. for sponsors list)"},
    {"--types", "", FlagType::Option, FlagValueType::Multi_Value, "<patterns>", "Include only files matching given patterns (e.g. *.h *.hpp)"},
    {"--exclude-dirs", "", FlagType::Option, FlagValueType::Multi_Value, "<dirs>", "Exclude directories by name"},
    {"--exclude-files", "", FlagType::Option, FlagValueType::Multi_Value, "<patterns>", "Exclude files matching patterns (e.g. *Impl.hpp)"},
    {"--delete-target-first", "", FlagType::Option, FlagValueType::No_Value, "", "Delete the entire target folder before copying"},
    {"--no-overwrite", "", FlagType::Option, FlagValueType::No_Value, "", "Skip files that already exist"},
    {"--force-overwrite", "", FlagType::Option, FlagValueType::No_Value, "", "Overwrite existing files without asking"},
    {"--only-newer", "", FlagType::Option, FlagValueType::No_Value, "", "(comming feature) only copy, when source file is newer than the destination file"},
    {"--cmdln-out-off", "", FlagType::Option, FlagValueType::No_Value, "", "Suppress console output", true, "--log-level none"},
    {"--log-dir", "", FlagType::Option, FlagValueType::Value,"<path>", "Write operations to a log file in the specified folder"},
    {"--log-open", "",FlagType::Option ,FlagValueType::No_Value,"","Open the log file after the operation (only when --log-dir <path> is set )"},
    {"--log-level", "", FlagType::Option, FlagValueType::Value, "<level>", "Set console log level: All, Standard, Info, Warning, Error, None"},
    {"--flatten", "", FlagType::Option, FlagValueType::No_Value, "", "Copy all files into a single target directory"},
    {"--flatten-auto-rename", "", FlagType::Option, FlagValueType::No_Value, "", "automatically rename conflict files (filename(1).ext), affect only with --flatten flag"},
    {"--flatten-suffix", "", FlagType::Option, FlagValueType::No_Value, "", "Same as --flatten but adds suffixes(e.g.folders) to prevent name clashes"},
    {"--parallel-async", "", FlagType::Option, FlagValueType::No_Value, "", "(comming feature) Use async-based parallel file copying"},
    {"--parallel-thread", "", FlagType::Option, FlagValueType::No_Value, "", "(comming feature) Use threaded parallel file copying"},
    {"--parallel-openMP", "", FlagType::Option, FlagValueType::No_Value, "", "(comming feature) Use OpenMP for parallel copying"},
    {"--color", "", FlagType::Option, FlagValueType::Value,"<mode>", "Console color output: auto (default), always, never"},
    {"--dry-run", "", FlagType::Option, FlagValueType::No_Value, "", "Show what would be copied without doing it"}
};
std::vector<Flag> presetFlags = {
	{"--preset", "", FlagType::Preset, FlagValueType::Value, "<name>", "Load and execute a named preset from ./presets/<name>.json"},
	{"--save-preset", "", FlagType::Preset, FlagValueType::Value, "<name>", "Save current settings to ./presets/<name>.json"},
	{"--list-presets", "", FlagType::Preset, FlagValueType::No_Value, "", "List all available presets in the ./presets folder"},
	{"--show-preset", "", FlagType::Preset, FlagValueType::Value, "<name>", "Print the CLI equivalent of the preset"},
    {"--dump-options", "", FlagType::Preset, FlagValueType::No_Value, "", "(coming feature) Dump current config as JSON (for CI/debugging)"}
};


// Holds all deprecated flags that were used during parsing (for later warning emission)
std::vector<std::string> ArgumentParser::s_deprecatedUsedFlags = {};

// Main argument parser – decides which parsing mode to apply
void ArgumentParser::parse(int argc, char* argv[], PruneOptions& options, ParsedCliControl& controlFlags) {
    // Check deprecated flags first
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (!arg.empty() && arg[0] == '-') {
            ArgumentParser::checkDeprecatedFlag(arg);
        }
    }
    // Handle CLI preset-related flags first
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "--preset") {
            if (i + 1 >= argc) throw std::runtime_error("--preset requires a preset name");
            controlFlags.usePreset = true;
            controlFlags.presetName = argv[++i];
            controlFlags.normalMode = false;
			return;
        }
        else if (arg == "--save-preset") {
            if (i + 1 >= argc) throw std::runtime_error("--save-preset requires a preset name");
            controlFlags.savePreset = true;
            controlFlags.presetName = argv[++i];
            // normalMode bleibt true – darf weiter normal laufen
        }
        else if (arg == "--show-preset") {
            if (i + 1 >= argc) throw std::runtime_error("--show-preset requires a preset name");
            controlFlags.showPreset = true;
            controlFlags.presetName = argv[++i];
            controlFlags.normalMode = false;
			return;
        }
        else if (arg == "--list-presets") {
            controlFlags.listPresets = true;
            controlFlags.normalMode = false;
			return;
        }
    }

    // --- Legacy Mode ---
    if (argc >= 3 && argv[1][0] != '-' && argv[2][0] != '-') {
        options.sources.push_back(fs::absolute(argv[1]));
        options.destinations.push_back(fs::absolute(argv[2]));
    }

    // --- Hybrid Mode ---
    else if (argc >= 3 && argv[1][0] != '-' && (hasFlag(argc, argv, "--destination") || hasFlag(argc, argv, "-d"))) {
        options.sources.push_back(fs::absolute(argv[1]));
        options.destinations = ConvertUtils::toPaths(getOptionValues(argc, argv, "--destination"));
        if (options.destinations.empty()) {
            options.destinations = ConvertUtils::toPaths(getOptionValues(argc, argv, "-d"));
        }
    }

    // --- Full CLI Mode ---
    else {
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

    // --- Multi-Value Options ---
    options.types = getOptionValues(argc, argv, "--types");
    options.excludeDirs = getOptionValues(argc, argv, "--exclude-dirs");
    options.excludeFiles = getOptionValues(argc, argv, "--exclude-files");

    // --- Compile Patterns ---
    options.typePatterns = PatternUtils::convertToRegex(options.types);
    options.excludeFilePatterns = PatternUtils::convertToRegex(options.excludeFiles);

    // --- Booleans ---
    options.dryRun = hasFlag(argc, argv, "--dry-run");
    options.noOverwrite = hasFlag(argc, argv, "--no-overwrite");
    options.forceOverwrite = hasFlag(argc, argv, "--force-overwrite");
    options.flatten = hasFlag(argc, argv, "--flatten") || hasFlag(argc, argv, "--flatten-suffix");
    options.flattenAutoRename = hasFlag(argc, argv, "--flatten-auto-rename");
    options.flattenWithSuffix = hasFlag(argc, argv, "--flatten-suffix");
    options.deleteTargetFirst = hasFlag(argc, argv, "--delete-target-first");
    options.quiet = hasFlag(argc, argv, "--cmdln-out-off");
    options.openLog = hasFlag(argc, argv, "--log-open");

    // --- Parallel Modes ---
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

    // --- Single-Value Options ---
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
                options.logLevel = parseLogLevel(argv[++i]);
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


// Checks if the given flag is present in the argument list
bool ArgumentParser::hasFlag(int argc, char* argv[], const std::string& flag) {
    return std::any_of(argv + 1, argv + argc, [&](const char* arg) {
        return std::string(arg) == flag;
        });
}

// Collects all values that follow the given flag, until the next flag starts
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
            if (!arg.empty() && arg[0] == '-') break; // Stop at the next flag
            values.push_back(arg);
        }
    }
    return values;
}

// Returns the single value directly after the given flag
std::string ArgumentParser::getOptionValue(int argc, char* argv[], const std::string& flag) {
    for (int i = 1; i < argc - 1; ++i) {
        if (std::string(argv[i]) == flag) {
            return argv[i + 1];
        }
    }
    return {};
}


// Validates the overall argument combination and ensures proper usage
bool ArgumentParser::checkArguments(int argc, char* argv[]) {
    std::string message;

    // If no arguments were given, print error and return false
    if (argc <= 1) {
        message = "No arguments provided.\nuse --help or -h for help\n";
        LogManager::log(LogLevel::Error, message);
        return false;
    }

    // Lambda to match a known flag in all flag categories
    auto isRecognizedFlag = [](const std::string& arg) -> const Flag* {
        auto it = std::find_if(developerFlags.begin(), developerFlags.end(), [&](const Flag& f) {
            return arg == f.name || (!f.shortName.empty() && arg == f.shortName);
            });
        if (it != developerFlags.end()) return &(*it);

        it = std::find_if(presetFlags.begin(), presetFlags.end(), [&](const Flag& f) {
            return arg == f.name || (!f.shortName.empty() && arg == f.shortName);
            });
        if (it != presetFlags.end()) return &(*it);

        it = std::find_if(multi_required.begin(), multi_required.end(), [&](const Flag& f) {
            return arg == f.name || (!f.shortName.empty() && arg == f.shortName);
            });
        if (it != multi_required.end()) return &(*it);

        it = std::find_if(infoFlags.begin(), infoFlags.end(), [&](const Flag& f) {
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

    // Parse all arguments and track flag usage and positional arguments
    for (int i = 1; i < argc;) {
        std::string currentArg = argv[i];

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

    // If only one argument is given, it must be informational
    if (argc == 2) {
        const std::string arg = argv[1];
        const Flag* flag = isRecognizedFlag(arg);
        if (!flag || !(flag->type == FlagType::Info || flag->type == FlagType::Internal || flag->type == FlagType::Preset)) {
            message = "Single argument must be an Info flag (e.g., --help).\nuse --help or -h for help\n";
            LogManager::log(LogLevel::Error, message);
            return false;
        }
    }

    // Check if only info or only internal flags were used
    bool onlyInfoFlags = std::all_of(usedFlags.begin(), usedFlags.end(), [](const std::string& name) {
        auto it = std::find_if(infoFlags.begin(), infoFlags.end(), [&](const Flag& f) {
            return f.name == name;
            });
        return it != infoFlags.end();
        });

    bool onlyInternalFlags = std::all_of(usedFlags.begin(), usedFlags.end(), [](const std::string& name) {
        auto it = std::find_if(developerFlags.begin(), developerFlags.end(), [&](const Flag& f) {
            return f.name == name;
            });
        return it != developerFlags.end();
        });

    bool onlypresetFlags = std::all_of(usedFlags.begin(), usedFlags.end(), [](const std::string& name) {
        auto it = std::find_if(presetFlags.begin(), presetFlags.end(), [&](const Flag& f) {
            return f.name == name;
            });
        return it != presetFlags.end();
        });

    // Validate that required argument combinations are fulfilled
    if (!(onlyInfoFlags || onlyInternalFlags || onlypresetFlags)) {
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

// Checks and handles all informational flags, and invokes corresponding Console output
bool ArgumentParser::checkInfo(int argc, char* argv[]) {
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
    if (ArgumentParser::hasFlag(argc, argv, "--update")) {
        Console::printUpdate();
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

// Checks if test-related flags are set
bool ArgumentParser::checkTests(int argc, char* argv[]) {
    if (hasFlag(argc, argv, "--test-all")) {
        return true;
    }
    // Additional test flags (e.g. --unit-test, --benchmark) could be handled here in future
    return false;
}

// Parses and returns the appropriate log level enum from a string
LogLevel ArgumentParser::parseLogLevel(const std::string& str) {
    std::string s = str;
    std::transform(s.begin(), s.end(), s.begin(), ::tolower);
    if (s == "all")      return LogLevel::All;
    if (s == "standard") return LogLevel::Standard;
    if (s == "info")     return LogLevel::Info;
    if (s == "warning")  return LogLevel::Warning;
    if (s == "error")    return LogLevel::Error;
    if (s == "none")     return LogLevel::None;
    throw std::invalid_argument("Invalid log level: " + str);
}

// Detects if a used flag is deprecated and stores it for later warning output
void ArgumentParser::checkDeprecatedFlag(const std::string& arg) {
    for (const auto& flag : getAllRegisteredFlags()) {
        if ((flag.name == arg || flag.shortName == arg) && flag.deprecated) {
            s_deprecatedUsedFlags.push_back(arg); // Store deprecated flag
        }
    }
}

// Emits warnings for all deprecated flags that were detected
void ArgumentParser::emitDeprecatedWarnings() {
    for (const auto& flag : s_deprecatedUsedFlags) {
        for (const auto& reg : getAllRegisteredFlags()) {
            if (reg.name == flag || reg.shortName == flag) {
                std::string msg = "Flag '" + flag + "' is deprecated.";
                if (reg.replacement.has_value()) {
                    msg += " Use '" + reg.replacement.value() + "' instead.";
                }
                LogManager::log(LogLevel::Warning, msg);
            }
        }
    }
}

// Clears the list of deprecated flags (typically called after warnings were printed)
void ArgumentParser::clearDeprecatedFlagLog() {
    s_deprecatedUsedFlags.clear();
}

// Combines and returns all known flag definitions across categories
const std::vector<Flag>& ArgumentParser::getAllRegisteredFlags() {
    static std::vector<Flag> all = [] {
        std::vector<Flag> combined;
        combined.insert(combined.end(), infoFlags.begin(), infoFlags.end());
        combined.insert(combined.end(), optionFlags.begin(), optionFlags.end());
        combined.insert(combined.end(), developerFlags.begin(), developerFlags.end());
        combined.insert(combined.end(), legacy_required.begin(), legacy_required.end());
        combined.insert(combined.end(), multi_required.begin(), multi_required.end());
        return combined;
        }();
    return all;
}

/*****************************************************************//**
 * @brief   Reconstructs the command line arguments based on current options.
 * @param   options The PruneOptions structure to convert.
 * @return  A vector of argument strings equivalent to a CLI call.
 *********************************************************************/
std::vector<std::string> ArgumentParser::rebuildArgumentsFromOptions(const PruneOptions& options)
{
    std::vector<std::string> args;

    // --- Source directories ---
    if (!options.sources.empty()) {
        args.push_back("--source");
        for (const auto& src : options.sources) {
            args.push_back(src.string());
        }
    }

    // --- Destination directories ---
    if (!options.destinations.empty()) {
        args.push_back("--destination");
        for (const auto& dst : options.destinations) {
            args.push_back(dst.string());
        }
    }

    // --- Include file types ---
    if (!options.types.empty()) {
        args.push_back("--types");
        for (const auto& pattern : options.types) {
            args.push_back(pattern);
        }
    }

    // --- Excluded directories ---
    if (!options.excludeDirs.empty()) {
        args.push_back("--exclude-dirs");
        for (const auto& dir : options.excludeDirs) {
            args.push_back(dir);
        }
    }

    // --- Excluded file patterns ---
    if (!options.excludeFiles.empty()) {
        args.push_back("--exclude-files");
        for (const auto& pattern : options.excludeFiles) {
            args.push_back(pattern);
        }
    }

    // --- Log options ---
    if (options.enableLogging && !options.logDir.empty()) {
        args.push_back("--log-dir");
        args.push_back(options.logDir.string());

        if (options.openLog) {
            args.push_back("--log-open");
        }
    }

    // --- Log level ---
    switch (options.logLevel) {
    case LogLevel::None:      args.push_back("--log-level"); args.push_back("None"); break;
    case LogLevel::Error:     args.push_back("--log-level"); args.push_back("Error"); break;
    case LogLevel::Warning:   args.push_back("--log-level"); args.push_back("Warning"); break;
    case LogLevel::Info:      args.push_back("--log-level"); args.push_back("Info"); break;
    case LogLevel::Standard:  args.push_back("--log-level"); args.push_back("Standard"); break;
    case LogLevel::All:       args.push_back("--log-level"); args.push_back("All"); break;
    default: break;
    }

    // --- Booleans / flags ---
    if (options.dryRun)            args.push_back("--dry-run");
    if (options.deleteTargetFirst) args.push_back("--delete-target-first");
    if (options.noOverwrite)       args.push_back("--no-overwrite");
    if (options.forceOverwrite)    args.push_back("--force-overwrite");
    if (options.flatten)           args.push_back("--flatten");
    if (options.flattenWithSuffix) args.push_back("--flatten-suffix");

    // --- Parallel mode ---
    switch (options.parallelMode) {
    case ParallelMode::Async:   args.push_back("--parallel-async"); break;
    case ParallelMode::Thread:  args.push_back("--parallel-thread"); break;
    case ParallelMode::OpenMP:  args.push_back("--parallel-openMP"); break;
    default: break;
    }

    // --- Color mode ---
    switch (options.colorMode) {
    case ColorMode::Always: args.push_back("--color"); args.push_back("always"); break;
    case ColorMode::Never:  args.push_back("--color"); args.push_back("never"); break;
    case ColorMode::Auto:
    default:                args.push_back("--color"); args.push_back("auto"); break;
    }

    return args;
}

