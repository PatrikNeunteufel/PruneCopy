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


 // includes für einzelne Testgruppen
#include "BasicFunctionTest.hpp"

#include <iostream>

bool TestRunner::runAllTests() {
	bool allPassed = true;

	std::cout << "[TEST] Running unit tests...\n";

	if (!BasicFunctionTest::run()) allPassed = false;
	std::cout << "[TEST] Running integration tests...\n";
	if (!FileCopierTest::run()) allPassed = false;
	// usw.

	std::cout << (allPassed ? "[TEST] ALL PASSED\n" : "[TEST] FAILURES DETECTED\n");
	return allPassed;
}

