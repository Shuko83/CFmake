cmake_minimum_required(VERSION 3.27)

################################################################################
#  Includes
################################################################################

include(${CMAKE_CURRENT_LIST_DIR}/utility/Target.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/utility/Info.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/utility/PostConfigure.cmake)
set(CMAKE_PROJECT_INCLUDE ${CMAKE_CURRENT_LIST_DIR}/utility/PostProject.cmake)
cmake_language(DEFER DIRECTORY ${CMAKE_SOURCE_DIR} CALL cstoolkit_post_configure())

################################################################################
# Project definition
################################################################################

#set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/install") // Done in PostProject.cmake

set(PACKAGE_VERSION 0.0.0.0 CACHE STRING "Version of the package, will set CMAKE_PROJECT_VERSION and PROJECT_VERSION")

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

################################################################################
# Options
################################################################################

option(CSTOOLKIT_AUTO_FIND_PACKAGE "Automatically calls find_package on unknown libraries passed to add_target()")

