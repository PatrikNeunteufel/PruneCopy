/*****************************************************************//**
 * @file   Console.hpp
 * @brief  Interface for console output and user interaction.
 * 
 * @author Patrik Neunteufel
 * @date   April 2025
 *********************************************************************/

#pragma once
#include <string>
#include <vector>
#include <iomanip>

#include "cli/ArgumentParser.hpp"

 // Represents the type of message to be displayed in the console
enum class MessageType {
	Info,       // General informational message (neutral)
	Warning,    // Indicates a non-critical issue or caution
	Error,      // Critical problem that likely stops execution
	Success,    // Operation completed successfully
	Conflict,   // Indicates a file or configuration conflict
	Skip,       // Operation skipped (e.g. due to filters or settings)
	Overwrite,  // File was overwritten
	Deleted     // File or directory was deleted
};

// Represents a project supporter (e.g. donor or contributor)
struct Supporter {
	std::string name;   // Display name of the supporter
	std::string url;    // Optional URL (e.g. profile or website)
	int weight = 1;     // Weight for random selection (higher = more likely to appear)
};
class Console {
public:

	/**
	 * @brief Prints command-line flags help to the console.
	 *
	 * @param flags The list of command-line flags to print.
	 * @param commandSize The size of the command for formatting.
	 */
	static void printFlagsHelp(const std::vector<Flag>& flags, int commandSize);

	/**
	 * @brief Prints usage/help message to the console.
	 */
	static void printHelp();

	/**
	 * @brief Prints usage/help message to the console.
	 *
	 * @param messageType The type of message to print (Info, Warning, Error, etc.).
	 * @param message The message to print.
	 */
	static void printUsage();

	/**
	 * @brief Prints the update information to the console.
	 *
	 * @param version The version of the update.
	 * @param changelog The changelog for the update.
	 */
	static void printUpdate();

	/**
	 * @brief Prints version information to the console.
	 */
	static void printVersion();

	/**
	 * @brief Prints about information to the console.
	 */
	static void printAbout();

	/**
	 * @brief Prints contact information to the console.
	 */
	static void contactDev();

	/**
	 * @brief Prints donation information to the console.
	 */
	static void printDonate();

	/**
	 * @brief Prints a message to the console with the specified message type.
	 *
	 * @param messageType The type of message to print (Info, Warning, Error, etc.).
	 * @param message The message to print.
	 */
	static void printMessage(MessageType messageType, const std::string& message);

	/**
	 * @brief Prints a message to the console with the specified message type and color.
	 *
	 * @param messageType The type of message to print (Info, Warning, Error, etc.).
	 * @param message The message to print.
	 * @param color The color to use for the message.
	 */
	static std::string inputRequest(const std::string& message, const std::string& defaultValue = "");

	/**
	 * @brief Prints a random supporter message to the console.
	 *
	 * @param allowNetwork If true, allows network-related messages.
	 */
	static void printRandomSupporter(bool allowNetwork);

	/**
	 * @brief Prints a message to the console with the specified message type and color.
	 *
	 * @param messageType The type of message to print (Info, Warning, Error, etc.).
	 * @param message The message to print.
	 * @param color The color to use for the message.
	 */
	static void printAllSupporters(bool allowNetwork = true);
};
