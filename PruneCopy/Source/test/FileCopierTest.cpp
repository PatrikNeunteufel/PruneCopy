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



bool FileCopierTest::run()
{
	const fs::path testRoot = "test_workspace";
	const fs::path srcDir = testRoot / "source";
	const fs::path dstDir = testRoot / "destination";

	PruneOptions options;
	options.sources = { srcDir };
	options.destinations = { dstDir };
	options.typePatterns = PatternUtils::wildcardsToRegex({ "*.txt", "*.cpp" });
	options.excludeFilePatterns = PatternUtils::wildcardsToRegex({ "*.tmp" });
	options.excludeDirs = { "build" }; // Test für Ordner-Exclusion
	options.quiet = true;

	// Test: noOverwrite aktiv – Datei darf nicht überschrieben werden
	options.noOverwrite = true;
	options.forceOverwrite = false;

	bool success = true;
	if (!CopyFilteredTest(testRoot, srcDir, dstDir, options)) success = false;
	if (!matchesPatternTest(testRoot, srcDir, dstDir, options)) success = false;
	if (!ExcludeDirTest(testRoot, srcDir, dstDir)) success = false;

	return success;
}

bool FileCopierTest::CopyFilteredTest(fs::path testRoot, fs::path srcDir, fs::path dstDir, PruneOptions& options) {
	bool ok = true;

	fs::remove_all(testRoot);
	fs::create_directories(srcDir / "subdir");
	fs::create_directories(srcDir / "build" / "temp");

	// Dateien erstellen
	std::ofstream(srcDir / "file1.txt") << "Test content A";
	std::ofstream(srcDir / "file2.tmp") << "Excluded file";
	std::ofstream(srcDir / "subdir" / "file3.cpp") << "int main() {}";
	std::ofstream(srcDir / "build" / "temp" / "excluded.cpp") << "// should be excluded due to folder";


	std::ofstream logFile("copy_log.txt");
	FileCopier::copyFiltered(options, &logFile);
	logFile.close();

	// Prüfung: enthaltene Dateien
	ok &= assertTrue(fs::exists(dstDir / "file1.txt"), "file1.txt should exist in target");
	ok &= assertTrue(fs::exists(dstDir / "subdir" / "file3.cpp"), "file3.cpp should exist in subdir");

	// Prüfung: ausgeschlossene Dateien
	ok &= assertFalse(fs::exists(dstDir / "file2.tmp"), "file2.tmp should not exist in target (excluded by pattern)");
	ok &= assertFalse(fs::exists(dstDir / "build" / "temp" / "excluded.cpp"), "excluded.cpp should not exist (excluded by dir)");

	// Log prüfen
	std::ifstream inLog("copy_log.txt");
	std::stringstream buffer;
	buffer << inLog.rdbuf();
	std::string logContent = buffer.str();

	ok &= assertTrue(logContent.find("file1.txt") != std::string::npos, "Log should contain file1.txt");
	ok &= assertTrue(logContent.find("file3.cpp") != std::string::npos, "Log should contain file3.cpp");
	ok &= assertFalse(logContent.find("file2.tmp") != std::string::npos, "Log should not contain file2.tmp");
	ok &= assertFalse(logContent.find("excluded.cpp") != std::string::npos, "Log should not contain excluded.cpp");

	return ok;
}

bool FileCopierTest::ExcludeDirTest(fs::path testRoot, fs::path srcDir, fs::path dstDir) {
	bool ok = true;

	ok &= assertTrue(FileCopier::isExcludedDir("build", { "build", "out" }), "build should be excluded");
	ok &= assertFalse(FileCopier::isExcludedDir("src", { "build", "out" }), "src should not be excluded");

	return ok;
}

bool FileCopierTest::matchesPatternTest(fs::path testRoot, fs::path srcDir, fs::path dstDir, PruneOptions& options) {
	bool ok = true;
	std::regex pattern(R"(.*\.cpp)", std::regex::icase);
	ok &= assertTrue(PatternUtils::matchesPattern("main.cpp", { pattern }), "Pattern matches main.cpp");

	// Vorbereitung: Datei überschreiben (damit man sieht ob sie ersetzt wird)
	std::ofstream(dstDir / "file1.txt") << "Old content that should be preserved";


	std::ofstream logFile2("copy_log_no_overwrite.txt");
	FileCopier::copyFiltered(options, &logFile2);
	logFile2.close();

	std::ifstream resultFile(dstDir / "file1.txt");
	std::string content;
	std::getline(resultFile, content);
	ok &= assertTrue(content.find("Old content") != std::string::npos, "Content should remain unchanged with noOverwrite");

	// Test: forceOverwrite aktiv – Datei muss überschrieben werden
	options.noOverwrite = false;
	options.forceOverwrite = true;

	std::ofstream logFile3("copy_log_force_overwrite.txt");
	FileCopier::copyFiltered(options, &logFile3);
	logFile3.close();

	std::ifstream resultFile2(dstDir / "file1.txt");
	std::getline(resultFile2, content);
	ok &= assertFalse(content.find("Old content") != std::string::npos, "Content should be replaced with forceOverwrite");

	return ok;
}

