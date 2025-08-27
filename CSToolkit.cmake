# Include guard to avoid double inclusion of cstoolkit
if(__CSTOOLKIT_GUARD__)
    return()
endif()
set(__CSTOOLKIT_GUARD__ 1)

if(CMAKE_MINIMUM_REQUIRED_VERSION VERSION_LESS "3.27")
    cmake_minimum_required(VERSION 3.27)
endif()

if(POLICY CMP0144)
    cmake_policy(SET CMP0144 NEW)
endif()
if(POLICY CMP0152)
    cmake_policy(SET CMP0152 NEW)
endif()
if(POLICY CMP0174)
    cmake_policy(SET CMP0174 NEW)
endif()

if(NOT PROJECT_NAME)
    message(FATAL_ERROR "CSToolkit: CSToolkit.cmake included before top level project() call")
    return()
endif()

################################################################################
#  Timer
################################################################################

include(${CMAKE_CURRENT_LIST_DIR}/utility/Timer.cmake)

cstoolkit_start_timer(CSTOOLKIT_CONFIGURE_TIMER)

string(TIMESTAMP CSTOOLKIT_CONFIGURATION_DATE "%a, %d %b %Y %H:%M:%S %z")

################################################################################
#  Early Includes
################################################################################

include(${CMAKE_CURRENT_LIST_DIR}/utility/Conan.cmake)

################################################################################
# GLOBAL MODIFIERS
################################################################################

set(CMAKE_NETRC "OPTIONAL")
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

################################################################################
# Cache variables to override if needed
################################################################################

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    cmake_path(SET _install_path NORMALIZE "${CMAKE_BINARY_DIR}/../install")
    set(CMAKE_INSTALL_PREFIX "${_install_path}" CACHE PATH "Default install directory used by install()." FORCE)
endif()

set(CSTOOLKIT_EXTERNALS "${CMAKE_BINARY_DIR}/externals" CACHE PATH "Directory where externals project are situated")
set(CSTOOLKIT_DOWNLOAD_BASE_DIR "${CMAKE_BINARY_DIR}/_cstkdl" CACHE PATH "Directory where files are downloaded before being extracted by CSToolkit")

list(APPEND CMAKE_PREFIX_PATH "${CSTOOLKIT_EXTERNALS}")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/modules")

set(CSTOOLKIT_ARTIFACTORY_URL "http://artifactory.divst:8081/artifactory" CACHE STRING "Url of artifactory")
set(CSTOOLKIT_NEXUS_DR_URL "https://dr-aix-nexus-01v.sccoa.si.c-s.fr/repository" CACHE STRING "Url of Nexus DR")
set(CSTOOLKIT_NEXUS_ITAR_URL "https://itar-aix-nexus-01v.sccoa.si.c-s.fr/repository" CACHE STRING "Url of Nexus ITAR")

set(CSTOOLKIT_PROJECT_VERSION 0.0.0.0 CACHE STRING "Version of the project, will set CMAKE_PROJECT_VERSION and PROJECT_VERSION")

set(CSTOOLKIT_INSTALL_TARGETS "" CACHE STRING "List of targets to install, if empty install all targets")

set(CSTOOLKIT_DEFAULT_PUBLIC_HEADERS_DIRS "include" CACHE STRING "Default folder for public headers of a target created with cstoolkit_add_target")
set(CSTOOLKIT_DEFAULT_PRIVATE_HEADERS_DIRS "src" CACHE STRING "Default folder for private headers of a target created with cstoolkit_add_target")
set(CSTOOLKIT_DEFAULT_SOURCES_DIRS "src" CACHE STRING "Default folder for source files of a target created with cstoolkit_add_target")
set(CSTOOLKIT_DEFAULT_UI_DIRS "src" CACHE STRING "Default folder for Qt ui files of a target created with cstoolkit_add_target")
set(CSTOOLKIT_DEFAULT_RESOURCES_DIRS "src" CACHE STRING "Default folder for Qt qrc files of a target created with cstoolkit_add_target")
set(CSTOOLKIT_DEFAULT_TRANSLATION_DIRS "src" CACHE STRING "Default folder for Qt ts files of a target created with cstoolkit_add_target")

set(CSTOOLKIT_QT_BIG_RESOURCES_THRESHOLD 100000000 CACHE STRING "Size threshold for qt big_resources") #100 Mo

set(CSTOOLKIT_DEFAULT_INSTALL_LIBDIR "$<TARGET_PROPERTY:INSTALL_DESTINATION>/lib/$<LOWER_CASE:$<CONFIG>>" CACHE STRING "Default install folder for library files of a target created with cstoolkit_add_target")
set(CSTOOLKIT_DEFAULT_INSTALL_BINDIR "$<TARGET_PROPERTY:INSTALL_DESTINATION>/bin/$<LOWER_CASE:$<CONFIG>>" CACHE STRING "Default install folder for binary files of a target created with cstoolkit_add_target")
set(CSTOOLKIT_DEFAULT_INSTALL_SYMBOLSDIR "$<TARGET_PROPERTY:INSTALL_DESTINATION>/symbols/$<LOWER_CASE:$<CONFIG>>" CACHE STRING "Default install folder for symbol files of a target created with cstoolkit_add_target")
set(CSTOOLKIT_DEFAULT_INSTALL_INCLUDEDIR "$<TARGET_PROPERTY:INSTALL_DESTINATION>/include" CACHE STRING "Default install folder for header files of a target created with cstoolkit_add_target")
set(CSTOOLKIT_DEFAULT_INSTALL_CMAKEDIR "$<TARGET_PROPERTY:INSTALL_DESTINATION>/cmake" CACHE STRING "Default install folder for cmake files of a target created with cstoolkit_add_target")

################################################################################
# Options
################################################################################

option(CSTOOLKIT_AUTO_FIND_PACKAGE "Automatically calls find_package on unknown libraries of all targets" ON)
option(CSTOOLKIT_CHECK_DEPENDENCIES "Raise a warning if a dependency of a target is not defined" ON)
option(CSTOOLKIT_COMMON_CPPRULES "Include common DivST Cpp Rules" ON)
option(CSTOOLKIT_USE_GIT_TAG_VERSION "Set project version from git tag or branch name if possible" OFF)
option(CSTOOLKIT_CPACK_RULES "Add CPack configuration to the project" OFF)
option(CSTOOLKIT_AUTO_DEPLOY_QT "Add qt deploy rules to all Qt dependent executables" ON)
option(CSTOOLKIT_PREFIX_OUTPUT_NAME "If ON, all targets output file are prefixed by project name" OFF)
option(CSTOOLKIT_FETCH_PACKAGE_PREFER_MODULE "If ON, cstoolkit_fetch_package will search for Find<package> files before <package>Config files" OFF)
if(CONAN)
    option(CSTOOLKIT_GENERATE_FETCH_DEPENDENCY "If ON, calls to cstoolkit_fetch_dependency will be generated in Config files" OFF)
    option(CSTOOLKIT_FETCH_DEPENDENCY "If OFF, calls to cstoolkit_fetch_dependency will do nothing" OFF)
else()
    option(CSTOOLKIT_GENERATE_FETCH_DEPENDENCY "If ON, calls to cstoolkit_fetch_dependency will be generated in Config files" ON)
    option(CSTOOLKIT_FETCH_DEPENDENCY "If OFF, calls to cstoolkit_fetch_dependency will do nothing" ON)
endif()
option(CSTOOLKIT_WINDOWS_SDK_WARNING "If ON, cstoolkit will raise a CMake warning if the Windows SDK is missing or incompatible with the MSVC version" ON)

################################################################################
#  Internal variables
################################################################################

set(CSTOOLKIT_ROOT_DIR "${CMAKE_CURRENT_LIST_DIR}")
set(CSTOOLKIT_COPY "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_LIST_DIR}/scripts/cstoolkit_copy.cmake" --)
set_property(GLOBAL PROPERTY CSTOOLKIT_INSTALLED_TARGETS "")
set_property(GLOBAL PROPERTY CSTOOLKIT_INSTALLED_TARGETS_ALL "")
add_custom_target(CSTOOLKIT)
if(PREDEFINED_TARGETS_FOLDER)
    set_target_properties(CSTOOLKIT PROPERTIES FOLDER ${PREDEFINED_TARGETS_FOLDER})
else()
    set_target_properties(CSTOOLKIT PROPERTIES FOLDER "CMakePredefinedTargets")
endif()

################################################################################
# Legacy Support
################################################################################

if(CSTOOLKIT_DISABLE_COMMON_CPPRULES)
    message(NOTICE ${COLOR_YELLOW_BOLD} "CSToolkit: CSTOOLKIT_DISABLE_COMMON_CPPRULES option deprecated, use CSTOOLKIT_COMMON_CPPRULES=OFF." ${COLOR_RESET})
    set(CSTOOLKIT_COMMON_CPPRULES OFF CACHE BOOL "Include common DivST Cpp Rules" FORCE)
endif()

################################################################################
#  Includes
################################################################################

include(${CMAKE_CURRENT_LIST_DIR}/utility/Color.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/utility/Debug.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/utility/BuildType.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/utility/Arguments.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/utility/File.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/utility/Genex.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/utility/Target.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/utility/Info.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/utility/PostConfigure.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/utility/Deploy.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/utility/Fetch.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/utility/Git.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/utility/Version.cmake)
if(CSTOOLKIT_COMMON_CPPRULES)
    include(${CMAKE_CURRENT_LIST_DIR}/utility/CppRules.cmake)
endif()
include(${CMAKE_CURRENT_LIST_DIR}/utility/mkspecs.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/utility/Windows.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/utility/Qt.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/utility/WindowsRcFile.cmake)
if(CSTOOLKIT_CPACK_RULES)
    include(${CMAKE_CURRENT_LIST_DIR}/utility/CPackRules.cmake)
endif()
include(${CMAKE_CURRENT_LIST_DIR}/utility/Streamwork.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/utility/CODX.cmake)

cmake_language(DEFER DIRECTORY ${CMAKE_SOURCE_DIR} CALL cstoolkit_post_configure())
