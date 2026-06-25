# cmake_minimum_required is mandatory
# CFMake currently require 3.27 minimum
cmake_minimum_required(VERSION 3.27)

# call to project is mandatory
# version can be defined with project(MyProject VERSION 1.2.3)
# or by defining CFMAKE_PROJECT_VERSION : cmake -DCFMAKE_PROJECT_VERSION="1.2.3-alpha"
project(MyProject)

# Options and variables should be set before inclusion of CFMake
set(CFMAKE_DEFAULT_RESOURCES_DIRS "resources")
#set(CFMAKE_INSTALL_TARGETS "MyExe")
set(CFMAKE_CPACK_RULES ON)

# CFMake should be included near the top of top level CMakeLists.txt but after call to project()
include(../CFMake.cmake)

set(CMAKE_INSTALL_PREFIX "install/$<CONFIG>")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")


# call to add_subdirectory includes the CMakeLists.txt in that directory
# it's is not mandatory but recomended for a cleaner Directory Structure
# order is not important
add_subdirectory(gallery)
add_subdirectory(MyExe)
add_subdirectory(MySharedAndStaticLib)
add_subdirectory(MyLib)
add_subdirectory(MyQtLib)
add_subdirectory(MyQtExe)
add_subdirectory(MyStaticLib)
add_subdirectory(MyCombinedStaticLib)
add_subdirectory(MyExeWithPlugins)
add_subdirectory(MyPlugin)
