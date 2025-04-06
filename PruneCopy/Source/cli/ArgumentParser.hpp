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
	std::string getOptionValue(int argc, char* argv[], const std::string& flag);
};



