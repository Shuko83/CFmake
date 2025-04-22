# cmake_minimum_required is mandatory
# CSToolkit currently require 3.27 minimum
cmake_minimum_required(VERSION 3.27)

# call to project is mandatory
# version can be defined with project(MyProject VERSION 1.2.3)
# or by defining CSTOOLKIT_PROJECT_VERSION : cmake -DCSTOOLKIT_PROJECT_VERSION="1.2.3-alpha"
project(MyProject)

# Options and variables should be set before inclusion of CSToolkit
set(CSTOOLKIT_DEFAULT_RESOURCES_DIRS "resources")
#set(CSTOOLKIT_INSTALL_TARGETS "MyExe")
set(CSTOOLKIT_CPACK_RULES ON)

# CSToolkit should be included near the top of top level CMakeLists.txt but after call to project()
include(../CSToolkit.cmake)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

# cstoolkit_fetch_artifactory simplify the downloding of artifactory packages
cstoolkit_fetch_artifactory("TinyXML2" VERSION "5.0.1-4")
#cstoolkit_fetch_artifactory("Boost" VERSION "1.78.0")

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
