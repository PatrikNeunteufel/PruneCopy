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

#include <regex>

 // BasicFunctionTest
	bool BasicFunctionTest::run() {
		bool success = true;
		if (!Unittest::matchesPatternTest()) success = false;
		if (!Unittest::globToRegexTest()) success = false;
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

	bool Unittest::globToRegexTest()
	{
		bool ok = true;

		ok &= assertEqual(PatternUtils::globToRegex("*.cpp"), std::string(R"(.*\.cpp)"), "globToRegex: '*.cpp'");
		ok &= assertEqual(PatternUtils::globToRegex("data_??.txt"), std::string(R"(data_..\.txt)"), "globToRegex: 'data_??.txt'");
		ok &= assertEqual(PatternUtils::globToRegex("a*b?.h"), std::string(R"(a.*b.\.h)"), "globToRegex: 'a*b?.h'");

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

		ok &= assertTrue(PatternUtils::matchesPattern("test.h", PatternUtils::wildcardsToRegex({ R"(.*\.h)" })), "*.h matches test.h");
		ok &= assertFalse(PatternUtils::matchesPattern("main.cpp", PatternUtils::wildcardsToRegex({ R"(.*\.h)" })), "*.h does not match main.cpp");

		return ok;
	}


