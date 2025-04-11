# changelog

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
