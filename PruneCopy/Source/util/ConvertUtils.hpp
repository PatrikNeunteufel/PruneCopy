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

namespace fs = std::filesystem;

namespace ConvertUtils {

    std::vector<fs::path> toPaths(const std::vector<std::string>& input);

}
