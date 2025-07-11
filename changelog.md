# changelog

## V 1.0.4 - 2025-04-21
- added `--flatten` to flatten the directory structure in the destination
- added `--flatten-suffix` to add a suffix to the flattened files
- - added `--flatten-auto-rename` to automatically rename files if they already exist in the destination

## V 1.0.3 - 2025-04-21
- change structure for fileCopier to prepare reusability for the other copying methods
- moved isExcludedDir() methode from fileCopier to PatternUtils
- extended Testing for ArgumentParser and FileCopier
- added more TestUtils functions
- added deprecated for flags and handling in ArgumentParser
- unified LogManager to better reuse the code
- added more log levels, they now work hirarchically All > Standard > Info > Warning > Error > None
- added LogLevel to PruneOptions to centralize the log level for the whole program
- cleanup the comments and added doxygen comments
- fixed log to show excluded files and directories only once per source not per destination
- info for included files and also excluded directories and files are now shown in the log
- added presets
- 

## V 1.0.2 - 2025-04-11
- changed log color only for message tag
- moved checking for Info Output to bool ArgumentParser:checkInfo returns true if info printed -> end of program
- implemented multisource and multidestination ability
- added basic testing for development
- added update checker for the program

## V 1.0.1 - 2025-04-07
- added flag structure (including name, shortName, description, type, valueRepresentation, FlagType and FlagValueType)
- refactored printHelp to use the new flag structure
- added vector for infoFlags and optionFlags
- added better check for cli arguments using the new flag structure and the vectors
