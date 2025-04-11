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
#include <nlohmann/json.hpp>
#include <cpr/cpr.h>
#include <iostream>

namespace fs = std::filesystem;


    bool Updater::checkForNewVersion(std::string& outDescription, std::string& outURL) {
        const fs::path localPath = PathUtils::getExecutableDirectory() / "version.json";
        const std::string githubURL = "https://raw.githubusercontent.com/patrikNeunteufel/PruneCopy/master/data/version.json";

        // 1. Lokale Version einlesen
        if (!fs::exists(localPath)) return false;

        nlohmann::json localJson;
        try {
            std::ifstream in(localPath);
            in >> localJson;
        }
        catch (...) {
            return false;
        }
        Version localVersion = Version::fromJson(localJson);

        // 2. Remote-Version laden via CPR
        cpr::Response response = cpr::Get(cpr::Url{ githubURL });
        if (response.status_code != 200) return false;

        nlohmann::json remoteJson;
        try {
            remoteJson = nlohmann::json::parse(response.text);
        }
        catch (...) {
            return false;
        }

        Version remoteVersion = Version::fromJson(remoteJson);

        // 3. Vergleich
        if (localVersion < remoteVersion) {
            outDescription = remoteJson.value("Description", "No changelog available");
            outURL = remoteJson.value("URL", "https://github.com/patrikNeunteufel/PruneCopy");
            return true;
        }

        return false;
    }

