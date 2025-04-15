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

namespace FileCopierTest {
	bool run(); // Run all tests in this file
	bool CopyFilteredTest(fs::path testRoot, fs::path srcDir, fs::path dstDir, PruneOptions& options); // Test for copyFiltered function
	bool ExcludeDirTest(fs::path testRoot, fs::path srcDir, fs::path dstDir); // Test for isExcludedDir function
	bool matchesPatternTest(fs::path testRoot, fs::path srcDir, fs::path dstDir, PruneOptions& options); // Test for matchesPattern function
}; // namespace FileCopierTest
