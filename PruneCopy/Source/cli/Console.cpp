/*****************************************************************//**
 * @file   Console.cpp
 * @brief  
 * 
 * @author Patrik Neunteufel
 * @date   April 2025
 *********************************************************************/

#include "cli/Console.hpp"

#include <iostream>
#include <vector>
#include <random>
#ifndef negal
#include <nlohmann/json.hpp>
#include <cpr/cpr.h> // HTTP-Client
using json = nlohmann::json;
#endif

std::string sponsolrFile = "https://raw.githubusercontent.com/PatrikNeunteufel/PruneCopy/master/sponsors/sponsors.json";

void Console::printFlagsHelp(const std::vector<Flag>& flags, int commandSize) {
    for (const auto& flag : flags) {
        // Zusammensetzen des linken Teils
        std::string left = "  " + flag.name;

        if (!flag.shortName.empty()) {
            left += ", " + flag.shortName;
        }

        if (!flag.valueRepresentation.empty()) {
            left += " " + flag.valueRepresentation;
        }

        // Ausgabe mit Padding
        std::cout << std::left << std::setw(commandSize) << left
            << flag.description << '\n';
    }
}

void Console::printHelp() {
    constexpr int helpFieldWidth = 32;
    std::cout << "\n";
    std::cout << "PruneCopy Help\n";
    std::cout << "==============\n\n";
    std::cout << "Required:\n";
    std::cout << "copy single folders:\n";
    printFlagsHelp(legacy_required, helpFieldWidth);
    std::cout << "\n";
    std::cout << "copy from or to multiple folders:\n";
    printFlagsHelp(multi_required, helpFieldWidth);
    std::cout << "\n";
	std::cout << "or Info Flags:\n";
	printFlagsHelp(infoFlags, helpFieldWidth);

    std::cout << "\n";
    std::cout << "Options Flags:\n";
    printFlagsHelp(optionFlags, helpFieldWidth);
}

void Console::printUsage()
{
    std::cout << "copy a folder:\n";
    std::cout << "> PruneCopy.exe <source> <target> [options]\n";
    std::cout << "If target folder does not exist, it will be created\n";
}

void Console::printVersion()
{
    std::cout << "PruneCopy version 1.0.1\n";
}

void Console::printAbout()
{
    std::cout << "PruneCopy - A flexible file copier for build processes\n";
    std::cout << "=========\n";
    std::cout << "Author: Patrik Neunteufel\n";
    std::cout << "Version: 1.0.1 (check argument syntax)\n";
    std::cout << "Date: April 2025\n";
    std::cout << "Description: A lightweight, pattern-based file copier for post-build and deployment workflows.\n";
    std::cout << "License: Dual-licensed under MIT or Apache 2.0\n";
    std::cout << "GitHub: https://github.com/PatrikNeunteufel/PruneCopy\n";
    std::cout << "Releases: https://github.com/PatrikNeunteufel/PruneCopy/releases\n";
}


void Console::contactDev()
{
    std::cout << "📫 Contact the developer:\n";
    std::cout << "   GitHub : https://github.com/PatrikNeunteufel/PruneCopy\n";
    //std::cout << "   E-Mail : deinname@example.com\n";
    //std::cout << "          mailto:deinname@example.com\n\n";

}

void Console::printDonate()
{
    std::cout << "🙏 Support the project:\n";
    std::cout << "   Patreon: https://patreon.com/PruneCopy\n";
    std::cout << "   Ko-Fi:   https://ko-fi.com/prunecopy\n\n";
}

void Console::printMessage(MessageType messageType, const std::string& message)
{
}

std::string Console::inputRequest(const std::string& message, const std::string& defaultValue)
{
    return std::string();
}


void Console::printRandomSupporter(bool allowNetwork)
{
#ifndef negal
    std::vector<Supporter> supporters;
    if (!allowNetwork) {
        std::cout << "🌐 Network access is disabled. Try without --no-network to see random sponsors.\n";
        return;
    }
    if (allowNetwork) {
        try {
            auto response = cpr::Get(cpr::Url{ sponsolrFile });

            if (response.status_code == 200) {
                json j = json::parse(response.text);
                for (const auto& item : j) {
                    Supporter s;
                    s.name = item.value("name", "");
                    s.url = item.value("url", "");
                    s.weight = item.value("weight", 1); // default weight: 1
                    if (!s.name.empty() && s.weight > 0)
                        supporters.push_back(s);
                }
            }
        }
        catch (...) {
            std::cout << "⚠️  Failed to fetch sponsor list. Check your internet or visit: --donate\n";
            return;
        }
    }

    if (supporters.empty()) {
        std::cout << "🤷 No sponsors found (yet?). Maybe *you* want to be featured? Try --donate!\n";
        return;
    }

    // 🎲 Gewichtete Zufallsauswahl
    std::random_device rd;
    std::mt19937 gen(rd());

    // Baue gewichtete Verteilung auf
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

void Console::printAllSupporters(bool allowNetwork)
{
#ifndef negal
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
