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

} // namespace PatternUtils
