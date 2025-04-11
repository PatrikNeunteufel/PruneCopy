/*****************************************************************//**
 * @file   BasicFunctionTest.cpp
 * @brief  
 * 
 * @author Patrik Neunteufel
 * @date   April 2025
 *********************************************************************/

#include "BasicFunctionTest.hpp"
#include "TestUtils.hpp"

#include "util/PatternUtils.hpp"
#include "core/FileCopier.hpp"

#include <iostream>
#include <regex>

 // BasicFunctionTest
	bool BasicFunctionTest::run() {
		bool success = true;
		if (!Unittest::matchesPatternTest()) success = false;
		if (!Unittest::ExcludeDirTest()) success = false;
		if (!Integrationtest::PatternMatchingTest()) success = false;

		return success;
	}


	// Unit Tests
	bool Unittest::matchesPatternTest()
	{
		bool ok = true;
		std::regex pattern(R"(.*\.cpp)", std::regex::icase);
		ok &= assertTrue(PatternUtils::matchesPattern("main.cpp", { pattern }), "Pattern matches main.cpp");

		return ok;
	}


	bool Unittest::ExcludeDirTest() {
		bool ok = true;

		ok &= assertTrue(FileCopier::isExcludedDir("build", { "build", "out" }), "build should be excluded");
		ok &= assertFalse(FileCopier::isExcludedDir("src", { "build", "out" }), "src should not be excluded");

		return ok;
	}

	// Integrationtest
	bool Integrationtest::PatternMatchingTest() {
		bool ok = true;
		//ok &= assertTrue(true, "assertTrue true");
		//ok &= assertTrue(false, "assertTrue fale");
		//ok &= assertFalse(true, "assertFalse true");
		//ok &= assertFalse(false, "assertFalse fale");
		//std::cout << (PatternUtils::wildcardsToRegex({ R"(.*\.h)" })[0]) << std::endl;

		//ok &= assertEqual(PatternUtils::wildcardsToRegex({ "*.h" })[0], "^.*\\.h$", "Converted *.h to regex");
		//std::vector<std::regex> patterns = PatternUtils::wildcardsToRegex({ "*.h" });
		//ok &= assertEqual(patterns[0], std::regex("*.h"), "Converted *.h to regex");

		ok &= assertTrue(PatternUtils::matchesPattern("test.hpp", PatternUtils::wildcardsToRegex({ "*.hpp" })), " * .hpp matches test.hpp");

		ok &= assertTrue(PatternUtils::matchesPattern("test.h", PatternUtils::wildcardsToRegex({ "*.h" })), "*.h matches test.h");
		ok &= assertTrue(PatternUtils::matchesPattern("main.c", PatternUtils::wildcardsToRegex({ "*.c" })), "*.c matches main.c");
		ok &= assertFalse(PatternUtils::matchesPattern("test.hpp", PatternUtils::wildcardsToRegex({ "*.h" })), "*.h matches test.hpp");
		ok &= assertFalse(PatternUtils::matchesPattern("main.cpp", PatternUtils::wildcardsToRegex({ "*.h" })), "*.h does not match main.cpp");

		return ok;
	}


