# Include guard to avoid double inclusion of cstoolkit
if(__CSTOOLKIT_GUARD__)
  return()
endif()
set(__CSTOOLKIT_GUARD__ TRUE)

cmake_minimum_required(VERSION 3.27)

if(POLICY CMP0152)
  cmake_policy(SET CMP0152 NEW)
endif()

if(NOT PROJECT_NAME)
    message(FATAL_ERROR "CSToolkit: CSToolkit should be included after top level project() call.")
    return()
endif()

################################################################################
# GLOBAL MODIFIERS
################################################################################

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

################################################################################
# Cache variables to override if needed
################################################################################

if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  cmake_path(SET _install_path NORMALIZE "${CMAKE_BINARY_DIR}/../install")
  set(CMAKE_INSTALL_PREFIX "${_install_path}" CACHE PATH "Default install directory used by install()." FORCE)
endif()

set(CSTOOLKIT_EXTERNALS "${CMAKE_BINARY_DIR}/externals" CACHE PATH "Directory where externals project are situated")

list(APPEND CMAKE_PREFIX_PATH "${CSTOOLKIT_EXTERNALS}")

set(CSTOOLKIT_ARTIFACTORY_URL "http://artifactory.divst:8081/artifactory" CACHE STRING "Url of artifactory")

set(CSTOOLKIT_PROJECT_VERSION 0.0.0.0 CACHE STRING "Version of the project, will set CMAKE_PROJECT_VERSION and PROJECT_VERSION")

set(CSTOOLKIT_INSTALL_TARGETS "" CACHE STRING "List of targets to install, if empty install all targets")

set(CSTOOLKIT_DEFAULT_PUBLIC_HEADERS_DIRS "include" CACHE STRING "Default folder for public headers of a target created with cstoolkit_add_target")
set(CSTOOLKIT_DEFAULT_PRIVATE_HEADERS_DIRS "src" CACHE STRING "Default folder for private headers of a target created with cstoolkit_add_target")
set(CSTOOLKIT_DEFAULT_SOURCES_DIRS "src" CACHE STRING "Default folder for source files of a target created with cstoolkit_add_target")
set(CSTOOLKIT_DEFAULT_UI_DIRS "src" CACHE STRING "Default folder for Qt ui files of a target created with cstoolkit_add_target")
set(CSTOOLKIT_DEFAULT_RESOURCES_DIRS "src" CACHE STRING "Default folder for Qt qrc files of a target created with cstoolkit_add_target")
set(CSTOOLKIT_DEFAULT_TRANSLATION_DIRS "src" CACHE STRING "Default folder for Qt ts files of a target created with cstoolkit_add_target")

################################################################################
# Options
################################################################################

option(CSTOOLKIT_AUTO_FIND_PACKAGE "Automatically calls find_package on unknown libraries of all targets" ON)
option(CSTOOLKIT_CHECK_DEPENDENCIES "Raise a warning if a dependency of a target is not defined" ON)
option(CSTOOLKIT_DISABLE_COMMON_CPPRULES "Does not include common DivST Cpp Rules" OFF)
option(CSTOOLKIT_USE_GIT_TAG_VERSION "Set project version from git tag or branch name if possible" OFF)
option(CSTOOLKIT_CPACK_RULES "Add CPack configuration to the project" OFF)
option(CSTOOLKIT_AUTO_DEPLOY_QT "Add qt deploy rules to all Qt dependent executables" ON)
option(CSTOOLKIT_PREFIX_OUTPUT_NAME "If ON, all targets ouput file are prefixed by project name" OFF)

################################################################################
#  Internal variables
################################################################################

set(CSTOOLKIT_ROOT_DIR "${CMAKE_CURRENT_LIST_DIR}")
set(CSTOOLKIT_COPY "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_LIST_DIR}/scripts/cstoolkit_copy.cmake" --)
set(CSTOOLKIT_INSTALL_TARGETS_ALL)

################################################################################
#  Includes
################################################################################

include(${CMAKE_CURRENT_LIST_DIR}/utility/BuildType.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/utility/Target.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/utility/Info.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/utility/PostConfigure.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/utility/Deploy.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/utility/Fetch.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/utility/Version.cmake)
if(NOT CSTOOLKIT_DISABLE_COMMON_CPPRULES)
    include(${CMAKE_CURRENT_LIST_DIR}/utility/CppRules.cmake)
endif()
include(${CMAKE_CURRENT_LIST_DIR}/utility/mkspecs.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/utility/Windows.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/utility/Qt.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/utility/WindowsRcFile.cmake)
if(CSTOOLKIT_CPACK_RULES)
    include(${CMAKE_CURRENT_LIST_DIR}/utility/CPackRules.cmake)
endif()

cmake_language(DEFER DIRECTORY ${CMAKE_SOURCE_DIR} CALL cstoolkit_post_configure())
