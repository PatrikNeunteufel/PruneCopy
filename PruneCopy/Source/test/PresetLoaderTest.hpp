/*****************************************************************//**
 * @file   PresetLoaderTest.hpp
 * @brief  Unit tests for the PresetLoader module
 *
 * This header declares tests that validate saving, loading, listing,
 * and showing presets using mock configurations.
 *
 * @author ChatGPT
 * @date   April 2025
 *********************************************************************/
#pragma once



class PresetLoaderTest {
public:
    /**
     * @brief Runs all unit tests related to the PresetLoader.
     */
    static bool run();

    /**
     * @brief Tests saving and loading a preset and verifies content matches.
     */
    static bool testSaveAndLoadPreset();

    /**
     * @brief Tests listing presets after saving one.
     */
    static bool testListPresets();

    /**
     * @brief Tests generating a CLI string from a saved preset.
     */
    static bool testShowPreset();

};