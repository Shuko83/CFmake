################################################################################
# Common includes
################################################################################

include(Authentications)
include(Dependencies)
include(Documentations)
include(Installations)
include(LibFindMacros)
include(Libraries)
include(Logins)
include(Packages)
include(Projects)
include(Tests)
include(Tools)
include(Variables)

################################################################################
# Workspace definition
################################################################################

set(WORKSPACE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/.." CACHE PATH "Workspace path")
set(SUBPROJECTS_PATH "${CMAKE_BINARY_DIR}/.." CACHE PATH "Sub-projects path")
set(CMAKE_PREFIX_PATH "${WORKSPACE_PATH}/external" CACHE PATH "External dependencies path")
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" CACHE INTERNAL "CMake module path")
set(CMAKE_MODULE_LICENSE_PATH "${CMAKE_MODULE_PATH}/license" CACHE INTERNAL "CMake module license path")
set(CMAKE_MODULE_TEMPLATES_PATH "${CMAKE_MODULE_PATH}/templates" CACHE INTERNAL "CMake module templates path")

################################################################################
# Project definition
################################################################################

set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install" CACHE PATH "Project installation path")
set(CMAKE_PROJECT_VERSION "0.0.0" CACHE INTERNAL "Project version")

################################################################################
# Project options
################################################################################

option(BUILD_DOCUMENTATION "Build documentation" FALSE)
option(BUILD_TESTING "Build unit tests" TRUE)
option(ENABLE_DOCUMENTATION_FULL "Enable full documentation" FALSE)
option(ENABLE_PDB_COPY "Enable PDB files copy" FALSE)
option(ENABLE_PDB_INSTALL "Enable PDB files installation" TRUE)

################################################################################
# Compilation definition
################################################################################

set(TARGET_NAME "win-msvc2015-x64" CACHE INTERNAL "Target name")

# Common configuration
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/$<LOWER_CASE:$<CONFIG>>)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/$<LOWER_CASE:$<CONFIG>>)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<LOWER_CASE:$<CONFIG>>)
set(CMAKE_DEBUG_POSTFIX d)
set(CMAKE_MINSIZEREL_POSTFIX s)
set(CMAKE_RELWITHDEBINFO_POSTFIX rd)
# set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)

# MSVC configuration
if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
  set(CMAKE_GENERATOR_PLATFORM "x64" CACHE INTERNAL "Target architecture")
  string(APPEND CMAKE_CXX_FLAGS " /MP /W4 /wd4251 /wd4373")
  add_compile_definitions(NOMINMAX WIN32_LEAN_AND_MEAN)
endif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")

# GCC configuration
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  string(APPEND CMAKE_CXX_FLAGS_DEBUG " -Wall -Wextra -Wconversion -Wsign-conversion -Wold-style-cast")  # TODO: Add other warnings ?
  string(APPEND CMAKE_CXX_FLAGS_RELEASE " -w -Wl,-s")  # TODO: Set linker strip flag here ?
endif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")

# Clang configuration
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  string(APPEND CMAKE_CXX_FLAGS_DEBUG " -Weverything -Wno-padded -Wno-c++98-compat -Wno-c++98-compat-pedantic")
  string(APPEND CMAKE_CXX_FLAGS_RELEASE " -w -Wl,-s")  # TODO: Set linker strip flag here ?
endif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")

################################################################################
# Compilation options
################################################################################

option(ENABLE_CODE_COVERAGE "Enable code coverage" FALSE)
option(ENABLE_CODE_GUIDELINES "Enable code guidelines" FALSE)
option(ENABLE_CODE_SANITIZERS "Enable code sanitizers" FALSE)

# Code coverage
if(ENABLE_CODE_COVERAGE)
  string(APPEND CMAKE_CXX_FLAGS_DEBUG " --coverage")
endif(ENABLE_CODE_COVERAGE)

# Code guidelines
if(ENABLE_CODE_GUIDELINES)
  string(APPEND CMAKE_CXX_CLANG_TIDY " clang-tidy -header-filter=.* -checks=*,-llvmlibc-*,-modernize-use-nodiscard,-modernize-use-trailing-return-type --extra-arg=-I${CMAKE_CURRENT_SOURCE_DIR}/include")
endif(ENABLE_CODE_GUIDELINES)

# Code sanitizers
if(ENABLE_CODE_SANITIZERS)
  string(APPEND CMAKE_CXX_FLAGS_DEBUG " -fsanitize=address,dataflow,leak,memory,thread,undefined")  # TODO: Set enabled sanitizers ?
endif(ENABLE_CODE_SANITIZERS)
