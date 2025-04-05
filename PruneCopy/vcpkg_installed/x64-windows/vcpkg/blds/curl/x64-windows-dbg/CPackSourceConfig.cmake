# This file will be configured to contain variables for CPack. These variables
# should be set in the CMake list file of the project before CPack module is
# included. The list of available CPACK_xxx variables and their associated
# documentation may be obtained using
#  cpack --help-variable-list
#
# Some variables are common to all generators (e.g. CPACK_PACKAGE_NAME)
# and some are specific to a generator
# (e.g. CPACK_NSIS_EXTRA_INSTALL_COMMANDS). The generator specific variables
# usually begin with CPACK_<GENNAME>_xxxx.


set(CPACK_BUILD_SOURCE_DIRS "F:/Repos_2025/PruneCopy/PruneCopy/vcpkg_installed/x64-windows/vcpkg/blds/curl/src/url-8_11_0-2be05894fb.clean;F:/Repos_2025/PruneCopy/PruneCopy/vcpkg_installed/x64-windows/vcpkg/blds/curl/x64-windows-dbg")
set(CPACK_CMAKE_GENERATOR "Ninja")
set(CPACK_COMPONENT_UNSPECIFIED_HIDDEN "TRUE")
set(CPACK_COMPONENT_UNSPECIFIED_REQUIRED "TRUE")
set(CPACK_DEFAULT_PACKAGE_DESCRIPTION_FILE "C:/Users/patri/AppData/Local/vcpkg/downloads/tools/cmake-3.30.1-windows/cmake-3.30.1-windows-i386/share/cmake-3.30/Templates/CPack.GenericDescription.txt")
set(CPACK_DEFAULT_PACKAGE_DESCRIPTION_SUMMARY "CURL built using CMake")
set(CPACK_GENERATOR "7Z;ZIP")
set(CPACK_IGNORE_FILES "/CVS/;/\\.svn/;/\\.bzr/;/\\.hg/;/\\.git/;\\.swp\$;\\.#;/#")
set(CPACK_INNOSETUP_ARCHITECTURE "x64")
set(CPACK_INSTALLED_DIRECTORIES "F:/Repos_2025/PruneCopy/PruneCopy/vcpkg_installed/x64-windows/vcpkg/blds/curl/src/url-8_11_0-2be05894fb.clean;/")
set(CPACK_INSTALL_CMAKE_PROJECTS "")
set(CPACK_INSTALL_PREFIX "F:/Repos_2025/PruneCopy/PruneCopy/vcpkg_installed/x64-windows/vcpkg/pkgs/curl_x64-windows/debug")
set(CPACK_MODULE_PATH "F:/Repos_2025/PruneCopy/PruneCopy/vcpkg_installed/x64-windows/vcpkg/blds/curl/src/url-8_11_0-2be05894fb.clean/CMake;")
set(CPACK_NSIS_DISPLAY_NAME "CURL 8.11.0-DEV")
set(CPACK_NSIS_INSTALLER_ICON_CODE "")
set(CPACK_NSIS_INSTALLER_MUI_ICON_CODE "")
set(CPACK_NSIS_INSTALL_ROOT "$PROGRAMFILES64")
set(CPACK_NSIS_PACKAGE_NAME "CURL 8.11.0-DEV")
set(CPACK_NSIS_UNINSTALL_NAME "Uninstall")
set(CPACK_OUTPUT_CONFIG_FILE "F:/Repos_2025/PruneCopy/PruneCopy/vcpkg_installed/x64-windows/vcpkg/blds/curl/x64-windows-dbg/CPackConfig.cmake")
set(CPACK_PACKAGE_DEFAULT_LOCATION "/")
set(CPACK_PACKAGE_DESCRIPTION_FILE "C:/Users/patri/AppData/Local/vcpkg/downloads/tools/cmake-3.30.1-windows/cmake-3.30.1-windows-i386/share/cmake-3.30/Templates/CPack.GenericDescription.txt")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "CURL built using CMake")
set(CPACK_PACKAGE_FILE_NAME "CURL-8.11.0-DEV-Source")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "CURL 8.11.0-DEV")
set(CPACK_PACKAGE_INSTALL_REGISTRY_KEY "CURL 8.11.0-DEV")
set(CPACK_PACKAGE_NAME "CURL")
set(CPACK_PACKAGE_RELOCATABLE "true")
set(CPACK_PACKAGE_VENDOR "Humanity")
set(CPACK_PACKAGE_VERSION "8.11.0-DEV")
set(CPACK_PACKAGE_VERSION_MAJOR "8")
set(CPACK_PACKAGE_VERSION_MINOR "11")
set(CPACK_PACKAGE_VERSION_PATCH "0-DEV")
set(CPACK_RESOURCE_FILE_LICENSE "C:/Users/patri/AppData/Local/vcpkg/downloads/tools/cmake-3.30.1-windows/cmake-3.30.1-windows-i386/share/cmake-3.30/Templates/CPack.GenericLicense.txt")
set(CPACK_RESOURCE_FILE_README "C:/Users/patri/AppData/Local/vcpkg/downloads/tools/cmake-3.30.1-windows/cmake-3.30.1-windows-i386/share/cmake-3.30/Templates/CPack.GenericDescription.txt")
set(CPACK_RESOURCE_FILE_WELCOME "C:/Users/patri/AppData/Local/vcpkg/downloads/tools/cmake-3.30.1-windows/cmake-3.30.1-windows-i386/share/cmake-3.30/Templates/CPack.GenericWelcome.txt")
set(CPACK_RPM_PACKAGE_SOURCES "ON")
set(CPACK_SET_DESTDIR "OFF")
set(CPACK_SOURCE_7Z "ON")
set(CPACK_SOURCE_GENERATOR "7Z;ZIP")
set(CPACK_SOURCE_IGNORE_FILES "/CVS/;/\\.svn/;/\\.bzr/;/\\.hg/;/\\.git/;\\.swp\$;\\.#;/#")
set(CPACK_SOURCE_INSTALLED_DIRECTORIES "F:/Repos_2025/PruneCopy/PruneCopy/vcpkg_installed/x64-windows/vcpkg/blds/curl/src/url-8_11_0-2be05894fb.clean;/")
set(CPACK_SOURCE_OUTPUT_CONFIG_FILE "F:/Repos_2025/PruneCopy/PruneCopy/vcpkg_installed/x64-windows/vcpkg/blds/curl/x64-windows-dbg/CPackSourceConfig.cmake")
set(CPACK_SOURCE_PACKAGE_FILE_NAME "CURL-8.11.0-DEV-Source")
set(CPACK_SOURCE_TOPLEVEL_TAG "win64-Source")
set(CPACK_SOURCE_ZIP "ON")
set(CPACK_STRIP_FILES "")
set(CPACK_SYSTEM_NAME "win64")
set(CPACK_THREADS "1")
set(CPACK_TOPLEVEL_TAG "win64-Source")
set(CPACK_WIX_SIZEOF_VOID_P "8")

if(NOT CPACK_PROPERTIES_FILE)
  set(CPACK_PROPERTIES_FILE "F:/Repos_2025/PruneCopy/PruneCopy/vcpkg_installed/x64-windows/vcpkg/blds/curl/x64-windows-dbg/CPackProperties.cmake")
endif()

if(EXISTS ${CPACK_PROPERTIES_FILE})
  include(${CPACK_PROPERTIES_FILE})
endif()
