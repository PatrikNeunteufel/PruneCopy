/*****************************************************************//**
 * @file   Updater.cpp
 * @brief  Implements version check against GitHub version.json
 *
 * @author Patrik Neunteufel
 * @date   April 2025
 *********************************************************************/

#include "core/Updater.hpp"

#include "util/PathUtils.hpp"

#include <fstream>
#include <filesystem>
#include <iostream>
#ifndef atwork
#include <nlohmann/json.hpp>
#include <cpr/cpr.h>
#endif

namespace fs = std::filesystem;


// Checks if a newer version is available online by comparing with local version
bool Updater::checkForNewVersion(std::string& outDescription, std::string& outURL) {
    const fs::path localPath = PathUtils::getExecutableDirectory() / "version.json";
    const std::string githubURL = "https://raw.githubusercontent.com/patrikNeunteufel/PruneCopy/master/data/version.json";

    // 1. Read local version file
    if (!fs::exists(localPath)) return false;

#ifndef atwork
    nlohmann::json localJson;
    try {
        std::ifstream in(localPath);
        in >> localJson;
    }
    catch (...) {
        return false; // Failed to read or parse local version file
    }

    Version localVersion = Version::fromJson(localJson);

    // 2. Download latest version info from GitHub
    cpr::Response response = cpr::Get(cpr::Url{ githubURL });
    if (response.status_code != 200) return false;

    nlohmann::json remoteJson;
    try {
        remoteJson = nlohmann::json::parse(response.text);
    }
    catch (...) {
        return false; // Invalid or corrupt remote JSON
    }

    Version remoteVersion = Version::fromJson(remoteJson);

    // 3. Compare versions and return result if update is available
    if (localVersion < remoteVersion) {
        outDescription = remoteJson.value("Description", "No changelog available");
        outURL = remoteJson.value("URL", "https://github.com/patrikNeunteufel/PruneCopy");
        return true;
    }
#endif

    return false; // No update available or check disabled via `atwork`
}

