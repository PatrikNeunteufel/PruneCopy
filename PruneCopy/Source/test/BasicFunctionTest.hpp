/*****************************************************************//**
 * @file   BasicFunctionTest.hpp
 * @brief  
 * 
 * @author Patrik Neunteufel
 * @date   April 2025
 *********************************************************************/

#pragma once

#pragma once
#include <string>

class BasicFunctionTest {
public:
    static bool run();  // Führt alle Tests aus und gibt Erfolg zurück

private:
    static bool testConvertUtils();
    static bool testPathUtils();
    static bool testPatternUtils();
    static bool testPruneOptionsParsing();
};
