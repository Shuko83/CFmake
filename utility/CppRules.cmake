if(MSVC)        
    add_compile_options(-MP)                                                       # enables multi-processors compilation
    add_compile_definitions(UNICODE)
    add_compile_definitions(_UNICODE)
#[[
    msvc-proj: QMAKE_CXXFLAGS_EXCEPTIONS_OFF *= -EH                             # defines no exception handling model (if exceptions support is deactivated)
    QMAKE_CXXFLAGS_STL_ON = $$remove( QMAKE_CXXFLAGS_STL_ON, ^[-|/]EH.* )       # removes exception handling model stored in STL flags

    QMAKE_CXXFLAGS_WARN_ON = -W4                                                # enables level 4 compiler warnings (if warn_on option is set)

    ## linker flags

    QMAKE_LFLAGS *= /INCREMENTAL:NO                                             # disables incremental link

    QMAKE_CXXFLAGS_RELEASE -= -Zc:strictStrings                 # enables string literal type conversion in release and profile modes
    QMAKE_CXXFLAGS_RELEASE_WITH_DEBUGINFO -= -Zc:strictStrings
    ]]
endif()

#[[
win32 {
  DEFINES *= _WINDOWS _WIN32_PLATEFORM_              # custom Windows platform preprocessor macros
  DEFINES *= _WIN32                                                   # standard Windows 32/64 bits platform preprocessor macro
  equals( TOOLKIT.host.arch, x64 ): DEFINES *= _WIN64                 # standard Windows 64 bits platform preprocessor macro

  contains( DEFINES, _MBCS ): DEFINES -= UNICODE _UNICODE             # enables multi-byte characters set if predefined
  else:                       DEFINES *= UNICODE _UNICODE             # otherwise enables Unicode characters set by default

  DEFINES *= _USE_MATH_DEFINES                                        # enables mathematical constants defined in C/C++ math headers
  DEFINES *= WIN32_LEAN_AND_MEAN                                      # excludes APIs such as Cryptography, DDE, RPC, Shell, and Windows Sockets
}

else: linux: DEFINES *= __linux__                                       # standard Linux platform preprocessor macro
else: linux: DEFINES *= _LINUX_PLATEFORM_                       # custom Linux platform preprocessor macro




add_compile_optionsdebug(_DEBUG)


Reldebuginfo
<InlineFunctionExpansion>OnlyExplicitInline</InlineFunctionExpansion> instead of AnySuitable

#add_definitions(-D_UNICODE)
debug
compile options -Zc:rvalueCast -Zc:inline -Zc:strictStrings -Zc:throwingNew 
<BasicRuntimeChecks>StackFrameRuntimeCheck</BasicRuntimeChecks> diff
<BrowseInformation>false</BrowseInformation>
warninglevel 4
]]
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

#add_compile_definitions(UNICODE)
#add_compile_definitions(_UNICODE)

message("flags: ${CMAKE_CXX_FLAGS}")
message("flagsD: ${CMAKE_CXX_FLAGS_DEBUG}")
message("flagsR: ${CMAKE_CXX_FLAGS_RELEASE}")

# MSVC configuration
if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
  message(MSVC)
  #set(CMAKE_GENERATOR_PLATFORM "x64" CACHE INTERNAL "Target architecture")
  #add_compile_options(/MP /W4 /wd4251 /wd4373)
  #add_compile_definitions(NOMINMAX WIN32_LEAN_AND_MEAN)
  if(ENABLE_PDB_RELEASE)
    #add_compile_options($<$<CONFIG:RELEASE>:/Zi>)
    #add_link_options($<$<CONFIG:RELEASE>:/DEBUG>)
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
