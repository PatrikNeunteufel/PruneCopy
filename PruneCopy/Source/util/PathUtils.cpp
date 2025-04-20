/*****************************************************************//**
 * @file   PathUtils.cpp
 * @brief  
 * 
 * @author Patrik Neunteufel
 * @date   April 2025
 *********************************************************************/

#include "util/PathUtils.hpp"

#ifdef _WIN32
#include <windows.h>
#endif

 // Returns the directory where the current executable resides.
 // On Windows, uses GetModuleFileNameW; on other platforms, falls back to current working directory.
std::filesystem::path PathUtils::getExecutableDirectory() {
#ifdef _WIN32
    wchar_t buffer[MAX_PATH];
    GetModuleFileNameW(nullptr, buffer, MAX_PATH);
    return std::filesystem::path(buffer).parent_path();
#else
    return std::filesystem::current_path(); // TODO: Replace with platform-specific logic
#endif
}

