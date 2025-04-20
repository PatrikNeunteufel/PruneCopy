/*****************************************************************//**
 * @file   ConvertUtils.cpp
 * @brief  
 * 
 * @author Patrik Neunteufel
 * @date   April 2025
 *********************************************************************/

#include "ConvertUtils.hpp"

 // Converts a vector of string-based paths into absolute filesystem paths.
 // Each string is wrapped in a fs::path and made absolute before being added to the result.
std::vector<fs::path> ConvertUtils::toPaths(const std::vector<std::string>& input) {
    std::vector<fs::path> result;
    for (const auto& s : input) {
        result.emplace_back(fs::absolute(fs::path(s)));
    }
    return result;
}
