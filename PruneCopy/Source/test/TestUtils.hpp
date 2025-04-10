/*****************************************************************//**
 * @file   TestUtils.hpp
 * @brief  
 * 
 * @author Patrik Neunteufel
 * @date   April 2025
 *********************************************************************/

#pragma once
#include <string>
#include <iostream>

bool assertTrue(bool condition, const std::string& testName);

bool assertFalse(bool condition, const std::string& testName);

// Template für gleichwertige Vergleiche mit Ausgabe
template<typename T>
bool assertEqual(const T& expected, const T& actual, const std::string& testName) {
	if (expected != actual) {
		std::cerr << "[FAIL] " << testName << ": expected [" << expected << "] but got [" << actual << "]\n";
		return false;
	}
	std::cout << "[PASS] " << testName << "\n";
	return true;
}