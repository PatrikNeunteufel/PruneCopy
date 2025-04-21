/*****************************************************************//**
 * @file   Console.cpp
 * @brief  
 * 
 * @author Patrik Neunteufel
 * @date   April 2025
 *********************************************************************/

#include "cli/Console.hpp"
#include "core/Updater.hpp"
#include "util/PathUtils.hpp"

#include <iostream>
#include <vector>
#include <random>
#include <filesystem>

namespace fs = std::filesystem;

#ifndef atwork  // just a define to disable some features that are not available on another development machine
#include <nlohmann/json.hpp>
#include <cpr/cpr.h> // HTTP-Client
using json = nlohmann::json;
#endif

// Remote JSON file containing supporter data
std::string sponsolrFile = "https://raw.githubusercontent.com/PatrikNeunteufel/PruneCopy/master/data/sponsors.json";

// Prints a formatted list of flags with optional short names and value representations
void Console::printFlagsHelp(const std::vector<Flag>& flags, int commandSize) {
    for (const auto& flag : flags) {
        std::string left = "  " + flag.name;
        if (!flag.shortName.empty()) {
            left += ", " + flag.shortName;
        }
        if (!flag.valueRepresentation.empty()) {
            left += " " + flag.valueRepresentation;
        }
        std::cout << std::left << std::setw(commandSize) << left
            << flag.description << '\n';
    }
}

// Displays the main help screen, grouped by required and optional flags
void Console::printHelp() {
    constexpr int helpFieldWidth = 32;
    std::cout << "\n";
    std::cout << "PruneCopy Help\n";
    std::cout << "==============\n\n";
    std::cout << "Required:\n";
    std::cout << "copy single folders:\n";
    printFlagsHelp(legacy_required, helpFieldWidth);
    std::cout << "\ncopy from or to multiple folders:\n";
    printFlagsHelp(multi_required, helpFieldWidth);
    std::cout << "\nor Info Flags:\n";
    printFlagsHelp(infoFlags, helpFieldWidth);
    std::cout << "\nOptions Flags:\n";
    printFlagsHelp(optionFlags, helpFieldWidth);
	std::cout << "\nPreset Flags:\n";
	printFlagsHelp(presetFlags, helpFieldWidth);
	std::cout << "\nDeveloper Flags:\n";
	printFlagsHelp(developerFlags, helpFieldWidth);
}

// Shows usage examples for legacy, hybrid, and full CLI modes
void Console::printUsage() {
    std::cout << "copy a folder:\n";
    std::cout << "  single source to single destination:\n";
    std::cout << "> PruneCopy.exe <source> <destination> [options]\n";
    std::cout << "  single source to single/multiple destination:\n";
    std::cout << "> PruneCopy.exe <source> --destination <destinations> [options]\n";
    std::cout << "  single/multiple source to single/multiple destination:\n";
    std::cout << "> PruneCopy.exe --source <sources> --destination <destinations> [options]\n";
    std::cout << "If a destination folder does not exist, it will be created\n";
}

// Checks for newer versions online and displays update info if available
void Console::printUpdate() {
    std::string description, url;
    if (Updater::checkForNewVersion(description, url)) {
        std::cout << "📢 A new version of PruneCopy is available!\n\n";
        std::cout << "🆕 What's new:\n" << description << "\n\n";
        std::cout << "🔗 Download: " << url << "\n";
        std::cout << "📎 Tip: Use --check-update to verify manually in the future.\n";
    }
    else {
        std::cout << "✅ You are using the latest version of PruneCopy.\n";
    }
}

// Displays version information from the local version.json file
void Console::printVersion() {
    const fs::path localPath = PathUtils::getExecutableDirectory() / "version.json";

    if (!fs::exists(localPath)) {
        std::cout << "Version: unknown\n";
        return;
    }

    try {
#ifndef atwork
        std::ifstream in(localPath);
        nlohmann::json j;
        in >> j;

        int major = j.value("Major", 0);
        int minor = j.value("Minor", 0);
        int patch = j.value("Patch", 0);
        std::string date = j.value("Date", "unknown");
        std::string description = j.value("Description", "");

        std::cout << "Version: " << major << "." << minor << "." << patch;
        if (!description.empty()) {
            std::cout << " (" << description << ")";
        }
        std::cout << "\n";
        std::cout << "Date: " << date << "\n";
#endif
    }
    catch (...) {
        std::cout << "Version: unknown (failed to parse version.json)\n";
    }
}

// Prints about section including author, license, and project links
void Console::printAbout() {
    std::cout << "PruneCopy - A flexible file copier for build processes\n";
    std::cout << "=========\n";
    std::cout << "Author: Patrik Neunteufel\n";
    printVersion();
    std::cout << "Description: A lightweight, pattern-based file copier for post-build and deployment workflows.\n";
    std::cout << "License: Dual-licensed under MIT or Apache 2.0\n";
    std::cout << "GitHub: https://github.com/PatrikNeunteufel/PruneCopy\n";
    std::cout << "Releases: https://github.com/PatrikNeunteufel/PruneCopy/releases\n";
}

// Displays developer contact info
void Console::contactDev() {
    std::cout << "📫 Contact the developer:\n";
    std::cout << "   GitHub : https://github.com/PatrikNeunteufel/PruneCopy\n";
    // Uncomment to add email contact
}

// Displays donation links for community support
void Console::printDonate() {
    std::cout << "🙏 Support the project:\n";
    std::cout << "   Patreon: https://patreon.com/PruneCopy\n";
    std::cout << "   Ko-Fi:   https://ko-fi.com/prunecopy\n\n";
}


// Placeholder for standardized message output (info, warning, etc.)
void Console::printMessage(MessageType messageType, const std::string& message) {
    // TODO: Implement message formatting and output
}

// Placeholder for input prompts with optional default values
std::string Console::inputRequest(const std::string& message, const std::string& defaultValue) {
	// 
    return std::string();
}

// Downloads sponsor list from GitHub and displays one random supporter (weighted)
void Console::printRandomSupporter(bool allowNetwork) {
#ifndef atwork
    std::vector<Supporter> supporters;
    if (!allowNetwork) {
        std::cout << "🌐 Network access is disabled. Try without --no-network to see random sponsors.\n";
        return;
    }

    try {
        auto response = cpr::Get(cpr::Url{ sponsolrFile });

        if (response.status_code == 200) {
            json j = json::parse(response.text);
            for (const auto& item : j) {
                Supporter s;
                s.name = item.value("name", "");
                s.url = item.value("url", "");
                s.weight = item.value("weight", 1);
                if (!s.name.empty() && s.weight > 0)
                    supporters.push_back(s);
            }
        }
    }
    catch (...) {
        std::cout << "⚠️  Failed to fetch sponsor list. Check your internet or visit: --donate\n";
        return;
    }

    if (supporters.empty()) {
        std::cout << "🤷 No sponsors found (yet?). Maybe *you* want to be featured? Try --donate!\n";
        return;
    }

    // 🎲 Weighted random selection based on supporter weight
    std::random_device rd;
    std::mt19937 gen(rd());

    std::vector<int> weights;
    for (const auto& s : supporters) weights.push_back(s.weight);
    std::discrete_distribution<> dist(weights.begin(), weights.end());

    const auto& s = supporters[dist(gen)];

    std::cout << "🙏 Special thanks to our supporter: ";
    std::cout << s.name;
    if (!s.url.empty())
        std::cout << " (" << s.url << ")";
    std::cout << "\n";
#endif
}

// Downloads and displays the full list of supporters from GitHub
void Console::printAllSupporters(bool allowNetwork) {
#ifndef atwork
    if (!allowNetwork) {
        std::cout << "🌐 Network access is disabled. Use --sponsors without --no-network.\n";
        return;
    }

    try {
        auto response = cpr::Get(cpr::Url{ sponsolrFile });

        if (response.status_code != 200) {
            std::cout << "⚠️  Could not fetch sponsor list (HTTP " << response.status_code << "). Try again later or check --donate\n";
            return;
        }

        json j = json::parse(response.text);
        if (!j.is_array() || j.empty()) {
            std::cout << "🤷 No sponsors found. Maybe *you* want to be the first? See --donate!\n";
            return;
        }

        std::cout << "🙏 Current supporters of the PruneCopy project:\n";
        for (const auto& item : j) {
            const std::string name = item.value("name", "");
            const std::string url = item.value("url", "");
            const int weight = item.value("weight", 1);

            if (name.empty() || weight <= 0) continue;

            std::cout << " - " << name;
            if (!url.empty()) std::cout << " (" << url << ")";
            std::cout << "\n";
        }
    }
    catch (...) {
        std::cout << "⚠️  Failed to fetch sponsors. Maybe you're offline? See --donate\n";
    }
#endif
}