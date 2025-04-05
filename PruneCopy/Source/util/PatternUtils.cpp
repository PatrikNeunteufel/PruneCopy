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

} // namespace PatternUtils
