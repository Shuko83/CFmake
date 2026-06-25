#Findccl
if(TARGET ccl::ccl)
    return()
endif()

set(ccl_ROOT_DIR "${CFMAKE_EXTERNALS}/ccl")
set(ccl_LIBRARIES "ccl::ccl" "ccl::ccl_STATIC")

add_library(ccl::ccl SHARED IMPORTED)
add_library(ccl::ccl_STATIC STATIC IMPORTED)

set_target_properties(ccl::ccl PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${ccl_ROOT_DIR}/include/cigicl"
    IMPORTED_LINK_INTERFACE_LANGUAGES "CXX"
    IMPORTED_IMPLIB "${ccl_ROOT_DIR}/lib/cigicl.lib"
    IMPORTED_LOCATION "${ccl_ROOT_DIR}/bin/cigicl.dll"
)

set_target_properties(ccl::ccl_STATIC PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${ccl_ROOT_DIR}/include/cigicl"
    IMPORTED_LINK_INTERFACE_LANGUAGES "CXX"
    IMPORTED_LOCATION "${ccl_ROOT_DIR}/lib/cigicl_static.lib"
)

# handle the QUIETLY and REQUIRED arguments and set xxx_FOUND to TRUE if
# all listed variables are TRUE
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(ccl DEFAULT_MSG
    ccl_ROOT_DIR
    ccl_LIBRARIES
)
