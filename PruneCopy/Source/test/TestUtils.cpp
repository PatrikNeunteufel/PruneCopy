/*****************************************************************//**
 * @file   TestUtils.cpp
 * @brief  
 * 
 * @author Patrik Neunteufel
 * @date   April 2025
 *********************************************************************/

#include "TestUtils.hpp"

#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif


void setConsoleColor(int colorCode) {
#ifdef _WIN32
    static HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, colorCode);
#else
    std::cout << "\033[" << colorCode << "m";
#endif
}

void resetConsoleColor() {
#ifdef _WIN32
    setConsoleColor(7);
#else
    std::cout << "\033[0m";
#endif
}


void TestUtils::printPass(const std::string& message) {
    setConsoleColor(10); // Green
    std::cout << "[PASS] ";
    resetConsoleColor();
    std::cout << message << std::endl;
}

void TestUtils::printFail(const std::string& message) {
    setConsoleColor(12); // Red
    std::cout << "[FAIL] ";
    resetConsoleColor();
    std::cout << message << std::endl;
}

void TestUtils::printInfo(const std::string& message) {
    setConsoleColor(9); // Blue
    std::cout << message << std::endl;
    resetConsoleColor();
}

void TestUtils::printSection(const std::string& title) {
    setConsoleColor(9); // Blue
    std::cout << "[" << title << "]" << std::endl;
    resetConsoleColor();
}


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

bool TestUtils::assertContains(const std::string& haystack, const std::string& needle, const std::string& testName) {
    if (haystack.find(needle) != std::string::npos) {
        std::cout << "[PASS] " << testName << "\n";
        return true;
    } else {
        std::cerr << "[FAIL] " << testName << " – expected to find substring: '" << needle << "'\n";
        return false;
    }
}
