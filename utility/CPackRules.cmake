set(CPACK_GENERATOR "7Z" CACHE STRING "List of CPack generators to use.")
set(CPACK_PACKAGE_VENDOR "CSGroup" CACHE STRING "The name of the package vendor.")
set(CPACK_PACKAGE_DIRECTORY "${CMAKE_BINARY_DIR}/../install" CACHE STRING "The directory in which CPack is doing its packaging.")
set(CPACK_PACKAGE_VERSION "${CMAKE_PROJECT_VERSION}" CACHE STRING "Package full version")

set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY OFF)
set(CPACK_VERBATIM_VARIABLES ON)

include(CPack)
