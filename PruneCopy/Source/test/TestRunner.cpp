/*****************************************************************//**
 * @file   TestRunner.cpp
 * @brief  
 * 
 * @author Patrik Neunteufel
 * @date   April 2025
 *********************************************************************/

#include "TestRunner.hpp"
#include "TestUtils.hpp"
#include "FileCopierTest.hpp"
#include "ArgumentParseTest.hpp"
#include "PresetLoaderTest.hpp"


 // includes für einzelne Testgruppen
#include "BasicFunctionTest.hpp"

#include <iostream>

// Executes all registered test suites and prints summary to the console
bool TestRunner::runAllTests() {
	bool allPassed = true;

	// Run argument parsing tests
	std::cout << "[TEST] Running argument tests...\n";
	if (!ArgumentParseTest::run()) allPassed = false;

	// Run basic functionality tests
	std::cout << "[TEST] Running basic tests...\n";
	if (!BasicFunctionTest::run()) allPassed = false;

	// Run integration tests involving file operations
	std::cout << "[TEST] Running integration tests...\n";
	if (!FileCopierTest::run()) allPassed = false;

	// Run PresetLoader tests
	std::cout << "[TEST] Running Preset Loader tests...\n";
	if (!PresetLoaderTest::run()) allPassed = false;

	// Print overall test result
	std::cout << (allPassed ? "[TEST] ALL PASSED\n" : "[TEST] FAILURES DETECTED\n");

	return allPassed;
}
