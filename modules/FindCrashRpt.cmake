if(TARGET CrashRpt)
    return()
endif()

set(CrashRpt_ROOT_DIR "${CSTOOLKIT_EXTERNALS}/CrashRpt")
set(CrashRpt_LIBRARIES "CrashRpt")

add_library(CrashRpt SHARED IMPORTED)

set_target_properties(CrashRpt PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${CrashRpt_ROOT_DIR}/include"
)

set_target_properties(CrashRpt PROPERTIES
    IMPORTED_LINK_INTERFACE_LANGUAGES "CXX"
    IMPORTED_IMPLIB "${CrashRpt_ROOT_DIR}/win-msvc2015-${CSTOOLKIT_HOST_ARCH}/CrashRpt.lib"
    IMPORTED_LOCATION "${CrashRpt_ROOT_DIR}/win-msvc2015-${CSTOOLKIT_HOST_ARCH}/CrashRpt.dll"
)

set(CrashRpt_RUNTIME_DLLS
    "${CrashRpt_ROOT_DIR}/win-msvc2015-${CSTOOLKIT_HOST_ARCH}/dbghelp.dll"
    "${CrashRpt_ROOT_DIR}/win-msvc2015-${CSTOOLKIT_HOST_ARCH}/CrashSaver.exe"
)

# handle the QUIETLY and REQUIRED arguments and set xxx_FOUND to TRUE if
# all listed variables are TRUE
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(CrashRpt DEFAULT_MSG
    CrashRpt_ROOT_DIR CrashRpt_LIBRARIES CrashRpt_RUNTIME_DLLS)
