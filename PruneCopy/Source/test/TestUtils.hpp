#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <type_traits>
#include <filesystem>

namespace TestUtils {

    /**
     * @brief Converts any type that can be streamed to std::ostream into a std::string.
     *
     * @tparam T Any type supporting operator<<
     * @param value The value to be converted
     * @return String representation of the value
     */
    template<typename T>
    auto toString(const T& value) -> decltype(std::declval<std::ostream&>() << value, std::string()) {
        std::ostringstream oss;
        oss << value;
        return oss.str();
    }

    /**
     * @brief Converts a std::string to itself.
     *
     * @param value The input string
     * @return Same string (identity function)
     */
    inline std::string toString(const std::string& value) {
        return value;
    }

    /**
     * @brief Converts a C-style string to a std::string.
     *
     * @param value The input C-style string
     * @return A std::string copy of the input
     */
    inline std::string toString(const char* value) {
        return std::string(value);
    }

    /**
     * @brief Converts an enum to its underlying integer string representation.
     *
     * @tparam T An enum type
     * @param value Enum value to convert
     * @return String representation of the integer value
     */
    template<typename T>
    typename std::enable_if<std::is_enum<T>::value, std::string>::type
        toString(const T& value) {
        return std::to_string(static_cast<int>(value));
    }

    /**
     * @brief Converts a filesystem path to its string representation.
     *
     * @param path A std::filesystem::path object
     * @return String representation of the path
     */
    inline std::string toString(const std::filesystem::path& path) {
        return path.string();
    }

    /**
     * @brief Asserts that two values are equal by comparing their stringified representations.
     *
     * @tparam T Type of expected value
     * @tparam U Type of actual value
     * @param expected The expected value
     * @param actual The actual value
     * @param message A message describing the test
     * @return True if values are equal, false otherwise
     */
    template<typename T, typename U>
    bool assertEqual(const T& expected, const U& actual, const std::string& message = "") {
        const std::string expectedStr = toString(expected);
        const std::string actualStr = toString(actual);

        if (expectedStr != actualStr) {
            std::cout << "[FAIL] " << message
                << ": expected '" << expectedStr
                << "', got '" << actualStr << "'" << std::endl;
            return false;
        }
        else {
            std::cout << "[PASS] " << message << std::endl;
            return true;
        }
    }

    /**
     * @brief Asserts that a given condition is true.
     *
     * @param condition Boolean expression to evaluate
     * @param message A description for the test case
     * @return True if the condition is true, false otherwise
     */
    bool assertTrue(bool condition, const std::string& message = "");

    /**
     * @brief Asserts that a given condition is false.
     *
     * @param condition Boolean expression to evaluate
     * @param message A description for the test case
     * @return True if the condition is false, false otherwise
     */
    bool assertFalse(bool condition, const std::string& message = "");

} // namespace TestUtils
