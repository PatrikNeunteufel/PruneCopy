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

bool FileCopierTest::run() {
    bool success = true;

    success &= testCopyFiltered();
    success &= testExcludeDir();
    success &= testMatchesPattern();
	// success &= testOverwriteFalsify(); // Uncomment to test overwrite falsification

    if (success)
        std::cout << "[FileCopierTest] All tests passed!" << std::endl;
    else
        std::cout << "[FileCopierTest] Some tests FAILED!" << std::endl;

    return success;
}

// helper methods
void FileCopierTest::setupTestEnvironment(const fs::path& testRoot, const fs::path& srcDir) {
    fs::remove_all(testRoot);
    fs::create_directories(srcDir / "subdir");
    fs::create_directories(srcDir / "build" / "temp");

    std::ofstream(srcDir / "file1.txt") << "Test content A";
    std::ofstream(srcDir / "file2.tmp") << "Excluded file";
    std::ofstream(srcDir / "subdir" / "file3.cpp") << "int main() {}"; //comment out to falsify expected error: [FAIL] file3.cpp should exist in subdir

    std::ofstream(srcDir / "build" / "temp" / "excluded.cpp") << "// should be excluded due to folder";
}

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

// test methods
bool FileCopierTest::testCopyFiltered() {
    const fs::path testRoot = "test_workspace";
    const fs::path srcDir = testRoot / "source";
    const fs::path dstDir = testRoot / "destination";

    setupTestEnvironment(testRoot, srcDir);

    PruneOptions options;
    options.sources = { srcDir };
    options.destinations = { dstDir };
    options.typePatterns = PatternUtils::wildcardsToRegex({ "*.txt", "*.cpp" });
    options.excludeFilePatterns = PatternUtils::wildcardsToRegex({ "*.tmp" });
    options.excludeDirs = { "build" };
    options.quiet = true;

    std::ofstream logFile("copy_log.txt");
    FileCopier::copyFiltered(options, &logFile);
    logFile.close();

    bool ok = true;
    ok &= TestUtils::assertTrue(fs::exists(dstDir / "file1.txt"), "file1.txt should exist in target");
    ok &= TestUtils::assertTrue(fs::exists(dstDir / "subdir/file3.cpp"), "file3.cpp should exist in subdir");
    ok &= TestUtils::assertFalse(fs::exists(dstDir / "file2.tmp"), "file2.tmp should not exist in target");
    ok &= TestUtils::assertFalse(fs::exists(dstDir / "build/temp/excluded.cpp"), "excluded.cpp should not exist");

    cleanupTestEnvironment(testRoot);
    return ok;
}

bool FileCopierTest::testExcludeDir() {
    bool ok = true;

    ok &= TestUtils::assertTrue(FileCopier::isExcludedDir("build", { "build", "out" }), "build should be excluded");
    ok &= TestUtils::assertFalse(FileCopier::isExcludedDir("src", { "build", "out" }), "src should not be excluded");

    return ok;
}

bool FileCopierTest::testMatchesPattern() {
    const fs::path testRoot = "test_workspace";
    const fs::path srcDir = testRoot / "source";
    const fs::path dstDir = testRoot / "destination";

    setupTestEnvironment(testRoot, srcDir);

    PruneOptions options;
    options.sources = { srcDir };
    options.destinations = { dstDir };
    options.typePatterns = PatternUtils::wildcardsToRegex({ "*.txt", "*.cpp" });
    options.noOverwrite = true;
    options.quiet = true;

    bool ok = true;
    std::regex pattern(R"(.*\.cpp)", std::regex::icase);
    ok &= TestUtils::assertTrue(PatternUtils::matchesPattern("main.cpp", { pattern }), "Pattern matches main.cpp");

	// prepair file for noOverwrite test
    {
    fs::create_directories(dstDir);
    std::ofstream file1(dstDir / "file1.txt");
    file1 << "Old content";
	file1.close();
    }
    {
    std::ofstream logFile("copy_log_no_overwrite.txt");
    FileCopier::copyFiltered(options, &logFile);
    logFile.close();
    }
    {
    std::ifstream resultFile(dstDir / "file1.txt");
    std::string content;
    std::getline(resultFile, content);
    ok &= TestUtils::assertTrue(content == "Old content", "Content should remain unchanged with noOverwrite");
    }

    // test forceOverwrite
    options.noOverwrite = false;
    options.forceOverwrite = true;
    {
    std::ofstream logFile2("copy_log_force_overwrite.txt");
    FileCopier::copyFiltered(options, &logFile2);
    logFile2.close();
    }
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

bool FileCopierTest::testOverwriteFalsify() {
    const fs::path testRoot = "test_workspace";
    const fs::path srcDir = testRoot / "source";
    const fs::path dstDir = testRoot / "destination";

    setupTestEnvironment(testRoot, srcDir);

    // absichtlich falscher Inhalt vor dem Kopieren
    fs::create_directories(dstDir);
    std::ofstream(dstDir / "file1.txt") << "Manipuliert"; // nicht "Old content"

    PruneOptions options;
    options.sources = { srcDir };
    options.destinations = { dstDir };
    options.typePatterns = PatternUtils::wildcardsToRegex({ "*.txt", "*.cpp" });
    options.noOverwrite = true;
    options.quiet = true;

    {
        std::ofstream logFile("falsify_log.txt");
        FileCopier::copyFiltered(options, &logFile);
		logFile.close();
    }
	bool ok = true;
    // Test erwartet, dass der Inhalt NICHT ersetzt wurde (was er aber wurde!)
    {
        std::ifstream resultFile(dstDir / "file1.txt");
        std::string content;
        std::getline(resultFile, content);
        resultFile.close();

        // Erwartet "Old content", aber ist "Manipuliert" – sollte FAIL geben
        ok = TestUtils::assertTrue(content == "Old content", "Falsify: Content should remain unchanged with noOverwrite");
    }


    cleanupTestEnvironment(testRoot);
    return ok;
}
