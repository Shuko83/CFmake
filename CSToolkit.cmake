cmake_minimum_required(VERSION 3.27)

################################################################################
#  Includes
################################################################################

include(${CMAKE_CURRENT_LIST_DIR}/utility/Target.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/utility/Info.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/utility/PostConfigure.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/utility/Deploy.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/utility/Fetch.cmake)
set(CMAKE_PROJECT_INCLUDE ${CMAKE_CURRENT_LIST_DIR}/utility/PostProject.cmake)
cmake_language(DEFER DIRECTORY ${CMAKE_SOURCE_DIR} CALL cstoolkit_post_configure())

################################################################################
# Project definition
################################################################################

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

#set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/install") // Done in PostProject.cmake

#set(CMAKE_PREFIX_PATH "${CMAKE_BINARY_DIR}/externals") // Done in Fetch.cmake

set(CSTOOLKIT_PROJECT_VERSION 3.0.0.0 CACHE STRING "Version of the project, will set CMAKE_PROJECT_VERSION and PROJECT_VERSION")

set(CSTOOLKIT_COPY "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_LIST_DIR}/scripts/cstoolkit_copy.cmake" --)

################################################################################
# Options
################################################################################

option(CSTOOLKIT_AUTO_FIND_PACKAGE "Automatically calls find_package on unknown libraries passed to add_target()")
