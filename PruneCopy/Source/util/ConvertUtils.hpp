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

    /**
     * @brief Converts a list of string paths into absolute filesystem paths.
     *
     * Each input string is interpreted as a path, converted to a `std::filesystem::path`,
     * and then transformed into its absolute form using `fs::absolute()`.
     *
     * @param input A vector of string-based paths (e.g. relative or absolute file/folder paths)
     * @return A vector of absolute filesystem paths
     */
    std::vector<std::filesystem::path> toPaths(const std::vector<std::string>& input);

}
