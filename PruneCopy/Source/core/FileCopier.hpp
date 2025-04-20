/*****************************************************************//**
 * @file   FileCopier.hpp
 * @brief  Provides file filtering and copying logic for PruneCopy
 * 
 * @author Patrik Neunteufel
 * @date   April 2025
 *********************************************************************/

#pragma once
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>

#include "core/PruneOptions.hpp"

/**
 * @brief Class responsible for copying files based on specified options and filters.
 */
class FileCopier {
public:
	/**
	 * @brief Constructs a FileCopier with given options and optional log file output.
	 *
	 * @param options The configuration options for file copying.
	 * @param logFile Optional pointer to an ofstream for logging output.
	 */
    explicit FileCopier(const PruneOptions& options, std::ofstream* logFile = nullptr);

	/**
	 * @brief Executes the file copying operation based on configured source, destination, and filters.
	 */
    void execute();


    /**
	 * @brief Static helper to execute a filtered copy operation using a FileCopier instance.
	 * Legacy compatibility (for current testing and CLI integration)
	 * 
	 * @param options The configuration options for file copying.
	 * @param logFile Optional pointer to an ofstream for logging output.
     */
    static void copyFiltered(const PruneOptions& options, std::ofstream* logFile = nullptr);

	/**
	* @brief Checks if a directory is excluded based on the provided exclusion patterns.
	* 
	* @param dir The directory path to check.
	* @param excludeDirs The list of exclusion patterns.
	* @returns True if the directory is excluded, false otherwise.
	*/
    static bool isExcludedDir(const std::filesystem::path& dir, const std::vector<std::string>& excludeDirs);

protected:
	/** 
	 * @brief .
	 * 
	 * @param srcRoot root path of the source directory
	 * @param currentFile the current file being processed
	 * @param destRoot root path of the destination directory
	 * @return resolved target path for the file
	 */
    std::filesystem::path resolveTargetPath(const std::filesystem::path& srcRoot,
        const std::filesystem::path& currentFile,
        const std::filesystem::path& destRoot) const;

	/** 
	 * @brief .
	 * 
	 * @param targetFile the target file path
	 * @return true, if the user wants to overwrite the file
	 */
    bool handleOverwritePrompt(const std::filesystem::path& targetFile);
    
	/** 
	 * @brief .
	 * 
	 * @param path log the path of the copied file
	 */
	void logCopy(const std::filesystem::path& path);

protected:
	const PruneOptions& m_options;		///< The configuration options for file copying. >
	std::ofstream* m_logFile;			///< Optional pointer to an ofstream for logging output. >
};
