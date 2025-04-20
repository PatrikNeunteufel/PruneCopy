/*****************************************************************//**
 * @file   ArgumentParseTest.hpp
 * @brief  Header for testing CLI argument parsing (ArgumentParser)
 *
 * @author Patrik Neunteufel
 * @date   April 2025
 *********************************************************************/

#pragma once

  /**
   * @class ArgumentParseTest
   * @brief Provides unit tests for verifying argument parsing behavior in PruneCopy.
   *
   * This class contains test cases that validate different CLI parsing modes,
   * deprecated flag detection, and color handling in the ArgumentParser.
   */
class ArgumentParseTest {
public:
    /**
     * @brief Runs all available argument parser tests.
     * @return True if all tests pass, false otherwise.
     */
    static bool run();

private:
    /**
     * @brief Tests legacy mode with positional source and destination arguments.
     * @return True if test passes.
     */
    static bool testLegacyMode();

    /**
     * @brief Tests full CLI mode using --source and --destination flags.
     * @return True if test passes.
     */
    static bool testFullCLIMode();

    /**
     * @brief Tests parsing and evaluation of the --color flag.
     * @return True if color mode is parsed correctly.
     */
    static bool testColorMode();

    /**
     * @brief Tests whether deprecated flags trigger appropriate warnings.
     * @return True if the warning is detected and contains expected content.
     */
    static bool testDeprecatedDetection();

    /**
     * @brief Tests that clearing the deprecated flag log suppresses further warnings.
     * @return True if no output is generated after clearing.
     */
    static bool testDeprecatedClear();
};

