/*****************************************************************//**
 * @file   PatternUtils.cpp
 * @brief  Implements pattern matching utilities for PruneCopy
 * 
 * @author Patrik Neunteufel
 * @date   April 2025
 *********************************************************************/

#include "util/PatternUtils.hpp"

namespace PatternUtils {

    std::vector<std::regex> wildcardsToRegex(const std::vector<std::string>& wildcards) {
        std::vector<std::regex> result;
        for (const auto& w : wildcards) {
            std::string r = std::regex_replace(w, std::regex(R"([.^$+(){}|\[\]\\])"), R"(\$&)");
            r = std::regex_replace(r, std::regex(R"(\*)"), ".*");
            r = std::regex_replace(r, std::regex(R"(\?)"), ".");
            result.emplace_back("^" + r + "$", std::regex_constants::icase);
        }
        return result;
    }

    bool matchesPattern(const std::string& filename, const std::vector<std::regex>& patterns) {
        for (const auto& pattern : patterns) {
            if (std::regex_match(filename, pattern)) return true;
        }
        return false;
    }
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
    
    std::vector<std::regex> PatternUtils::convertToRegex(const std::vector<std::string>& patterns) {
        std::vector<std::regex> result;
        for (const auto& pattern : patterns) {
            result.emplace_back(globToRegex(pattern), std::regex::icase);
        }
        return result;
    }


} // namespace PatternUtils
