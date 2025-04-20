/*****************************************************************//**
 * @file   BasicFunctionTest.cpp
 * @brief  
 * 
 * @author Patrik Neunteufel
 * @date   April 2025
 *********************************************************************/

#include "BasicFunctionTest.hpp"
#include "TestUtils.hpp"
#include "../util/ConvertUtils.hpp"
#include "../util/PathUtils.hpp"
#include "../util/PatternUtils.hpp"
#include "../core/PruneOptions.hpp"

#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

// Runs all test cases related to utility functions and basic functionality
bool BasicFunctionTest::run() {
    std::cout << "[BasicFunctionTest] Running tests..." << std::endl;
    bool success = true;

    // Validate string-to-path conversions
    success &= testConvertUtils();

    // Validate platform path handling logic
    success &= testPathUtils();

    // Validate wildcard and regex pattern utility logic
    success &= testPatternUtils();

    // Validate default state and parsing logic of PruneOptions
    success &= testPruneOptionsParsing();

    if (success)
        std::cout << "[BasicFunctionTest] All tests passed!" << std::endl;
    else
        std::cout << "[BasicFunctionTest] Some tests FAILED!" << std::endl;

    return success;
}

// Tests conversion of string paths to std::filesystem::path objects via ConvertUtils
bool BasicFunctionTest::testConvertUtils() {
    std::vector<std::string> input = { "test/file.txt", "example/path" };
    auto paths = ConvertUtils::toPaths(input);

    // Basic check: number of output paths should match number of input strings
    bool success = TestUtils::assertTrue(paths.size() == input.size(), "ConvertUtils::toPaths size check");

    for (size_t i = 0; i < input.size(); ++i) {
        fs::path inputPath(input[i]);
        fs::path resultPath(paths[i]);

        // Compare file name components
        bool filenameMatches = inputPath.filename() == resultPath.filename();

        // Compare the last directory component of the parent path
        bool parentPathMatches = inputPath.parent_path().filename() == resultPath.parent_path().filename();

        success &= TestUtils::assertTrue(
            filenameMatches,
            "ConvertUtils::toPaths filename match check (Input: " + inputPath.filename().string() +
            ", Got: " + resultPath.filename().string() + ")"
        );

        success &= TestUtils::assertTrue(
            parentPathMatches,
            "ConvertUtils::toPaths parent path match check (Input: " + inputPath.parent_path().string() +
            ", Got: " + resultPath.parent_path().string() + ")"
        );
    }

    return success;
}




// Tests whether PathUtils returns a non-empty executable directory path
bool BasicFunctionTest::testPathUtils() {
    auto exeDir = PathUtils::getExecutableDirectory();

    // Ensure executable directory can be resolved and is not empty
    return TestUtils::assertFalse(exeDir.empty(), "PathUtils::getExecutableDirectory not empty");
}

// Tests wildcard-to-regex conversion and basic pattern matching behavior
bool BasicFunctionTest::testPatternUtils() {
    bool success = true;

    auto pattern = PatternUtils::wildcardsToRegex({ "*.txt" });

    // Check positive case: *.txt matches test.txt
    success &= TestUtils::assertTrue(
        PatternUtils::matchesPattern("test.txt", pattern),
        "PatternUtils matchesPattern positive (*.txt with test.txt)"
    );

    // Check negative case: *.txt should not match test.png
    success &= TestUtils::assertFalse(
        PatternUtils::matchesPattern("test.png", pattern),
        "PatternUtils matchesPattern negative (*.txt with test.png)"
    );

    return success;
}

// Tests manual setup and internal consistency of a PruneOptions instance
bool BasicFunctionTest::testPruneOptionsParsing() {
    PruneOptions options;

    // Set sample values
    options.types = { "*.cpp", "*.hpp" };
    options.excludeDirs = { "test" };
    options.flatten = true;
    options.parallelMode = ParallelMode::Async;

    // Simulate pre-parsed regex patterns
    options.typePatterns = {
        std::regex(R"(.*\.cpp)"),
        std::regex(R"(.*\.hpp)")
    };

    bool success = true;

    // Validate all assigned values
    success &= TestUtils::assertTrue(options.types.size() == 2, "PruneOptions 'types' size check");
    success &= TestUtils::assertEqual(std::string("*.cpp"), options.types[0], "PruneOptions 'types' first entry");
    success &= TestUtils::assertTrue(options.excludeDirs.size() == 1, "PruneOptions 'excludeDirs' size check");
    success &= TestUtils::assertEqual(std::string("test"), options.excludeDirs[0], "PruneOptions 'excludeDirs' entry check");
    success &= TestUtils::assertTrue(options.flatten, "PruneOptions 'flatten' flag set");
    success &= TestUtils::assertTrue(options.parallelMode == ParallelMode::Async, "PruneOptions 'parallelMode' set to Async");
    success &= TestUtils::assertTrue(options.typePatterns.size() == 2, "PruneOptions 'typePatterns' size check");

    return success;
}
