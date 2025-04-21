/*****************************************************************//**
 * @file   PresetLoader.cpp
 * @brief  Implements loading, saving and listing of PruneCopy presets.
 *         Presets are stored as simple JSON files containing CLI args.
 *
 * @author Patrik Neunteufel
 * @date   April 2025
 *********************************************************************/

#include "cli/PresetLoader.hpp"
#include "cli/ArgumentParser.hpp"
#include "log/LogManager.hpp"

#ifndef atwork
#include <nlohmann/json.hpp>
#endif

using namespace std;
namespace fs = std::filesystem;

#ifndef atwork
using json = nlohmann::json;
#endif

// Helper function to get environment variable (TODO: move later to a better utils)
std::optional<std::string> getEnvVar(const char* name) {
    char* buffer = nullptr;
    size_t size = 0;
    if (_dupenv_s(&buffer, &size, name) == 0 && buffer != nullptr) {
        std::string result(buffer);
        free(buffer); // _dupenv_s allokiert mit malloc → mit free freigeben
        return result;
    }
    return std::nullopt;
}


// Static helper to resolve preset directory
fs::path PresetLoader::getPresetDir() {
    if (auto env = getEnvVar("PRUNECOPY_PRESET_DIR")) {
        return fs::path(*env);
    }
#ifdef _WIN32
    return fs::path(*getEnvVar("APPDATA")) / "PruneCopy" / "presets";
#else
    return fs::path(*getEnvVar("HOME")) / ".config" / "prunecopy" / "presets";
#endif
}


std::optional<PruneOptions> PresetLoader::loadPreset(const std::string& name) {
#ifndef atwork
    fs::path file = getPresetDir() / (name + ".json");
    if (!fs::exists(file)) {
        LogManager::log(LogType::Error, "Preset file not found: " + file.string());
        return std::nullopt;
    }

    try {
        std::ifstream in(file);
        json j;
        in >> j;

        if (!j.contains("args") || !j["args"].is_array()) {
            LogManager::log(LogType::Error, "Preset format invalid (missing 'args') in: " + file.string());
            return std::nullopt;
        }

        // Convert JSON to vector<string>
        std::vector<std::string> flatArgs = j["args"].get<std::vector<std::string>>();

        // Fake argv/argc
        std::vector<const char*> argv;
        argv.push_back("prunecopy"); // dummy executable
        for (const auto& arg : flatArgs) {
            argv.push_back(arg.c_str());
        }

        PruneOptions opts;
        ArgumentParser parser;
        ParsedCliControl controlFlags;
        parser.parse(static_cast<int>(argv.size()), const_cast<char**>(argv.data()), opts, controlFlags);
        return opts;
    }
    catch (const std::exception& ex) {
        LogManager::log(LogType::Error, "Failed to load preset: " + std::string(ex.what()));
        return std::nullopt;
    }
#else
    LogManager::log(LogType::Warning, "Preset loading is disabled on this machine.");
    return std::nullopt;
#endif
}

bool PresetLoader::savePreset(const std::string& name, const PruneOptions& options) {
#ifndef atwork
    fs::path dir = getPresetDir();
    fs::create_directories(dir);

    fs::path file = dir / (name + ".json");

    ArgumentParser parser;
    std::vector<std::string> args = parser.rebuildArgumentsFromOptions(options);

    json j;
    j["args"] = args;

    try {
        std::ofstream out(file);
        out << j.dump(4); // pretty print with indent
        LogManager::log(LogType::Success, "Preset saved: " + file.string());
        return true;
    }
    catch (const std::exception& ex) {
        LogManager::log(LogType::Error, "Failed to save preset: " + std::string(ex.what()));
        return false;
    }
#else
    LogManager::log(LogType::Warning, "Preset saving is disabled on this machine.");
    return false;
#endif
}

std::vector<std::string> PresetLoader::listPresets() {
    std::vector<std::string> names;
    fs::path dir = getPresetDir();

    if (!fs::exists(dir)) {
        return names;
    }

    for (const auto& entry : fs::directory_iterator(dir)) {
        if (entry.path().extension() == ".json") {
            names.push_back(entry.path().stem().string());
        }
    }

    std::sort(names.begin(), names.end());
    return names;
}

std::string PresetLoader::generateCommandLineFromPreset(const PruneOptions& options) {
    ArgumentParser parser;
    const std::vector<std::string> args = parser.rebuildArgumentsFromOptions(options);

    std::ostringstream oss;
    for (const auto& arg : args) {
        oss << arg << " ";
    }

    return oss.str();
}

std::string PresetLoader::showPreset(const std::string& name) {
    const auto loaded = loadPreset(name);
    if (!loaded.has_value()) {
        return {};
    }

    return generateCommandLineFromPreset(loaded.value());
}
