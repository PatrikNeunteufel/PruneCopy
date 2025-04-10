/*****************************************************************//**
 * @file   BasicFunctionTest.hpp
 * @brief  
 * 
 * @author Patrik Neunteufel
 * @date   April 2025
 *********************************************************************/

#pragma once


namespace Unittest {
	bool matchesPatternTest();   // matchesPattern
	bool globToRegexTest();      // globToRegex

	bool ExcludeDirTest(); // z. B. isExcludedDir
}

namespace Integrationtest {
	bool PatternMatchingTest();  // wildcardsToRegex + matchesPattern
}

namespace BasicFunctionTest {
	bool run(); // calls all basic function tests
}
