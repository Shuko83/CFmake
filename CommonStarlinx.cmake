
include(Common)
include(generate_target_info)

set(CMAKE_PROJECT_INCLUDE "${CMAKE_SOURCE_DIR}/cmake/Version.cmake")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# # Allow to call CommonStarlinx before project()
# set(CMAKE_PROJECT_INCLUDE "${CMAKE_SOURCE_DIR}/cmake/CommonStarlinx.cmake")

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(ARTIFACTORY_URL "http://artifactory.divst:8081/artifactory" CACHE STRING "Artifactory base URL")

set(Qt5_VERSION "5.9.6" CACHE INTERNAL "Qt5 version")
set(Qt5_ARCH "msvc2015_64" CACHE INTERNAL "Qt5 architecture")

add_compile_definitions(UNICODE)