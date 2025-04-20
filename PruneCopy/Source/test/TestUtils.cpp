/*****************************************************************//**
 * @file   TestUtils.cpp
 * @brief  
 * 
 * @author Patrik Neunteufel
 * @date   April 2025
 *********************************************************************/

#include "TestUtils.hpp"

#include <iostream>

 // Asserts that a condition is true; logs result to stdout or stderr
bool TestUtils::assertTrue(bool condition, const std::string& testName) {
	if (!condition) {
		std::cerr << "[FAIL] " << testName << "\n";
		return false;
	}
	std::cout << "[PASS] " << testName << "\n";
	return true;
}

// Asserts that a condition is false by negating it and delegating to assertTrue()
bool TestUtils::assertFalse(bool condition, const std::string& testName) {
	return assertTrue(!condition, testName);
}
