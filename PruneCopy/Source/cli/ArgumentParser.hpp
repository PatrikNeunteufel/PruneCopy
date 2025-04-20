/*****************************************************************//**
 * @file   ArgumentParser.hpp
 * @brief  Parses command-line arguments for the PruneCopy tool
 *
 * @author Patrik Neunteufel
 * @date   April 2025
 *********************************************************************/

#pragma once
#include "log/LogManager.hpp"
#include "core/PruneOptions.hpp"
#include "test/TestRunner.hpp"

#include <string>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

/**
 * @brief Enum for different types of command-line flags.
 *
 * This enum categorizes the flags into different types such as Info, Option, and Internal.
 */
enum class FlagType {
	Info,          ///< Information flag (e.g., --help)
	Option,         ///< Option flag (e.g., --types)
	Internal,       ///< Internal flag (e.g., --unit-test)
};
/**
 * @brief Enum for different types of command-line flags.
 */
enum class FlagValueType {
	No_Value,        ///< Flag without value (e.g., --help)
	Value,          ///< Flag with a single value (e.g., --output <path>)
	Multi_Value     ///< Flag with multiple values (e.g., --types <type1> <type2>)
};
/**
 * @brief Struct representing a command-line flag.
 */
 // Represents a single command-line flag or option
struct Flag {
	std::string name;                       // Full flag name (e.g., "--help")
	std::string shortName;                  // Optional short alias (e.g., "-h")
	FlagType type;                          // Category of the flag (Info, Option, Internal, etc.)
	FlagValueType valueType;                // Whether the flag expects a value (or multiple) or none
	std::string valueRepresentation;        // Human-readable placeholder for value(s), shown in help (e.g., "<path>")
	std::string description;                // Description of the flag used in help output
	bool deprecated = false;                // Marks the flag as deprecated (triggers warning)
	std::optional<std::string> replacement; // Suggested replacement flag if deprecated
};

/**
 * @brief List of developer-specific command-line flags for the PruneCopy tool.
 *
 * This list contains all available internal flags that are not shown in normal help.
 */
extern std::vector<Flag> developerFlags;

/**
 * @brief List of command-line flags for the PruneCopy tool.
 *
 * This list contains all available required positional arguments in legacy mode.
 */
extern std::vector<Flag> legacy_required;

/**
 * @brief List of command-line flags for the PruneCopy tool.
 *
 * This list contains all available option flags, their types, and descriptions.
 */
extern std::vector<Flag> multi_required;

/**
 * @brief List of command-line flags for the PruneCopy tool.
 *
 * This list contains all available info flags, their types, and descriptions.
 */
extern std::vector<Flag> infoFlags;

/**
 * @brief List of command-line flags for the PruneCopy tool.
 *
 * This list contains all available info flags, their types, and descriptions.
 */
extern std::vector<Flag> infoFlags;

/**
 * @brief List of command-line flags for the PruneCopy tool.
 *
 * This list contains all available option flags, their types, and descriptions.
 */
extern std::vector<Flag> optionFlags;

/**
 * @brief Parses command-line arguments for the PruneCopy tool.
 *
 * This class provides methods to parse command-line arguments and set
 * configuration variables based on the provided options.
 */
class ArgumentParser {
public:

    /**
     * @brief Parses command-line arguments and sets configuration variables.
     */
    static void parse(int argc, char* argv[], PruneOptions& options);

	/**
	 * @brief Checks if a specific flag is present in the command-line arguments.
	 * @param argc Number of command-line arguments.
	 * @param argv Array of command-line arguments.
	 * @param flag The flag to check for (e.g., "--help").
	 * @return True if the flag is present, false otherwise.
	 */
    static bool hasFlag(int argc, char* argv[], const std::string& flag);

	 /**
	 * @brief Collects all values that follow a specific flag in the command-line arguments.
	 * @param argc Number of command-line arguments.
	 * @param argv Array of command-line arguments.
	 * @param flag The flag to check for (e.g., "--help").
	 * @return A vector of values associated with the flag.
	 */	
	static std::vector<std::string> getOptionValues(int argc, char* argv[], const std::string& flag);
	
	/**
	 * @brief Retrieves the value associated with a specific flag in the command-line arguments.
	 * @param argc Number of command-line arguments.
	 * @param argv Array of command-line arguments.
	 * @param flag The flag to check for (e.g., "--output").
	 * @return The value associated with the flag, or an empty string if not found.
	 */
	static std::string getOptionValue(int argc, char* argv[], const std::string& flag);
	
	/**
	 * @brief Checks and handles all informational flags, and invokes corresponding Console output.
	 * @param argc Number of command-line arguments.
	 * @param argv Array of command-line arguments.
	 * @return True if the arguments are valid, false otherwise.
	 */
	static bool checkArguments(int argc, char* argv[]);
	
	/**
	 * @brief Checks if test-related flags are set.
	 * @param argc Number of command-line arguments.
	 * @param argv Array of command-line arguments.
	 * @return True if test-related flags are set, false otherwise.
	 */
	static bool checkInfo(int argc, char* argv[]);
	
	/**
	 * @brief Checks if test-related flags are set.
	 * @param argc Number of command-line arguments.
	 * @param argv Array of command-line arguments.
	 * @return True if test-related flags are set, false otherwise.
	 */
	static bool checkTests(int argc, char* argv[]);
	
	/**
	 * @brief Parses a string and returns the corresponding LogLevel enum.
	 * @param str The string representation of the log level (e.g., "info", "warning").
	 * @return The corresponding LogLevel enum value.
	 */
	static LogLevel parseLogLevel(const std::string& str);
	
	/**
	 * @brief Checks if the given path is a valid directory.
	 * @param path The path to check.
	 * @return True if the path is a valid directory, false otherwise.
	 */
	static void emitDeprecatedWarnings();

	/**
	 * @brief Clears the list of deprecated flags used during parsing.
	 */
	static void clearDeprecatedFlagLog();

private:

	/**
	 * @brief List of deprecated flags that were used during parsing.
	 *
	 * This list is used to emit warnings for deprecated flags after parsing.
	 */
	static std::vector<std::string> s_deprecatedUsedFlags;

	/**
	 * @brief Emits warnings for all deprecated flags that were detected.
	 */
	static void checkDeprecatedFlag(const std::string& arg);

	/**
	 * @brief Combines and returns all known flag definitions across categories.
	 * @return A vector containing all registered flags.
	 */
	static const std::vector<Flag>& getAllRegisteredFlags();
};



