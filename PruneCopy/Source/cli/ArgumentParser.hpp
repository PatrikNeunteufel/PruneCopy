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

#include <string>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

enum class FlagType {
	Info,          ///< Information flag (e.g., --help)
	Option         ///< Option flag (e.g., --types)
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
struct Flag {
	std::string name;
	std::string shortName;
	FlagType type;
	FlagValueType valueType;
	std::string valueRepresentation;
	std::string description;
};

extern std::vector<Flag> legacy_required;
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
	 * @brief get the values of a flag.
	 * 
	 * @param argc
	 * @param argv
	 * @param flag
	 * @return 
	 */
	static std::vector<std::string> getOptionValues(int argc, char* argv[], const std::string& flag);
	static std::string getOptionValue(int argc, char* argv[], const std::string& flag);
	static bool checkArguments(int argc, char* argv[]);
	static bool checkInfo(int argc, char* argv[]);

};



