# cmake_minimum_required is mandatory
# CSToolkit curently require 3.27 minimum
cmake_minimum_required(VERSION 3.27)

# call to project is mandatory
# version can be defined with project(MyProject VERSION 1.2.3)
# or by defining CSTOOLKIT_PROJECT_VERSION : cmake -DCSTOOLKIT_PROJECT_VERSION="1.2.3"
project(MyProject)

# CSToolkit should be included near the top of top level CMakeLists.txt but after call to project()
include(../CSToolkit.cmake)

# cstoolkit_fetch_artifactory simplify the downloding of artifactory packages
cstoolkit_fetch_artifactory("TinyXML2" VERSION "5.0.1-2")
#cstoolkit_fetch_artifactory("Cryptopp" VERSION "5.0.1-2")

# call to add_subdirectory includes the CMakeLists.txt in that directory
# it's is not mandatory but recomended for a cleaner Directory Structure
# order is not important
add_subdirectory(MyExe)
#add_subdirectory(MyExeWithPlugins)
add_subdirectory(MyLib)
#add_subdirectory(MyPlugin)
#add_subdirectory(MyStaticLib)
#add_subdirectory(MyCombinedStaticLib)

include(CPack)
