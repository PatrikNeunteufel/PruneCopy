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

bool BasicFunctionTest::run() {
    std::cout << "[BasicFunctionTest] Running tests..." << std::endl;
    bool success = true;

    success &= testConvertUtils();
    success &= testPathUtils();
    success &= testPatternUtils();
    success &= testPruneOptionsParsing();

    if (success)
        std::cout << "[BasicFunctionTest] All tests passed!" << std::endl;
    else
        std::cout << "[BasicFunctionTest] Some tests FAILED!" << std::endl;

    return success;
}

bool BasicFunctionTest::testConvertUtils() {
    std::vector<std::string> input = { "test/file.txt", "example/path" };
    auto paths = ConvertUtils::toPaths(input);

    bool success = TestUtils::assertTrue(paths.size() == input.size(), "ConvertUtils::toPaths size check");


    for (size_t i = 0; i < input.size(); ++i) {
        fs::path inputPath(input[i]);
        fs::path resultPath(paths[i]);

        bool filenameMatches = inputPath.filename() == resultPath.filename();
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



bool BasicFunctionTest::testPathUtils() {
    auto exeDir = PathUtils::getExecutableDirectory();
    return TestUtils::assertFalse(exeDir.empty(), "PathUtils::getExecutableDirectory not empty");
}

bool BasicFunctionTest::testPatternUtils() {
    bool success = true;

    auto pattern = PatternUtils::wildcardsToRegex({ "*.txt" });
    success &= TestUtils::assertTrue(
        PatternUtils::matchesPattern("test.txt", pattern),
        "PatternUtils matchesPattern positive (*.txt mit test.txt)"
    );

    success &= TestUtils::assertFalse(
        PatternUtils::matchesPattern("test.png", pattern),
        "PatternUtils matchesPattern negative (*.txt mit test.png)"
    );

    return success;
}

bool BasicFunctionTest::testPruneOptionsParsing() {
    PruneOptions options;

    options.types = { "*.cpp", "*.hpp" };
    options.excludeDirs = { "test" };
    options.flatten = true;
    options.parallelMode = ParallelMode::Async;

	// create explicite regex patterns for types
    options.typePatterns = { std::regex(R"(.*\.cpp)"), std::regex(R"(.*\.hpp)") };

    bool success = true;

    success &= TestUtils::assertTrue(options.types.size() == 2, "PruneOptions 'types' size check");
    success &= TestUtils::assertEqual(std::string("*.cpp"), options.types[0], "PruneOptions 'types' first entry");
    success &= TestUtils::assertTrue(options.excludeDirs.size() == 1, "PruneOptions 'excludeDirs' size check");
    success &= TestUtils::assertEqual(std::string("test"), options.excludeDirs[0], "PruneOptions 'excludeDirs' entry check");
    success &= TestUtils::assertTrue(options.flatten, "PruneOptions 'flatten' flag set");
    success &= TestUtils::assertTrue(options.parallelMode == ParallelMode::Async, "PruneOptions 'parallelMode' set to Async");
    success &= TestUtils::assertTrue(options.typePatterns.size() == 2, "PruneOptions 'typePatterns' size check");

    return success;
}
