/*****************************************************************//**
 * @file   ConvertUtils.hpp
 * @brief  
 * 
 * @author Patrik Neunteufel
 * @date   April 2025
 *********************************************************************/

#pragma once

#include <vector>
#include <string>
#include <filesystem>

namespace ConvertUtils {

    namespace fs = std::filesystem;

    inline std::vector<fs::path> toPaths(const std::vector<std::string>& input) {
        std::vector<fs::path> result;
        for (const auto& s : input) {
            result.emplace_back(fs::absolute(fs::path(s)));
        }
        return result;
    }

}
