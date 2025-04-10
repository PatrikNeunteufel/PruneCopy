/*****************************************************************//**
 * @file   TestUtils.cpp
 * @brief  
 * 
 * @author Patrik Neunteufel
 * @date   April 2025
 *********************************************************************/

#include "TestUtils.hpp"

#include <iostream>

bool assertTrue(bool condition, const std::string& testName) {
	if (!condition) {
		std::cerr << "[FAIL] " << testName << "\n";
		return false;
	}
	std::cout << "[PASS] " << testName << "\n";
	return true;
}

bool assertFalse(bool condition, const std::string& testName) {
	return assertTrue(!condition, testName);
}
