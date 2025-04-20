/*****************************************************************//**
 * @file   PatternUtils.cpp
 * @brief  Implements pattern matching utilities for PruneCopy
 * 
 * @author Patrik Neunteufel
 * @date   April 2025
 *********************************************************************/

#include <filesystem>
#include "util/PatternUtils.hpp"


 // Converts a list of wildcard strings (e.g. "*.cpp", "file?.txt") to case-insensitive regex patterns.
// Wildcards: * → .*, ? → . ; all other regex meta-characters are escaped.
std::vector<std::regex> PatternUtils::wildcardsToRegex(const std::vector<std::string>& wildcards) {
    std::vector<std::regex> result;
    for (const auto& w : wildcards) {
        std::string r = std::regex_replace(w, std::regex(R"([.^$+(){}|\[\]\\])"), R"(\$&)");
        r = std::regex_replace(r, std::regex(R"(\*)"), ".*");
        r = std::regex_replace(r, std::regex(R"(\?)"), ".");
        result.emplace_back("^" + r + "$", std::regex_constants::icase);
    }
    return result;
}

// Checks whether a given filename matches any of the provided regex patterns.
bool PatternUtils::matchesPattern(const std::string& filename, const std::vector<std::regex>& patterns) {
    for (const auto& pattern : patterns) {
        if (std::regex_match(filename, pattern)) return true;
    }
    return false;
}

// Converts a single glob pattern to a regex string.
// Used for internal conversions: * → .*, ? → ., . → \.
std::string PatternUtils::globToRegex(const std::string& pattern) {
    std::string regex = "^";
    for (char c : pattern) {
        switch (c) {
        case '*': regex += ".*"; break;
        case '?': regex += "."; break;
        case '.': regex += "\\."; break;
        default: regex += c;
        }
    }
    regex += "$";
    return regex;
}

// Converts a list of glob patterns (e.g. *.txt, src/*) into compiled case-insensitive regex objects.
std::vector<std::regex> PatternUtils::convertToRegex(const std::vector<std::string>& patterns) {
    std::vector<std::regex> result;
    for (const auto& pattern : patterns) {
        result.emplace_back(globToRegex(pattern), std::regex::icase);
    }
    return result;
}

// Checks if a directory name matches any of the given exclusion substrings.
// The check is based on partial matches (not full pattern matching).
bool PatternUtils::isExcludedDir(const std::filesystem::path& dir, const std::vector<std::string>& excludeDirs) {
    const std::string name = dir.filename().string();
    for (const auto& pattern : excludeDirs) {
        if (name.find(pattern) != std::string::npos) {
            return true;
        }
    }
    return false;
}

