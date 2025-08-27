#FindVegaPrime
if(TARGET VegaPrime::VegaPrime)
    return()
endif()

file(TO_CMAKE_PATH "$ENV{PRESAGIS_VEGA_PRIME_22_0}" PRESAGIS_VEGA_PRIME_22_0)
file(TO_CMAKE_PATH "$ENV{PRESAGIS_VEGA_PRIME_LIB_22_0}" PRESAGIS_VEGA_PRIME_LIB_22_0)
file(TO_CMAKE_PATH "$ENV{PRESAGIS_VEGA_PRIME_BIN_22_0}" PRESAGIS_VEGA_PRIME_BIN_22_0)

set(VegaPrime_ROOT_DIR "${PRESAGIS_VEGA_PRIME_22_0}")
set(VegaPrime_LIB_DIR "${PRESAGIS_VEGA_PRIME_LIB_22_0}")
set(VegaPrime_BIN_DIR "${PRESAGIS_VEGA_PRIME_BIN_22_0}")
set(VegaPrime_VERSION "22.0")
set(VegaPrime_LIBRARIES "VegaPrime::VegaPrime" "VegaPrime::SimDIS" "VegaPrime::SimUtils" "VegaPrime::vsgu" "VegaPrime::vuMemoryProfiler" "VegaPrime::vsgr" "VegaPrime::vsgs" "VegaPrime::vp" "VegaPrime::vsgn_builder" "VegaPrime::vpcoordsys")

add_library(VegaPrime::VegaPrime INTERFACE IMPORTED)
add_library(VegaPrime::SimDIS SHARED IMPORTED)
add_library(VegaPrime::SimUtils SHARED IMPORTED)

add_library(VegaPrime::vsgu SHARED IMPORTED)
add_library(VegaPrime::vuMemoryProfiler SHARED IMPORTED)
add_library(VegaPrime::vsgr SHARED IMPORTED)
add_library(VegaPrime::vsgs SHARED IMPORTED)
add_library(VegaPrime::vp SHARED IMPORTED)
add_library(VegaPrime::vsgn_builder SHARED IMPORTED)
add_library(VegaPrime::vpcoordsys SHARED IMPORTED)

set_target_properties(VegaPrime::VegaPrime PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${VegaPrime_ROOT_DIR}/include/vegaprime;${VegaPrime_ROOT_DIR}/include/vsg;${VegaPrime_ROOT_DIR}/include/ms_utils;${VegaPrime_ROOT_DIR}/include/cigi_ccl"
    INTERFACE_LINK_DIRECTORIES "${VegaPrime_LIB_DIR}"
    IMPORTED_LINK_INTERFACE_LANGUAGES "CXX"
)

set_target_properties(VegaPrime::SimDIS PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${VegaPrime_ROOT_DIR}/include/vegaprime"
    IMPORTED_LINK_INTERFACE_LANGUAGES "CXX"
    IMPORTED_IMPLIB_RELEASE "${VegaPrime_LIB_DIR}/SimDIS22.lib"
    IMPORTED_LOCATION_RELEASE "${VegaPrime_BIN_DIR}/SimDIS22.dll"
    COMPILE_PDB_NAME_RELEASE "${VegaPrime_BIN_DIR}/SimDIS22.pdb"
    IMPORTED_IMPLIB_DEBUG "${VegaPrime_LIB_DIR}/SimDIS22d.lib"
    IMPORTED_LOCATION_DEBUG "${VegaPrime_BIN_DIR}/SimDIS22d.dll"
    COMPILE_PDB_NAME_DEBUG "${VegaPrime_BIN_DIR}/SimDIS22d.pdb"
)

set_target_properties(VegaPrime::SimUtils PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${VegaPrime_ROOT_DIR}/include/vegaprime"
    IMPORTED_LINK_INTERFACE_LANGUAGES "CXX"
    IMPORTED_IMPLIB_RELEASE "${VegaPrime_LIB_DIR}/SimUtils22.lib"
    IMPORTED_LOCATION_RELEASE "${VegaPrime_BIN_DIR}/SimUtils22.dll"
    COMPILE_PDB_NAME_RELEASE "${VegaPrime_BIN_DIR}/SimUtils22.pdb"
    IMPORTED_IMPLIB_DEBUG "${VegaPrime_LIB_DIR}/SimUtils22d.lib"
    IMPORTED_LOCATION_DEBUG "${VegaPrime_BIN_DIR}/SimUtils22d.dll"
    COMPILE_PDB_NAME_DEBUG "${VegaPrime_BIN_DIR}/SimUtils22d.pdb"
)

set_target_properties(VegaPrime::vsgu PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${VegaPrime_ROOT_DIR}/include/vsg"
    IMPORTED_LINK_INTERFACE_LANGUAGES "CXX"
    IMPORTED_IMPLIB_RELEASE "${VegaPrime_LIB_DIR}/vsgu22_0.lib"
    IMPORTED_LOCATION_RELEASE "${VegaPrime_BIN_DIR}/vsgu22_0.dll"
    COMPILE_PDB_NAME_RELEASE "${VegaPrime_BIN_DIR}/vsgu22_0.pdb"
    IMPORTED_IMPLIB_DEBUG "${VegaPrime_LIB_DIR}/vsgu22_0D.lib"
    IMPORTED_LOCATION_DEBUG "${VegaPrime_BIN_DIR}/vsgu22_0D.dll"
    COMPILE_PDB_NAME_DEBUG "${VegaPrime_BIN_DIR}/vsgu22_0D.pdb"
)

set_target_properties(VegaPrime::vuMemoryProfiler PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${VegaPrime_ROOT_DIR}/include/vsg"
    IMPORTED_LINK_INTERFACE_LANGUAGES "CXX"
    IMPORTED_IMPLIB_RELEASE "${VegaPrime_LIB_DIR}/vuMemoryProfiler22_0.lib"
    IMPORTED_LOCATION_RELEASE "${VegaPrime_BIN_DIR}/vuMemoryProfiler22_0.dll"
    COMPILE_PDB_NAME_RELEASE "${VegaPrime_BIN_DIR}/vuMemoryProfiler22_0.pdb"
    IMPORTED_IMPLIB_DEBUG "${VegaPrime_LIB_DIR}/vuMemoryProfiler22_0D.lib"
    IMPORTED_LOCATION_DEBUG "${VegaPrime_BIN_DIR}/vuMemoryProfiler22_0D.dll"
    COMPILE_PDB_NAME_DEBUG "${VegaPrime_BIN_DIR}/vuMemoryProfiler22_0D.pdb"
)

set_target_properties(VegaPrime::vsgr PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${VegaPrime_ROOT_DIR}/include/vsg"
    IMPORTED_LINK_INTERFACE_LANGUAGES "CXX"
    IMPORTED_IMPLIB_RELEASE "${VegaPrime_LIB_DIR}/vsgr22_0.lib"
    IMPORTED_LOCATION_RELEASE "${VegaPrime_BIN_DIR}/vsgr22_0.dll"
    COMPILE_PDB_NAME_RELEASE "${VegaPrime_BIN_DIR}/vsgr22_0.pdb"
    IMPORTED_IMPLIB_DEBUG "${VegaPrime_LIB_DIR}/vsgr22_0D.lib"
    IMPORTED_LOCATION_DEBUG "${VegaPrime_BIN_DIR}/vsgr22_0D.dll"
    COMPILE_PDB_NAME_DEBUG "${VegaPrime_BIN_DIR}/vsgr22_0D.pdb"
)

set_target_properties(VegaPrime::vsgs PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${VegaPrime_ROOT_DIR}/include/vsg"
    IMPORTED_LINK_INTERFACE_LANGUAGES "CXX"
    IMPORTED_IMPLIB_RELEASE "${VegaPrime_LIB_DIR}/vsgs22_0.lib"
    IMPORTED_LOCATION_RELEASE "${VegaPrime_BIN_DIR}/vsgs22_0.dll"
    COMPILE_PDB_NAME_RELEASE "${VegaPrime_BIN_DIR}/vsgs22_0.pdb"
    IMPORTED_IMPLIB_DEBUG "${VegaPrime_LIB_DIR}/vsgs22_0D.lib"
    IMPORTED_LOCATION_DEBUG "${VegaPrime_BIN_DIR}/vsgs22_0D.dll"
    COMPILE_PDB_NAME_DEBUG "${VegaPrime_BIN_DIR}/vsgs22_0D.pdb"
)

set_target_properties(VegaPrime::vp PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${VegaPrime_ROOT_DIR}/include/vegaprime"
    IMPORTED_LINK_INTERFACE_LANGUAGES "CXX"
    IMPORTED_IMPLIB_RELEASE "${VegaPrime_LIB_DIR}/vp22_0.lib"
    IMPORTED_LOCATION_RELEASE "${VegaPrime_BIN_DIR}/vp22_0.dll"
    COMPILE_PDB_NAME_RELEASE "${VegaPrime_BIN_DIR}/vp22_0.pdb"
    IMPORTED_IMPLIB_DEBUG "${VegaPrime_LIB_DIR}/vp22_0D.lib"
    IMPORTED_LOCATION_DEBUG "${VegaPrime_BIN_DIR}/vp22_0D.dll"
    COMPILE_PDB_NAME_DEBUG "${VegaPrime_BIN_DIR}/vp22_0D.pdb"
)

set_target_properties(VegaPrime::vsgn_builder PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${VegaPrime_ROOT_DIR}/include/vsg"
    IMPORTED_LINK_INTERFACE_LANGUAGES "CXX"
    IMPORTED_IMPLIB_RELEASE "${VegaPrime_LIB_DIR}/vsgn_builder22_0.lib"
    IMPORTED_LOCATION_RELEASE "${VegaPrime_BIN_DIR}/vsgn_builder22_0.dll"
    COMPILE_PDB_NAME_RELEASE "${VegaPrime_BIN_DIR}/vsgn_builder22_0.pdb"
    IMPORTED_IMPLIB_DEBUG "${VegaPrime_LIB_DIR}/vsgn_builder22_0D.lib"
    IMPORTED_LOCATION_DEBUG "${VegaPrime_BIN_DIR}/vsgn_builder22_0D.dll"
    COMPILE_PDB_NAME_DEBUG "${VegaPrime_BIN_DIR}/vsgn_builder22_0D.pdb"
)

set_target_properties(VegaPrime::vpcoordsys PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${VegaPrime_ROOT_DIR}/include/vegaprime"
    IMPORTED_LINK_INTERFACE_LANGUAGES "CXX"
    IMPORTED_IMPLIB_RELEASE "${VegaPrime_LIB_DIR}/vpcoordsys22_0.lib"
    IMPORTED_LOCATION_RELEASE "${VegaPrime_BIN_DIR}/vpcoordsys22_0.dll"
    COMPILE_PDB_NAME_RELEASE "${VegaPrime_BIN_DIR}/vpcoordsys22_0.pdb"
    IMPORTED_IMPLIB_DEBUG "${VegaPrime_LIB_DIR}/vpcoordsys22_0D.lib"
    IMPORTED_LOCATION_DEBUG "${VegaPrime_BIN_DIR}/vpcoordsys22_0D.dll"
    COMPILE_PDB_NAME_DEBUG "${VegaPrime_BIN_DIR}/vpcoordsys22_0D.pdb"
)

# handle the QUIETLY and REQUIRED arguments and set xxx_FOUND to TRUE if
# all listed variables are TRUE
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(VegaPrime DEFAULT_MSG
    VegaPrime_ROOT_DIR
    VegaPrime_LIBRARIES
)
