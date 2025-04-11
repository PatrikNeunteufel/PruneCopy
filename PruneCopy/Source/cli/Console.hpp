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

enum class MessageType {
	Info,
	Warning,
	Error,
	Success,
	Conflict,
	Skip,
	Overwrite,
	Deleted
};

namespace Console {

	/**
	 * @brief Represents a supporter with a name, URL, and weight.
	 */
	struct Supporter {
		std::string name;
		std::string url;
		int weight = 1;
	};

	/**
	 * @brief Prints command-line flags help to the console.
	 *
	 * @param flags The list of command-line flags to print.
	 * @param commandSize The size of the command for formatting.
	 */
	void printFlagsHelp(const std::vector<Flag>& flags, int commandSize);
	/**
	 * @brief Prints usage/help message to the console. 
	 */
	void printHelp();
	/**
	 * @brief Prints usage/help message to the console.
	 *
	 * @param messageType The type of message to print (Info, Warning, Error, etc.).
	 * @param message The message to print.
	 */
	void printUsage();
	void printUpdate();
	/**
	 * @brief Prints version information to the console.
	 */
	void printVersion();
	/**
	 * @brief Prints about information to the console.
	 */
	void printAbout();	
	/**
	 * @brief Prints contact information to the console.
	 */
	void contactDev();

	void printDonate();

	/**
	 * @brief Prints a message to the console with the specified message type.
	 *
	 * @param messageType The type of message to print (Info, Warning, Error, etc.).
	 * @param message The message to print.
	 */
	void printMessage(MessageType messageType, const std::string& message);

	/**
	 * @brief Prints a message to the console with the specified message type and color.
	 *
	 * @param messageType The type of message to print (Info, Warning, Error, etc.).
	 * @param message The message to print.
	 * @param color The color to use for the message.
	 */
	std::string inputRequest(const std::string& message, const std::string& defaultValue = "");

	/**
	 * @brief Prints a random supporter message to the console.
	 *
	 * @param allowNetwork If true, allows network-related messages.
	 */
	void printRandomSupporter(bool allowNetwork);
	/**
	 * @brief Prints a message to the console with the specified message type and color.
	 *
	 * @param messageType The type of message to print (Info, Warning, Error, etc.).
	 * @param message The message to print.
	 * @param color The color to use for the message.
	 */
	void printAllSupporters(bool allowNetwork = true);
}//	namespace Console
