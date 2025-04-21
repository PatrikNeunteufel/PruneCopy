/*****************************************************************//**
 * @file   ArgumentParseTest.cpp
 * @brief  Tests ArgumentParser behavior with simulated CLI input
 *
 * @author Patrik Neunteufel
 * @date   April 2025
 *********************************************************************/

#include "ArgumentParseTest.hpp"
#include "test/TestUtils.hpp"

#include "cli/ArgumentParser.hpp"
#include "core/PruneOptions.hpp"

#include <iostream>
#include <sstream>

 // Entry point for running all ArgumentParser-related unit tests
bool ArgumentParseTest::run() {
    std::cout << "[ArgumentParseTest] Running tests..." << std::endl;
    bool success = true;

    // Run each individual test and accumulate success
    success &= testLegacyMode();
    success &= testFullCLIMode();
    success &= testColorMode();
    success &= testDeprecatedDetection();
    success &= testDeprecatedClear();

    // Report overall result
    if (success)
        std::cout << "[ArgumentParseTest] All tests passed!" << std::endl;
    else
        std::cout << "[ArgumentParseTest] Some tests FAILED!" << std::endl;

    return success;
}

// Tests legacy mode where source and destination are passed as positional arguments
bool ArgumentParseTest::testLegacyMode() {
    const char* argv[] = { "prunecopy", "source", "target" };
    int argc = sizeof(argv) / sizeof(argv[0]);
    PruneOptions opts;
	ParsedCliControl controlFlags;
    // Parse legacy-style CLI input
    ArgumentParser::parse(argc, const_cast<char**>(argv), opts, controlFlags);

    bool success = true;

    // Expect exactly one source and one destination to be parsed
    success &= TestUtils::assertEqual(1u, opts.sources.size(), "LegacyMode: 1 source");
    success &= TestUtils::assertEqual(1u, opts.destinations.size(), "LegacyMode: 1 destination");

    return success;
}


// Tests full CLI mode where all arguments are passed via named options
bool ArgumentParseTest::testFullCLIMode() {
    const char* argv[] = {
        "prunecopy",
        "--source", "src1",
        "--destination", "dst1",
        "--dry-run",
        "--force-overwrite",
        "--log-dir", "logs",
        "--log-level", "error"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);
    PruneOptions opts;

    ParsedCliControl controlFlags;
    // Parse CLI-style argument list
    ArgumentParser::parse(argc, const_cast<char**>(argv), opts, controlFlags);

    bool success = true;

    // Validate all parsed values against expected configuration
    success &= TestUtils::assertEqual(1u, opts.sources.size(), "CLI: 1 source");
    success &= TestUtils::assertEqual(1u, opts.destinations.size(), "CLI: 1 destination");
    success &= TestUtils::assertTrue(opts.dryRun, "CLI: dry-run");
    success &= TestUtils::assertTrue(opts.forceOverwrite, "CLI: force-overwrite");
    success &= TestUtils::assertTrue(opts.enableLogging, "CLI: enable logging");
    success &= TestUtils::assertEqual(std::string("logs"), opts.logDir.filename().string(), "CLI: log-dir name");
    success &= TestUtils::assertEqual(LogLevel::Error, opts.logLevel, "CLI: log-level");

    return success;
}

// Tests parsing of the --color flag with "always" value
bool ArgumentParseTest::testColorMode() {
    const char* argv[] = {
        "prunecopy",
        "--source", "src",
        "--destination", "dst",
        "--color", "always"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);
    PruneOptions opts;
    ParsedCliControl controlFlags;

    // Parse input and check whether color mode is set to Always
    ArgumentParser::parse(argc, const_cast<char**>(argv), opts, controlFlags);
    return TestUtils::assertEqual(ColorMode::Always, opts.colorMode, "ColorMode: always");
}


// Tests whether deprecated flags are correctly detected and warnings are emitted
bool ArgumentParseTest::testDeprecatedDetection() {
    const char* argv[] = {
        "prunecopy",
        "../source",
        "../target",
        "--cmdln-out-off"  // deprecated flag
    };
    int argc = sizeof(argv) / sizeof(argv[0]);
    PruneOptions opts;
    ParsedCliControl controlFlags;

    ArgumentParser::clearDeprecatedFlagLog(); // Clear any previous warnings
    ArgumentParser::parse(argc, const_cast<char**>(argv), opts, controlFlags);

    // Redirect std::cout to capture output from deprecated warning emitter
    std::ostringstream out;
    std::streambuf* oldCout = std::cout.rdbuf(out.rdbuf());
    ArgumentParser::emitDeprecatedWarnings();
    std::cout.rdbuf(oldCout);

    std::string result = out.str();
    bool success = true;

    // Check that warning message and deprecated flag name are present in the output
    success &= TestUtils::assertTrue(result.find("deprecated") != std::string::npos, "Deprecated: warning present");
    success &= TestUtils::assertTrue(result.find("--cmdln-out-off") != std::string::npos, "Deprecated: flag mentioned");

    return success;
}

// Tests that calling clearDeprecatedFlagLog() prevents warnings from being emitted
bool ArgumentParseTest::testDeprecatedClear() {
    ArgumentParser::clearDeprecatedFlagLog(); // Clear all tracked deprecated flags

    // Capture output to ensure no warnings are printed
    std::ostringstream out;
    std::streambuf* oldCout = std::cout.rdbuf(out.rdbuf());
    ArgumentParser::emitDeprecatedWarnings();
    std::cout.rdbuf(oldCout);

    // Output should be completely empty after clearing
    return TestUtils::assertEqual(std::string(""), out.str(), "Deprecated: no output after clear");
}

