# Include guard to avoid double inclusion of cfmake
if(__CFMAKE_GUARD__)
    return()
endif()
set(__CFMAKE_GUARD__ 1)

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
    message(FATAL_ERROR "CFMake: CFMake.cmake included before top level project() call")
    return()
endif()

################################################################################
#  Timer
################################################################################

include(${CMAKE_CURRENT_LIST_DIR}/utility/Timer.cmake)

cfmake_start_timer(CFMAKE_CONFIGURE_TIMER)

string(TIMESTAMP CFMAKE_CONFIGURATION_DATE "%a, %d %b %Y %H:%M:%S %z")

################################################################################
#  Early Includes
################################################################################

include(${CMAKE_CURRENT_LIST_DIR}/utility/Conan.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/utility/Internal.cmake OPTIONAL)

################################################################################
# GLOBAL MODIFIERS
################################################################################

set(CMAKE_NETRC "OPTIONAL")
set_property(GLOBAL PROPERTY USE_FOLDERS 1)
set_property(GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS  1)
set_property(GLOBAL PROPERTY FIND_LIBRARY_USE_LIB32_PATHS  1)

################################################################################
# Cache variables to override if needed
################################################################################

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    cmake_path(SET _install_path NORMALIZE "${CMAKE_BINARY_DIR}/../install")
    set(CMAKE_INSTALL_PREFIX "${_install_path}" CACHE PATH "Default install directory used by install()." FORCE)
endif()

set(CFMAKE_EXTERNALS "${CMAKE_BINARY_DIR}/externals" CACHE PATH "Directory where externals project are situated")
set(CFMAKE_DOWNLOAD_BASE_DIR "${CMAKE_BINARY_DIR}/_cstkdl" CACHE PATH "Directory where files are downloaded before being extracted by CFMake")

list(APPEND CMAKE_PREFIX_PATH "${CFMAKE_EXTERNALS}")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/modules")

set(CFMAKE_PROJECT_VERSION 0.0.0.0 CACHE STRING "Version of the project, will set CMAKE_PROJECT_VERSION and PROJECT_VERSION")

set(CFMAKE_INSTALL_TARGETS "" CACHE STRING "List of targets to install, if empty install all targets")

set(CFMAKE_DEFAULT_FILES_MODE "EXPLICIT" CACHE STRING "Default mode for file discovery in cfmake_add_target (EXPLICIT, AUTO, APPEND)")
set(CFMAKE_DEFAULT_PUBLIC_HEADERS_DIRS "include" CACHE STRING "Default folder for public headers of a target created with cfmake_add_target")
set(CFMAKE_DEFAULT_PRIVATE_HEADERS_DIRS "src" CACHE STRING "Default folder for private headers of a target created with cfmake_add_target")
set(CFMAKE_DEFAULT_SOURCES_DIRS "src" CACHE STRING "Default folder for source files of a target created with cfmake_add_target")
set(CFMAKE_DEFAULT_UI_DIRS "src" CACHE STRING "Default folder for Qt ui files of a target created with cfmake_add_target")
set(CFMAKE_DEFAULT_RESOURCES_DIRS "src" CACHE STRING "Default folder for Qt qrc files of a target created with cfmake_add_target")
set(CFMAKE_DEFAULT_TRANSLATION_DIRS "src" CACHE STRING "Default folder for Qt ts files of a target created with cfmake_add_target")

set(CFMAKE_DEFAULT_INSTALL_LIBDIR "$<TARGET_PROPERTY:INSTALL_DESTINATION>/lib/$<LOWER_CASE:$<CONFIG>>" CACHE STRING "Default install folder for library files of a target created with cfmake_add_target")
set(CFMAKE_DEFAULT_INSTALL_BINDIR "$<TARGET_PROPERTY:INSTALL_DESTINATION>/bin/$<LOWER_CASE:$<CONFIG>>" CACHE STRING "Default install folder for binary files of a target created with cfmake_add_target")
set(CFMAKE_DEFAULT_INSTALL_SYMBOLSDIR "$<TARGET_PROPERTY:INSTALL_DESTINATION>/symbols/$<LOWER_CASE:$<CONFIG>>" CACHE STRING "Default install folder for symbol files of a target created with cfmake_add_target")
set(CFMAKE_DEFAULT_INSTALL_INCLUDEDIR "$<TARGET_PROPERTY:INSTALL_DESTINATION>/include" CACHE STRING "Default install folder for header files of a target created with cfmake_add_target")
set(CFMAKE_DEFAULT_INSTALL_CMAKEDIR "$<TARGET_PROPERTY:INSTALL_DESTINATION>/cmake" CACHE STRING "Default install folder for cmake files of a target created with cfmake_add_target")

set(CFMAKE_CXX_SOURCE_FILE_EXTENSIONS "c;C;c++;cc;cpp;cxx;CPP" CACHE STRING "File extensions for C++ source files")
set(CFMAKE_CXX_HEADER_FILE_EXTENSIONS "h;H;h++;hh;hpp;hxx;HPP;inl" CACHE STRING "File extensions for C++ header files")
set(CFMAKE_CSharp_SOURCE_FILE_EXTENSIONS "cs" CACHE STRING "File extensions for CSharp source files")

set(CFMAKE_QT_BIG_RESOURCES_THRESHOLD 100000000 CACHE STRING "Size threshold for qt big_resources") #100 Mo

################################################################################
# Options
################################################################################

option(CFMAKE_AUTO_FIND_PACKAGE "Automatically calls find_package on unknown libraries of all targets" ON)
option(CFMAKE_CHECK_DEPENDENCIES "Raise a warning if a dependency of a target is not defined" ON)
option(CFMAKE_COMMON_CPPRULES "Include common DivST Cpp Rules" ON)
option(CFMAKE_USE_GIT_TAG_VERSION "Set project version from git tag or branch name if possible" OFF)
option(CFMAKE_CPACK_RULES "Add CPack configuration to the project" OFF)
option(CFMAKE_AUTO_DEPLOY_QT "Add qt install rules to all Qt dependent executables" ON)
option(CFMAKE_BUILD_DEPLOY "Copy runtime dependencies and other files to the build directory" ON)
option(CFMAKE_PREFIX_OUTPUT_NAME "If ON, all targets output file are prefixed by project name" OFF)

option(CFMAKE_KEEP_ARCHIVE "If OFF, deletes the archive file after successful extraction to save disk space" ON)
option(CFMAKE_AUTO_REFRESH_TARGET_FILES "If ON, automatically reconfigure when target files are added, removed, or renamed" OFF)
option(CFMAKE_WINDOWS_SDK_WARNING "If ON, cfmake will raise a CMake warning if the Windows SDK is missing or incompatible with the MSVC version" ON)
option(CFMAKE_INSTALL_SYMBOLS_TO_SYMBOLSDIR "If ON, all debug files are installed in the CFMAKE_DEFAULT_INSTALL_SYMBOLSDIR directory instead of alongside binaries" OFF)

################################################################################
#  Internal variables
################################################################################

set(CFMAKE_ROOT_DIR "${CMAKE_CURRENT_LIST_DIR}")
set(CFMAKE_COPY "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_LIST_DIR}/scripts/cfmake_copy.cmake" --)
set(CFMAKE_LOCK "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_LIST_DIR}/scripts/cfmake_lock.cmake" --)
set_property(GLOBAL PROPERTY CFMAKE_INSTALLED_TARGETS "")
set_property(GLOBAL PROPERTY CFMAKE_INSTALLED_TARGETS_ALL "")
add_custom_target(CFMAKE)
if(PREDEFINED_TARGETS_FOLDER)
    set_target_properties(CFMAKE PROPERTIES FOLDER ${PREDEFINED_TARGETS_FOLDER})
else()
    set_target_properties(CFMAKE PROPERTIES FOLDER "CMakePredefinedTargets")
endif()

################################################################################
# Legacy Support
################################################################################

if(CFMAKE_DISABLE_COMMON_CPPRULES)
    message(NOTICE ${COLOR_YELLOW_BOLD} "CFMake: CFMAKE_DISABLE_COMMON_CPPRULES option deprecated, use CFMAKE_COMMON_CPPRULES=OFF." ${COLOR_RESET})
    set(CFMAKE_COMMON_CPPRULES OFF CACHE BOOL "Include common DivST Cpp Rules" FORCE)
endif()

################################################################################
#  Includes
################################################################################

include(${CMAKE_CURRENT_LIST_DIR}/utility/Color.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/utility/Debug.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/utility/Regex.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/utility/BuildType.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/utility/Arguments.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/utility/File.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/utility/Genex.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/utility/Target.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/utility/Info.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/utility/PostConfigure.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/utility/Deploy.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/utility/Git.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/utility/Version.cmake)
if(CFMAKE_COMMON_CPPRULES)
    include(${CMAKE_CURRENT_LIST_DIR}/utility/CppRules.cmake)
endif()
include(${CMAKE_CURRENT_LIST_DIR}/utility/mkspecs.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/utility/Windows.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/utility/Qt.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/utility/VortexOpenSplice.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/utility/WindowsRcFile.cmake)
if(CFMAKE_CPACK_RULES)
    include(${CMAKE_CURRENT_LIST_DIR}/utility/CPackRules.cmake)
endif()
include(${CMAKE_CURRENT_LIST_DIR}/utility/Streamwork.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/utility/CODX.cmake)

cmake_language(DEFER DIRECTORY ${CMAKE_SOURCE_DIR} CALL cfmake_post_configure())
