#FindOpenFlightAPI
if(TARGET OpenFlightAPI::fltdata)
    return()
endif()

file(TO_CMAKE_PATH "$ENV{PRESAGIS_OPENFLIGHT_API_22_0}" PRESAGIS_OPENFLIGHT_API_22_0)

set(OpenFlightAPI_ROOT_DIR "${PRESAGIS_OPENFLIGHT_API_22_0}")
set(OpenFlightAPI_VERSION "22.0")
set(OpenFlightAPI_LIBRARIES "OpenFlightAPI::fltdata" "OpenFlightAPI::mgapilib" "OpenFlightAPI::mgdd")

add_library(OpenFlightAPI::fltdata SHARED IMPORTED)
add_library(OpenFlightAPI::mgapilib SHARED IMPORTED)
add_library(OpenFlightAPI::mgdd SHARED IMPORTED)

if (${CSTOOLKIT_HOST_ARCH} STREQUAL "x64")
    set(OpenFlightAPI_LIB_DIR "lib_x64")
    set(OpenFlightAPI_BIN_DIR "bin_x64")
else()
    set(OpenFlightAPI_LIB_DIR "lib")
    set(OpenFlightAPI_BIN_DIR "bin")
endif()

set_target_properties(OpenFlightAPI::fltdata PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${OpenFlightAPI_ROOT_DIR}/include"
    IMPORTED_LINK_INTERFACE_LANGUAGES "CXX"
    IMPORTED_IMPLIB_DEBUG "${OpenFlightAPI_ROOT_DIR}/${OpenFlightAPI_LIB_DIR}/debug/fltdata.lib"
    IMPORTED_LOCATION_DEBUG "${OpenFlightAPI_ROOT_DIR}/${OpenFlightAPI_BIN_DIR}/debug/fltdata.dll"
    COMPILE_PDB_NAME_DEBUG "${OpenFlightAPI_ROOT_DIR}/${OpenFlightAPI_BIN_DIR}/debug/fltdata.pdb"
    IMPORTED_IMPLIB_RELEASE "${OpenFlightAPI_ROOT_DIR}/${OpenFlightAPI_LIB_DIR}/release/fltdata.lib"
    IMPORTED_LOCATION_RELEASE "${OpenFlightAPI_ROOT_DIR}/${OpenFlightAPI_BIN_DIR}/release/fltdata.dll"
)

set_target_properties(OpenFlightAPI::mgapilib PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${OpenFlightAPI_ROOT_DIR}/include"
    IMPORTED_LINK_INTERFACE_LANGUAGES "CXX"
    IMPORTED_IMPLIB_DEBUG "${OpenFlightAPI_ROOT_DIR}/${OpenFlightAPI_LIB_DIR}/debug/mgapilib.lib"
    IMPORTED_LOCATION_DEBUG "${OpenFlightAPI_ROOT_DIR}/${OpenFlightAPI_BIN_DIR}/debug/mgapilib.dll"
    COMPILE_PDB_NAME_DEBUG "${OpenFlightAPI_ROOT_DIR}/${OpenFlightAPI_BIN_DIR}/debug/mgapilib.pdb"
    IMPORTED_IMPLIB_RELEASE "${OpenFlightAPI_ROOT_DIR}/${OpenFlightAPI_LIB_DIR}/release/mgapilib.lib"
    IMPORTED_LOCATION_RELEASE "${OpenFlightAPI_ROOT_DIR}/${OpenFlightAPI_BIN_DIR}/release/mgapilib.dll"
)

set_target_properties(OpenFlightAPI::mgdd PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${OpenFlightAPI_ROOT_DIR}/include"
    IMPORTED_LINK_INTERFACE_LANGUAGES "CXX"
    IMPORTED_IMPLIB_DEBUG "${OpenFlightAPI_ROOT_DIR}/${OpenFlightAPI_LIB_DIR}/debug/mgdd.lib"
    IMPORTED_LOCATION_DEBUG "${OpenFlightAPI_ROOT_DIR}/${OpenFlightAPI_BIN_DIR}/debug/mgdd.dll"
    COMPILE_PDB_NAME_DEBUG "${OpenFlightAPI_ROOT_DIR}/${OpenFlightAPI_BIN_DIR}/debug/mgdd.pdb"
    IMPORTED_IMPLIB_RELEASE "${OpenFlightAPI_ROOT_DIR}/${OpenFlightAPI_LIB_DIR}/release/mgdd.lib"
    IMPORTED_LOCATION_RELEASE "${OpenFlightAPI_ROOT_DIR}/${OpenFlightAPI_BIN_DIR}/release/mgdd.dll"
)

set(OpenFlightAPI_3ds_PLUGIN "${OpenFlightAPI_ROOT_DIR}/${OpenFlightAPI_BIN_DIR}/$<LOWER_CASE:$<CONFIG>>/3ds/3ds.dll")
set(OpenFlightAPI_fbx_PLUGIN "${OpenFlightAPI_ROOT_DIR}/${OpenFlightAPI_BIN_DIR}/$<LOWER_CASE:$<CONFIG>>/fbx/fbx.dll")
set(OpenFlightAPI_kml_PLUGIN "${OpenFlightAPI_ROOT_DIR}/${OpenFlightAPI_BIN_DIR}/$<LOWER_CASE:$<CONFIG>>/kml/kml.dll")
set(OpenFlightAPI_stl_PLUGIN "${OpenFlightAPI_ROOT_DIR}/${OpenFlightAPI_BIN_DIR}/$<LOWER_CASE:$<CONFIG>>/stl/stl.dll")
set(OpenFlightAPI_wave_PLUGIN "${OpenFlightAPI_ROOT_DIR}/${OpenFlightAPI_BIN_DIR}/$<LOWER_CASE:$<CONFIG>>/wave/wave.dll")
set(OpenFlightAPI_xplane_PLUGIN "${OpenFlightAPI_ROOT_DIR}/${OpenFlightAPI_BIN_DIR}/$<LOWER_CASE:$<CONFIG>>/xplane/xplane.dll")

# handle the QUIETLY and REQUIRED arguments and set xxx_FOUND to TRUE if
# all listed variables are TRUE
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(OpenFlightAPI DEFAULT_MSG
    OpenFlightAPI_ROOT_DIR
    OpenFlightAPI_LIBRARIES
    OpenFlightAPI_3ds_PLUGIN
    OpenFlightAPI_fbx_PLUGIN
    OpenFlightAPI_kml_PLUGIN
    OpenFlightAPI_stl_PLUGIN
    OpenFlightAPI_wave_PLUGIN
    OpenFlightAPI_xplane_PLUGIN
)
