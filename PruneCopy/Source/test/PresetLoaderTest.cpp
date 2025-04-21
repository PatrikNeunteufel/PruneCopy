/*****************************************************************//**
 * @file   PresetLoaderTest.cpp
 * @brief  Unit tests for the PresetLoader module
 *
 * These tests cover saving, loading, listing, and showing presets
 * using temporary mock options for CLI equivalence verification.
 *
 * @author Patrik Neunteufel
 * @date   April 2025
 *********************************************************************/

#include "test/PresetLoaderTest.hpp"

#include "cli/PresetLoader.hpp"
#include "cli/ArgumentParser.hpp"
#include "core/PruneOptions.hpp"
#include "test/TestUtils.hpp"

#include <iostream>
#include <algorithm>

namespace fs = std::filesystem;

// Helper to create mock options
static PruneOptions createMockOptions() {
	PruneOptions opt;
	opt.sources = { "srcA", "srcB" };
	opt.destinations = { "dstA" };
	opt.types = { "*.h", "*.cpp" };
	opt.excludeDirs = { "build", "temp" };
	opt.excludeFiles = { "*Test.cpp" };
	opt.flatten = true;
	opt.colorMode = ColorMode::Always;
	opt.logLevel = LogLevel::Standard;
	opt.logDir = "mockLogs";
	opt.enableLogging = true;
	return opt;
}

// Entry point for all tests
bool PresetLoaderTest::run() {
	std::cout << "[PresetLoaderTest] Running tests...\n";
	bool success = true;

	success &= testSaveAndLoadPreset();
	success &= testListPresets();
	success &= testShowPreset();

	if (success)
		std::cout << "[PresetLoaderTest] All tests passed!\n";
	else
		std::cout << "[PresetLoaderTest] Some tests FAILED!\n";

	return success;
}

bool PresetLoaderTest::testSaveAndLoadPreset() {
	std::string name = "TestConfig_A";
	PruneOptions original = createMockOptions();

	bool success = true;

	success &= TestUtils::assertTrue(PresetLoader::savePreset(name, original), "SavePreset: save successful");

	auto loaded = PresetLoader::loadPreset(name);
	success &= TestUtils::assertTrue(loaded.has_value(), "SavePreset: load successful");

	if (loaded.has_value()) {
		auto originalArgs = ArgumentParser::rebuildArgumentsFromOptions(original);
		auto loadedArgs = ArgumentParser::rebuildArgumentsFromOptions(loaded.value());

		// Normalize paths to filenames (or use relative paths)
		for (std::string& s : originalArgs) {
			if (s.find(":\\") != std::string::npos)
				s = fs::path(s).filename().string(); // strip to filename (or use relative)
		}
		for (std::string& s : loadedArgs) {
			if (s.find(":\\") != std::string::npos)
				s = fs::path(s).filename().string();
		}

		success &= TestUtils::assertEqual(originalArgs, loadedArgs, "SavePreset: CLI args match");
	}

	return success;
}

bool PresetLoaderTest::testListPresets() {
	std::vector<std::string> presets = PresetLoader::listPresets();
	bool success = true;

	success &= TestUtils::assertFalse(presets.empty(), "ListPresets: result not empty");
	success &= TestUtils::assertTrue(std::find(presets.begin(), presets.end(), "TestConfig_A") != presets.end(),
		"ListPresets: contains 'TestConfig_A'");

	return success;
}

bool PresetLoaderTest::testShowPreset() {
	std::string cli = PresetLoader::showPreset("TestConfig_A");

	bool success = true;
	success &= TestUtils::assertFalse(cli.empty(), "ShowPreset: output not empty");
	success &= TestUtils::assertContains(cli, "--types", "ShowPreset: contains --types");
	success &= TestUtils::assertContains(cli, "--exclude-dirs", "ShowPreset: contains --exclude-dirs");

	return success;
}
