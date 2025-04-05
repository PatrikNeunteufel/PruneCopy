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
#include <nlohmann/json.hpp>
#include <cpr/cpr.h> // HTTP-Client

using json = nlohmann::json;

std::string sponsolrFile = "https://raw.githubusercontent.com/PatrikNeunteufel/PruneCopy/master/sponsors/sponsors.json";


void Console::printHelp() {
    std::cout << "\n";
    std::cout << "PruneCopy Help\n";
    std::cout << "==============\n\n";
    std::cout << "Info as first and only argument:\n";
    std::cout << "  --help, -h                    Show this help message\n";
    std::cout << "  --usage                       Show usage information\n";
    std::cout << "  --version                     Show version information\n";
    std::cout << "  --about                       Show about information\n";
    std::cout << "  --contact-dev                 Contact the developer\n";
    std::cout << "  --donate                      This tool helps you and you want to support me\n";
    std::cout << "  --sponsors                    see the list of supporters\n";
    std::cout << "\n";
	std::cout << "Required:\n";
    std::cout << "  <source>                      Source directory as first argument requried (as long no Info requested)\n";
    std::cout << "  <destination>                 Destination directory as second argument required (as long no Info requested)\n";
    std::cout << "\n";
    std::cout << "Options:\n";
	std::cout << "  --no-network                  Disable network access (e.g. for sponsors list)\n";
    //std::cout << "  --additional-dest <paths>     (planned) Copy to multiple destinations directories additional to the required destination (e.g. --multi-dest C:/path1 C:/path2)\n";
    std::cout << "  --types <patterns>            Include only files matching given patterns (e.g. *.h *.hpp)\n";
    std::cout << "  --exclude-dirs <dirs>         Exclude directories by name\n";
    std::cout << "  --exclude-files <patterns>    Exclude files matching patterns (e.g. *Impl.hpp)\n";
    std::cout << "  --delete-target-first         Delete the entire target folder before copying\n";
    std::cout << "  --no-overwrite                Skip files that already exist\n";
    std::cout << "  --force-overwrite             Overwrite existing files without asking\n";
    std::cout << "  --cmdln-out-off               Suppress console output\n";
    std::cout << "  --log-dir <path>              Write operations to a log file in the specified folder\n";
    std::cout << "  --log-open                    Open the log file after the operation (only when --log-dir <path> is set)\n";
    std::cout << "  --log-level <level>           Set console log level: info (default), warning, error\n";
	//std::cout << "  --flatten                     (planned) Copy all files into a single target directory\n";
 //   std::cout << "  --flatten-suffix              (planned) same as --flatten but adds suffixes(e.g.folders) to prevent name clashes\n";
 //   std::cout << "  --parallel-async              (planned) Use async-based parallel file copying\n";
 //   std::cout << "  --parallel-thread             (planned) Use threaded parallel file copying\n";
 //   std::cout << "  --parallel-openMP             (planned) Use OpenMP for parallel copying\n";
    std::cout << "  --color <mode>                Console color output: auto (default), always, never\n";
    std::cout << "  --dry-run                     Show what would be copied without doing it\n";
    
}

void Console::printUsage()
{
    std::cout << "copy a folder:\n";
    std::cout << "> PruneCopy.exe <source> <target> [options]\n";
    std::cout << "If target folder does not exist, it will be created\n";
}

void Console::printVersion()
{
    std::cout << "PruneCopy version 1.0.0\n";
}

void Console::printAbout()
{
    std::cout << "PruneCopy - A flexible file copier for build processes\n";
    std::cout << "=========\n";
    std::cout << "Author: Patrik Neunteufel\n";
    std::cout << "Version: 1.0.0 (Initial Public Release)\n";
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
}

void Console::printAllSupporters(bool allowNetwork)
{
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
}
