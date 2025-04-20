/*****************************************************************//**
 * @file   Updater.hpp
 * @brief  Provides version check against GitHub version.json
 *
 * @author Patrik Neunteufel
 * @date   April 2025
 *********************************************************************/

#pragma once

#include <string>
#ifndef atwork 
#include <nlohmann/json.hpp>
#endif

 // Represents a semantic version number (major.minor.patch)
struct Version {
    int major = 0; // Major version: breaking changes
    int minor = 0; // Minor version: new features, backward-compatible
    int patch = 0; // Patch version: bug fixes or minor updates

    // Compares versions using semantic versioning rules
    bool operator<(const Version& other) const {
        if (major != other.major) return major < other.major;
        if (minor != other.minor) return minor < other.minor;
        return patch < other.patch;
    }

    // Converts version to string format: "X.Y.Z"
    std::string toString() const {
        return std::to_string(major) + "." +
            std::to_string(minor) + "." +
            std::to_string(patch);
    }

#ifndef atwork
    // Loads version data from JSON object (used in dev or release check context)
    static Version fromJson(const nlohmann::json& j) {
        return Version{
            j.value("Major", 0),
            j.value("Minor", 0),
            j.value("Patch", 0)
        };
    }
#endif
};


class Updater {
public:
    /**
     * @brief Checks if a newer version is available on GitHub.
     *
     * @param outDescription  Description of the new version (if newer)
     * @param outURL          URL to the download page (if newer)
     * @return true if an update is available
     */
    static bool checkForNewVersion(std::string& outDescription, std::string& outURL);
};