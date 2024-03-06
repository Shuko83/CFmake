################################################################################
# Workspace definition
################################################################################

set(CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/external" CACHE PATH "Directories to be searched by find_package()")
# set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}") // Done in the CMakeList.txt
set(ARTIFACTORY_URL "https://artifactory.divst:8081/artifactory" CACHE STRING "Artifactory base URL")
