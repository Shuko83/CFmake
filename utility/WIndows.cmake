function(cstoolkit_get_windowssdk_library_dir _var)
    string(REGEX MATCH "^([0-9]+\\.[0-9]+)" _winsdk_ver ${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION})
    
    set(_winsdk_key "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Microsoft SDKs\\Windows\\v${_winsdk_ver}")

    cmake_host_system_information(RESULT _winsdk_installfolder QUERY WINDOWS_REGISTRY "${_winsdk_key}" VALUE "InstallationFolder")

    if(_winsdk_ver STREQUAL "8.1")
        set(_winsdk_subfolder "winv6.3")
    else()
        set(_winsdk_subfolder "${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION}")
    endif()

    if(CMAKE_SIZEOF_VOID_P MATCHES "8")
        set(_winsdk_arch x64)
    else()
        set(_winsdk_arch x86)
    endif()

    cmake_path(SET _winsdk_lib_dir NORMALIZE "${_winsdk_installfolder}\\Lib\\${_winsdk_subfolder}\\um\\${_winsdk_arch}")

    # Check to see if a library actually exists here.
    file(GLOB _libs "${_winsdk_lib_dir}/*.lib")
    if(NOT _libs)
        set(_winsdk_lib_dir NOTFOUND)
    endif()

    set(${_var} ${_winsdk_lib_dir} PARENT_SCOPE)
endfunction()

if(WIN32)
    cstoolkit_get_windowssdk_library_dir(CSTOOLKIT_WINDOWS_KITS_LIB_DIR)
    if(NOT CSTOOLKIT_WINDOWS_KITS_LIB_DIR)
        unset(CSTOOLKIT_WINDOWS_KITS_LIB_DIR)
    endif()
endif()
