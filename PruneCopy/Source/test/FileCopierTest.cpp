/*****************************************************************//**
 * @file   FileCopierTest.cpp
 * @brief  
 * 
 * @author Patrik Neunteufel
 * @date   April 2025
 *********************************************************************/
#include "FileCopierTest.hpp"
#include "TestUtils.hpp"
#include "core/FileCopier.hpp"
#include "util/PatternUtils.hpp"

#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>

 
// Executes all test cases related to FileCopier functionality
bool FileCopierTest::run() {
    bool success = true;

    // Test filtered copy operation (incl. pattern filtering and exclusions)
    success &= testCopyFiltered();

    // Test directory exclusion based on folder names
    success &= testExcludeDir();

    // Test file pattern matching logic
    success &= testMatchesPattern();

    // Test flatten-suffix prefix behavior
    success &= testFlattenSuffixNaming();

    // Test auto-renaming logic in flatten mode
    success &= testFlattenAutoRename();

    // Optional: deliberately failing overwrite test
    // success &= testOverwriteFalsify();

    if (success)
        std::cout << "[FileCopierTest] All tests passed!" << std::endl;
    else
        std::cout << "[FileCopierTest] Some tests FAILED!" << std::endl;

    return success;
}
// Sets up a temporary test folder structure with files and subdirectories
void FileCopierTest::setupTestEnvironment(const fs::path& testRoot, const fs::path& srcDir) {
    fs::remove_all(testRoot);
    fs::create_directories(srcDir / "subdir");
    fs::create_directories(srcDir / "build" / "temp");

    // Files to be included in copy
    std::ofstream(srcDir / "file1.txt") << "Test content A";
    std::ofstream(srcDir / "file2.tmp") << "Excluded file";
    std::ofstream(srcDir / "subdir" / "file3.cpp") << "int main() {}"; // comment out to test copy failure

    // This file should be excluded due to excluded directory
    std::ofstream(srcDir / "build" / "temp" / "excluded.cpp") << "// should be excluded due to folder";
}

// Attempts to clean up test folder structure, retrying on failure
void FileCopierTest::cleanupTestEnvironment(const fs::path& testRoot) {
    for (int i = 0; i < 5; ++i) {
        try {
            fs::remove_all(testRoot);
            return;
        }
        catch (const fs::filesystem_error& e) {
            std::cerr << "[WARNING] cleanup attempt " << (i + 1) << " failed: " << e.what() << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
    std::cerr << "[ERROR] cleanupTestEnvironment failed permanently for: " << testRoot << std::endl;
}


// Tests whether the copy operation respects include/exclude patterns and directory filters
bool FileCopierTest::testCopyFiltered() {
    const fs::path testRoot = "test_workspace";
    const fs::path srcDir = testRoot / "source";
    const fs::path dstDir = testRoot / "destination";

    // Set up test folder structure with test files
    setupTestEnvironment(testRoot, srcDir);

    // Prepare PruneOptions with pattern and folder exclusions
    PruneOptions options;
    options.sources = { srcDir };
    options.destinations = { dstDir };
    options.typePatterns = PatternUtils::wildcardsToRegex({ "*.txt", "*.cpp" });  // allowed types
    options.excludeFilePatterns = PatternUtils::wildcardsToRegex({ "*.tmp" });    // exclude *.tmp
    options.excludeDirs = { "build" };                                            // exclude build folders
    options.quiet = true; // deprecated → suppress output

    // Perform filtered copy
    std::ofstream logFile("copy_log.txt");
    FileCopier::copyFiltered(options, &logFile);
    logFile.close();

    // Verify expected results
    bool ok = true;
    ok &= TestUtils::assertTrue(fs::exists(dstDir / "file1.txt"), "file1.txt should exist in target");
    ok &= TestUtils::assertTrue(fs::exists(dstDir / "subdir/file3.cpp"), "file3.cpp should exist in subdir");
    ok &= TestUtils::assertFalse(fs::exists(dstDir / "file2.tmp"), "file2.tmp should not exist in target");
    ok &= TestUtils::assertFalse(fs::exists(dstDir / "build/temp/excluded.cpp"), "excluded.cpp should not exist");

    cleanupTestEnvironment(testRoot);
    return ok;
}

// Tests if the directory exclusion logic correctly identifies excluded folders
bool FileCopierTest::testExcludeDir() {
    bool ok = true;

    // "build" should be excluded based on rule list
    ok &= TestUtils::assertTrue(FileCopier::isExcludedDir("build", { "build", "out" }), "build should be excluded");

    // "src" should not match exclusion rules
    ok &= TestUtils::assertFalse(FileCopier::isExcludedDir("src", { "build", "out" }), "src should not be excluded");

    return ok;
}

// Tests pattern matching and overwrite behavior for file copying
bool FileCopierTest::testMatchesPattern() {
    const fs::path testRoot = "test_workspace";
    const fs::path srcDir = testRoot / "source";
    const fs::path dstDir = testRoot / "destination";

    setupTestEnvironment(testRoot, srcDir);

    // Set up options to allow only *.txt and *.cpp files
    PruneOptions options;
    options.sources = { srcDir };
    options.destinations = { dstDir };
    options.typePatterns = PatternUtils::wildcardsToRegex({ "*.txt", "*.cpp" });
    options.noOverwrite = true;
    options.quiet = true; // deprecated

    bool ok = true;

    // Check if regex pattern correctly matches a sample filename
    std::regex pattern(R"(.*\.cpp)", std::regex::icase);
    ok &= TestUtils::assertTrue(PatternUtils::matchesPattern("main.cpp", { pattern }), "Pattern matches main.cpp");

    // Prepare a file with existing content in target directory
    {
        fs::create_directories(dstDir);
        std::ofstream file1(dstDir / "file1.txt");
        file1 << "Old content";
        file1.close();
    }

    // Run copy in no-overwrite mode
    {
        std::ofstream logFile("copy_log_no_overwrite.txt");
        FileCopier::copyFiltered(options, &logFile);
        logFile.close();
    }

    // Verify that existing content was not overwritten
    {
        std::ifstream resultFile(dstDir / "file1.txt");
        std::string content;
        std::getline(resultFile, content);
        ok &= TestUtils::assertTrue(content == "Old content", "Content should remain unchanged with noOverwrite");
    }

    // Now test forceOverwrite mode
    options.noOverwrite = false;
    options.forceOverwrite = true;

    {
        std::ofstream logFile2("copy_log_force_overwrite.txt");
        FileCopier::copyFiltered(options, &logFile2);
        logFile2.close();
    }

    // Verify that content has been overwritten
    {
        std::ifstream resultFile2(dstDir / "file1.txt");
        std::string content;
        std::getline(resultFile2, content);
        ok &= TestUtils::assertFalse(content == "Old content", "Content should be replaced with forceOverwrite");
        resultFile2.close();
    }

    cleanupTestEnvironment(testRoot);
    return ok;
}

// Deliberately triggers a failed assertion to test falsified overwrite detection
bool FileCopierTest::testOverwriteFalsify() {
    const fs::path testRoot = "test_workspace";
    const fs::path srcDir = testRoot / "source";
    const fs::path dstDir = testRoot / "destination";

    setupTestEnvironment(testRoot, srcDir);

    // Prepare manipulated file that should not be overwritten
    fs::create_directories(dstDir);
    std::ofstream(dstDir / "file1.txt") << "Manipuliert"; // Intentionally incorrect content

    PruneOptions options;
    options.sources = { srcDir };
    options.destinations = { dstDir };
    options.typePatterns = PatternUtils::wildcardsToRegex({ "*.txt", "*.cpp" });
    options.noOverwrite = true;
    options.quiet = true; // deprecated

    {
        std::ofstream logFile("falsify_log.txt");
        FileCopier::copyFiltered(options, &logFile);
        logFile.close();
    }

    // This test is expected to fail because content was manipulated beforehand
    bool ok = true;
    {
        std::ifstream resultFile(dstDir / "file1.txt");
        std::string content;
        std::getline(resultFile, content);
        resultFile.close();

        // Should have been preserved as "Old content", but was "Manipuliert"
        ok = TestUtils::assertTrue(content == "Old content", "Falsify: Content should remain unchanged with noOverwrite");
    }

    cleanupTestEnvironment(testRoot);
    return ok;
}

// Tests that flatten-suffix does not apply a prefix to root-level files
// but adds a directory-based prefix for files in subdirectories
bool FileCopierTest::testFlattenSuffixNaming() {
    const fs::path testRoot = "test_flatten_suffix";
    const fs::path srcDir = testRoot / "src";
    const fs::path dstDir = testRoot / "out";

    fs::remove_all(testRoot);
    fs::create_directories(srcDir / "core");

    std::ofstream(srcDir / "rootfile.txt") << "root";
    std::ofstream(srcDir / "core" / "corefile.txt") << "core";

    PruneOptions options;
    options.sources = { srcDir };
    options.destinations = { dstDir };
    options.flatten = true;
    options.flattenWithSuffix = true;
    options.typePatterns = PatternUtils::wildcardsToRegex({ "*.txt" });

    FileCopier::copyFiltered(options);

    bool ok = true;
    ok &= TestUtils::assertTrue(fs::exists(dstDir / "rootfile.txt"), "Root file should not have prefix");
    ok &= TestUtils::assertTrue(fs::exists(dstDir / "core_corefile.txt"), "File in subdir should have prefix");

    fs::remove_all(testRoot);
    return ok;
}

// Tests automatic renaming logic in flatten mode
// Ensures that name conflicts are resolved via (1), (2), ... suffix
bool FileCopierTest::testFlattenAutoRename() {
    const fs::path testRoot = "test_flatten_autorename";
    const fs::path srcDir = testRoot / "src";
    const fs::path dstDir = testRoot / "out";

    fs::remove_all(testRoot);
    fs::create_directories(srcDir / "a");
    fs::create_directories(srcDir / "b");

    std::ofstream(srcDir / "a" / "same.txt") << "from A";
    std::ofstream(srcDir / "b" / "same.txt") << "from B";

    PruneOptions options;
    options.sources = { srcDir };
    options.destinations = { dstDir };
    options.flatten = true;
    options.flattenAutoRename = true;
    options.typePatterns = PatternUtils::wildcardsToRegex({ "*.txt" });

    FileCopier::copyFiltered(options);

    bool ok = true;
    ok &= TestUtils::assertTrue(fs::exists(dstDir / "same.txt"), "First file exists");
    ok &= TestUtils::assertTrue(fs::exists(dstDir / "same(1).txt"), "Second file renamed");

    fs::remove_all(testRoot);
    return ok;
}
