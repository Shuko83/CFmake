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
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
set(CMAKE_MODULE_LICENSE_PATH "${CMAKE_MODULE_PATH}/license" CACHE INTERNAL "CMake module license path")
set(CMAKE_MODULE_TEMPLATES_PATH "${CMAKE_MODULE_PATH}/templates" CACHE INTERNAL "CMake module templates path")

################################################################################
# Project definition
################################################################################
if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/install" CACHE PATH "Project installation path" FORCE)
endif()


################################################################################
# Project options
################################################################################

option(BUILD_DOCUMENTATION "Build documentation" FALSE)
option(BUILD_TESTING "Build unit tests" TRUE)
option(ENABLE_DOCUMENTATION_FULL "Enable full documentation" FALSE)
option(ENABLE_PDB_COPY "Enable PDB files copy" FALSE)
option(ENABLE_PDB_INSTALL "Enable PDB files installation" TRUE)
option(ENABLE_PDB_RELEASE "Enable PDB files for release" TRUE)

################################################################################
# Compilation definition
################################################################################

if(WIN32)
  set(TARGET_NAME "win-msvc2015-x64" CACHE INTERNAL "Target name")
else(WIN32)
  set(TARGET_NAME "linux-gcc12.2-x64" CACHE INTERNAL "Target name")
endif(WIN32)

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
  add_compile_options(/MP /W4 /wd4251 /wd4373)
  add_compile_definitions(NOMINMAX WIN32_LEAN_AND_MEAN)
  if(ENABLE_PDB_RELEASE)
    add_compile_options($<$<CONFIG:RELEASE>:/Zi>)
    add_link_options($<$<CONFIG:RELEASE>:/DEBUG>)
    add_link_options($<$<CONFIG:RELEASE>:/INCREMENTAL>)
  endif(ENABLE_PDB_RELEASE)
endif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")

# GCC configuration
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  add_compile_options(-Wall -Wextra -Wconversion -Wsign-conversion -Wold-style-cast)  # TODO: Add other warnings ?
  add_link_options($<$<CONFIG:RELEASE>:-Wl,-strip-all>)
endif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")

# Clang configuration
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Weverything -Wno-padded -Wno-c++98-compat -Wno-c++98-compat-pedantic)
  add_link_options($<$<CONFIG:RELEASE>:-Wl,-strip-all>)
endif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")

################################################################################
# Compilation options
################################################################################

option(ENABLE_CODE_COVERAGE "Enable code coverage" FALSE)
option(ENABLE_CODE_GUIDELINES "Enable code guidelines" FALSE)
option(ENABLE_CODE_SANITIZERS "Enable code sanitizers" FALSE)

# Code coverage
if(ENABLE_CODE_COVERAGE)
  add_compile_options($<$<CONFIG:DEBUG>:--coverage>)
endif(ENABLE_CODE_COVERAGE)

# Code guidelines
if(ENABLE_CODE_GUIDELINES)
  set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-header-filter=.*;-checks=*,-llvmlibc-*,-modernize-use-nodiscard,-modernize-use-trailing-return-type")
endif(ENABLE_CODE_GUIDELINES)

# Code sanitizers
if(ENABLE_CODE_SANITIZERS)
  add_compile_options($<$<CONFIG:DEBUG>:-fsanitize=address,dataflow,leak,memory,thread,undefined>)  # TODO: Set enabled sanitizers ?
endif(ENABLE_CODE_SANITIZERS)
