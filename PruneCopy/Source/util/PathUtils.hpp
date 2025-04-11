/*****************************************************************//**
 * @file   PathUtils.hpp
 * @brief  
 * 
 * @author Patrik Neunteufel
 * @date   April 2025
 *********************************************************************/

#pragma once
#include <filesystem>

namespace PathUtils {

	/**
	 * @brief Get the directory of the executable.
	 *
	 * @return The path to the directory of the executable.
	 */
	std::filesystem::path getExecutableDirectory();
}
