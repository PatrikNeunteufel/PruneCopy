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

class FileCopierTest {
public:
    static bool run(); // Führt alle Tests aus

private:
    static bool testCopyFiltered();
    static bool testExcludeDir();
    static bool testMatchesPattern();

    // Hilfsmethoden (optional zur besseren Strukturierung)
    static void setupTestEnvironment(const fs::path& testRoot, const fs::path& srcDir);
    static void cleanupTestEnvironment(const fs::path& testRoot);
    static bool testOverwriteFalsify();
};