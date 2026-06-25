if(TARGET libVLC::vlc)
    return()
endif()

set(VLC_ROOT_DIR "${CFMAKE_EXTERNALS}/libVLC/")
set(VLC_LIBRARIES "libVLC::vlc" "libVLC::vlccore")

add_library(libVLC::vlccore MODULE IMPORTED)
add_library(libVLC::vlc SHARED IMPORTED)

set_target_properties(libVLC::vlccore PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${VLC_ROOT_DIR}/include/"
    IMPORTED_LINK_INTERFACE_LANGUAGES "CXX"
    IMPORTED_LOCATION "${VLC_ROOT_DIR}/bin/x64/libvlccore.dll"
)

set_target_properties(libVLC::vlc PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${VLC_ROOT_DIR}/include/"
    IMPORTED_LINK_INTERFACE_LANGUAGES "CXX"
    IMPORTED_LOCATION "${VLC_ROOT_DIR}/bin/x64/libvlc.dll"
    IMPORTED_IMPLIB "${VLC_ROOT_DIR}/lib/x64/libvlc.lib"
)

# handle the QUIETLY and REQUIRED arguments and set xxx_FOUND to TRUE if
# all listed variables are TRUE
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(libVLC DEFAULT_MSG
    VLC_ROOT_DIR
    VLC_LIBRARIES
)
