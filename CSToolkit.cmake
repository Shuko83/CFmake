cmake_minimum_required(VERSION 3.27)

################################################################################
#  Includes
################################################################################

include(${CMAKE_CURRENT_LIST_DIR}/utility/Target.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/utility/Info.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/utility/PostConfigure.cmake)
set(CMAKE_PROJECT_INCLUDE ${CMAKE_CURRENT_LIST_DIR}/utility/PostProject.cmake)
cmake_language(DEFER DIRECTORY ${CMAKE_SOURCE_DIR} CALL cstoolkit_post_configure())

################################################################################
# Project definition
################################################################################

#set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/install") // Done in PostProject.cmake

set_property(GLOBAL PROPERTY USE_FOLDERS ON)



################################################################################
# Options
################################################################################

option(CSTOOLKIT_AUTO_FIND_PACKAGE "Automatically calls find_package on unknown libraries passed to add_target()")

################################################################################
# Compilation definition
################################################################################

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

add_compile_definitions(UNICODE)

# MSVC configuration
if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
  set(CMAKE_GENERATOR_PLATFORM "x64" CACHE INTERNAL "Target architecture")
  add_compile_options(/MP /W4 /wd4251 /wd4373)
  add_compile_definitions(NOMINMAX WIN32_LEAN_AND_MEAN)
  if(ENABLE_PDB_RELEASE)
    add_compile_options($<$<CONFIG:RELEASE>:/Zi>)
    add_link_options($<$<CONFIG:RELEASE>:/DEBUG>)
    add_link_options($<$<CONFIG:RELEASE>:/INCREMENTAL>)
  endif()
endif()

# GCC configuration
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  add_compile_options(-Wall -Wextra -Wconversion -Wsign-conversion -Wold-style-cast)  # TODO: Add other warnings ?
  add_link_options($<$<CONFIG:RELEASE>:-Wl,-strip-all>)
endif()

# Clang configuration
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Weverything -Wno-padded -Wno-c++98-compat -Wno-c++98-compat-pedantic)
  add_link_options($<$<CONFIG:RELEASE>:-Wl,-strip-all>)
endif()

################################################################################
# Compilation options
################################################################################

option(ENABLE_CODE_COVERAGE "Enable code coverage" FALSE)
option(ENABLE_CODE_GUIDELINES "Enable code guidelines" FALSE)
option(ENABLE_CODE_SANITIZERS "Enable code sanitizers" FALSE)

# Code coverage
if(ENABLE_CODE_COVERAGE)
  add_compile_options($<$<CONFIG:DEBUG>:--coverage>)
endif()

# Code guidelines
if(ENABLE_CODE_GUIDELINES)
  set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-header-filter=.*;-checks=*,-llvmlibc-*,-modernize-use-nodiscard,-modernize-use-trailing-return-type")
endif()

# Code sanitizers
if(ENABLE_CODE_SANITIZERS)
  add_compile_options($<$<CONFIG:DEBUG>:-fsanitize=address,dataflow,leak,memory,thread,undefined>)  # TODO: Set enabled sanitizers ?
endif()
