set(CPACK_GENERATOR "7Z" CACHE STRING "List of CPack generators to use.")
set(CPACK_PACKAGE_VENDOR "CSGroup" CACHE STRING "The name of the package vendor.")
set(CPACK_PACKAGE_DIRECTORY "${CMAKE_BINARY_DIR}/../package" CACHE STRING "The directory in which CPack is doing its packaging.")
set(CPACK_PACKAGE_VERSION "${CMAKE_PROJECT_VERSION}" CACHE STRING "Package full version.")
if(CMAKE_PROJECT_VERSION_IDENTIFIER)
    set(CPACK_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}_${CMAKE_PROJECT_VERSION}-${CMAKE_PROJECT_VERSION_IDENTIFIER}_${CFMAKE_BUILD_MKSPECS}" CACHE STRING "The name of the package file to generate.")
else()
    set(CPACK_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}_${CMAKE_PROJECT_VERSION}_${CFMAKE_BUILD_MKSPECS}" CACHE STRING "The name of the package file to generate.")
endif()

set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY OFF CACHE BOOL "Boolean toggle to include/exclude top level directory.")
set(CPACK_VERBATIM_VARIABLES ON CACHE BOOL "If set to TRUE, values of variables prefixed with CPACK_ will be escaped")

include(CPack)
