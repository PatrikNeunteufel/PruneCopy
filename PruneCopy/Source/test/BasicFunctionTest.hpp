/*****************************************************************//**
 * @file   BasicFunctionTest.hpp
 * @brief  
 * 
 * @author Patrik Neunteufel
 * @date   April 2025
 *********************************************************************/

#pragma once

#pragma once
#include <string>

  /**
   * @class BasicFunctionTest
   * @brief Provides tests for basic components such as path utilities, pattern conversion,
   *        type filtering, and internal PruneOptions parsing.
   */
class BasicFunctionTest {
public:
    /**
     * @brief Runs all available basic utility tests.
     * @return True if all tests pass, false otherwise.
     */
    static bool run();

private:
    /**
     * @brief Tests conversion of string-based file paths into filesystem paths.
     */
    static bool testConvertUtils();

    /**
     * @brief Tests resolution of the current executable directory.
     */
    static bool testPathUtils();

    /**
     * @brief Tests wildcard-to-regex conversion and regex matching behavior.
     */
    static bool testPatternUtils();

    /**
     * @brief Tests manual initialization and parsing behavior of PruneOptions.
     */
    static bool testPruneOptionsParsing();
};
