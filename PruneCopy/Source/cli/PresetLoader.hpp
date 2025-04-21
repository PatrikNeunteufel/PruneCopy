/*****************************************************************//**
 * @file   PresetLoader.hpp
 * @brief  Loads, saves and manages PruneCopy presets from JSON files
 *
 * @author Patrik Neunteufel
 * @date   April 2025
 *********************************************************************/
#pragma once

#include <string>
#include <vector>
#include <optional>
#include <filesystem>

#include "core/PruneOptions.hpp"

class PresetLoader {
	/* ******* */
	/* members */
	/* ******* */
public: /* public static methodes */

	/**
	 * @brief Returns the directory where presets are stored (e.g. ./presets/)
	 */
	static std::filesystem::path getPresetDir();

	/**
	 * @brief Loads a preset from a JSON file and returns a PruneOptions object
	 * @param name The name of the preset (without .json)
	 * @return Loaded PruneOptions or std::nullopt if loading failed
	 */
	static std::optional<PruneOptions> loadPreset(const std::string& name);

	/**
	 * @brief Saves the given options to a JSON file with the given name
	 * @param name The name for the preset file (without .json)
	 * @param options The PruneOptions to save
	 * @return True if successful, false otherwise
	 */
	static bool savePreset(const std::string& name, const PruneOptions& options);

	/**
	 * @brief Lists all available preset names in the preset directory
	 * @return Vector of preset names (without .json)
	 */
	static std::vector<std::string> listPresets();

	/**
	 * @brief Converts a preset into a representative command line string
	 * @param options The preset to show
	 * @return CLI string representation (e.g. "--types *.h --exclude-dirs Pch")
	 */
	static std::string generateCommandLineFromPreset(const PruneOptions& options);

	/**
	 * @brief Loads and converts a preset directly to command line string
	 * @param name The name of the preset to show
	 * @return CLI string or empty string on error
	 */
	static std::string showPreset(const std::string& name);
};
