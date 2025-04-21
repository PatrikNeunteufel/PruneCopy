/*****************************************************************//**
 * @file   FileCopierTest.hpp
 * @brief  
 * 
 * @author Patrik Neunteufel
 * @date   April 2025
 *********************************************************************/
#pragma once
#include "core/PruneOptions.hpp"
#include <filesystem>

namespace fs = std::filesystem;

 /**
  * @class FileCopierTest
  * @brief Contains test cases for validating core functionality of the FileCopier module.
  *
  * Tests include file filtering, directory exclusion, pattern matching,
  * overwrite protection, and edge case validation.
  */
class FileCopierTest {
public:
    /**
     * @brief Runs all available FileCopier tests.
     * @return True if all tests pass, false otherwise.
     */
    static bool run();

private:
    /**
     * @brief Tests filtered file copying including directory and pattern exclusions.
     */
    static bool testCopyFiltered();

    /**
     * @brief Tests logic for excluding specific directories during traversal.
     */
    static bool testExcludeDir();

    /**
     * @brief Tests wildcard pattern matching on filenames.
     */
    static bool testMatchesPattern();

    /**
     * @brief Tests falsification of overwrite protection (intentionally expected to fail).
     */
    static bool testOverwriteFalsify();

    /**
     * @brief Sets up the file structure for testing file copying behavior.
     * @param testRoot The root path of the test environment.
     * @param srcDir The source directory to be populated.
     */
    static void setupTestEnvironment(const fs::path& testRoot, const fs::path& srcDir);

    /**
     * @brief Cleans up the test directory structure after test execution.
     * @param testRoot The root path of the test environment to delete.
     */
    static void cleanupTestEnvironment(const fs::path& testRoot);
    
    /**
     * @brief Tests flatten-suffix naming behavior.
     */
    static bool testFlattenSuffixNaming();

    /**
     * @brief Tests auto-renaming behavior in flatten mode.
     */
    static bool testFlattenAutoRename();
};
