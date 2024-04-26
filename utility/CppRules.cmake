################################################################################
# Compilation definition
################################################################################

# Common configuration
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_DEBUG_POSTFIX d)
# Allow common behavior between windows and linux
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

#message("flags: ${CMAKE_CXX_FLAGS}")
#message("flagsD: ${CMAKE_CXX_FLAGS_DEBUG}")
#message("flagsR: ${CMAKE_CXX_FLAGS_RELEASE}")

################################################################################
# System specific
################################################################################

if(WIN32)
    add_compile_definitions(_WIN32_PLATEFORM_)       # custom Windows platform preprocessor macros

    add_compile_definitions(UNICODE _UNICODE)        # enables Unicode characters set by default

    add_compile_definitions(_WIN32)                  # standard Windows 32/64 bits platform preprocessor macro
    if (CMAKE_SIZEOF_VOID_P EQUAL 8)
        add_compile_definitions(WIN64 _WIN64)        # standard Windows 64 bits platform preprocessor macro
    endif()

    add_compile_definitions(_USE_MATH_DEFINES)       # enables mathematical constants defined in C/C++ math headers
    add_compile_definitions(WIN32_LEAN_AND_MEAN)     # excludes APIs such as Cryptography, DDE, RPC, Shell, and Windows Sockets
    #add_compile_definitions(NOMINMAX)                # This will prevent the definition of the min and max Windows-specific preprocessor macros.
endif()

if(LINUX)
    add_compile_definitions(__linux__)       # standard Linux platform preprocessor macro

    add_compile_definitions(_LINUX_PLATEFORM_)        # custom Linux platform preprocessor macro
endif()

################################################################################
# Compiler specific
################################################################################

# MSVC configuration
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_options(-MP)              # enables multi-processors compilation

    add_compile_options(-W4)              # enables level 4 compiler warnings

    add_link_options(/INCREMENTAL:NO)     # disables incremental link

    add_compile_options(-Zc:strictStrings) # enables string literal type conversion in release and profile modes
    add_compile_options(-Zc:inline)
    add_compile_options(-Zc:throwingNew)
    add_compile_options(-Zc:rvalueCast)

    set(CMAKE_CONFIGURATION_TYPES "Debug;Release")

    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Zi") # Force debug info
endif()

# GCC configuration
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-Wall -Wextra -Wconversion -Wsign-conversion -Wold-style-cast)  # TODO: Add other warnings ?
    add_link_options($<$<CONFIG:RELEASE>:-Wl,-strip-all>)
endif()

# Clang configuration
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_compile_options(-Weverything -Wno-padded -Wno-c++98-compat -Wno-c++98-compat-pedantic)
    add_link_options($<$<CONFIG:RELEASE>:-Wl,-strip-all>)
endif()
