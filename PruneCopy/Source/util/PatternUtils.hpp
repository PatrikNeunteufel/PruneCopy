/*****************************************************************//**
 * @file   PatternUtils.hpp
 * @brief  Provides pattern matching utilities (wildcard to regex)
 * 
 * @author Patrik Neunteufel
 * @date   April 2025
 *********************************************************************/

#pragma once

#include <vector>
#include <string>
#include <regex>
#include <filesystem>

namespace PatternUtils {

    /**
     * @brief Converts a list of wildcard patterns to equivalent regex objects.
     * @param wildcards Vector of wildcard strings (e.g., "*.hpp")
     * @return Vector of std::regex
     */
    std::vector<std::regex> wildcardsToRegex(const std::vector<std::string>& wildcards);

    /**
     * @brief Checks whether a filename matches any of the given regex patterns.
     * @param filename Name of file to test
     * @param patterns Compiled regex pattern list
     * @return true if any match, false otherwise
     */
    bool matchesPattern(const std::string& filename, const std::vector<std::regex>& patterns);

    /**
     * @brief Converts a glob pattern to a regex pattern.
     * @param pattern The glob pattern (e.g., "*.hpp")
     * @return The equivalent regex pattern
     */
    std::string globToRegex(const std::string& pattern);

	/**
	 * @brief Converts a list of glob patterns to equivalent regex objects.
	 * @param patterns Vector of glob strings (e.g., "*.hpp")
	 * @return Vector of std::regex
	 */
    std::vector<std::regex> convertToRegex(const std::vector<std::string>& patterns);

	/**
	 * @brief Checks if a directory name matches any of the given exclusion substrings.
	 * @param dir Directory name to check
	 * @param excludeDirs Vector of exclusion substrings
	 * @return true if the directory is excluded, false otherwise
	 */
    bool isExcludedDir(const std::filesystem::path& dir, const std::vector<std::string>& excludeDirs);


} // namespace PatternUtils
