function(cstoolkit_get_windowssdk_library_dir _var)
    if(CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION)
        string(REGEX MATCH "^([0-9]+\\.[0-9]+)" _winsdk_ver ${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION})
    else() # CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION is only defined for Win10 and up
        set(_winsdk_ver "8.1")
    endif()
    
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

macro(cstoolkit_windows_sdk_raise_warning)
message(WARNING "CSToolkit: Windows SDK version ${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION} is not officially compatible with Visual Studio ${CSTOOLKIT_COMPILER_VERSION} (MSVC v${MSVC_TOOLSET_VERSION} toolset)
The latest officially compatible version is ${OFFICIAL_VS_WINDOWS_TARGET_PLATFORM_VERSION}
Install Windows SDK and set CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION_MAXIMUM before call to project or set CSTOOLKIT_WINDOWS_SDK_WARNING=OFF to remove this warning")
endmacro()

if(WIN32)
    cstoolkit_get_windowssdk_library_dir(CSTOOLKIT_WINDOWS_KITS_LIB_DIR)
    if(NOT CSTOOLKIT_WINDOWS_KITS_LIB_DIR)
        unset(CSTOOLKIT_WINDOWS_KITS_LIB_DIR)
    endif()

    if(CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION)
        if(MSVC_VERSION EQUAL 1900) # 2015
            set(OFFICIAL_VS_WINDOWS_TARGET_PLATFORM_VERSION "10.0.14393.795")
        elseif(MSVC_VERSION GREATER_EQUAL 1910 AND MSVC_VERSION LESS 1920) # 2017
            set(OFFICIAL_VS_WINDOWS_TARGET_PLATFORM_VERSION "10.0.17763.132")
        elseif(MSVC_VERSION GREATER_EQUAL 1920 AND MSVC_VERSION LESS 1930) # 2019
            set(OFFICIAL_VS_WINDOWS_TARGET_PLATFORM_VERSION "10.0.19041.685")
        else()
            set(OFFICIAL_VS_WINDOWS_TARGET_PLATFORM_VERSION "${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION}")
        endif()
    endif()
    if(CSTOOLKIT_WINDOWS_SDK_WARNING)
        if(CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION)
            if(CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION VERSION_GREATER OFFICIAL_VS_WINDOWS_TARGET_PLATFORM_VERSION)
message(WARNING "CSToolkit: Detected Windows SDK version ${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION} is not officially compatible with Visual Studio ${CSTOOLKIT_COMPILER_VERSION} (MSVC toolset v${MSVC_TOOLSET_VERSION})
The latest officially compatible version is: ${OFFICIAL_VS_WINDOWS_TARGET_PLATFORM_VERSION}
  To resolve this warning:
    - Install compatible Windows SDK, and
    - Set CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION_MAXIMUM to ${OFFICIAL_VS_WINDOWS_TARGET_PLATFORM_VERSION} before the 'project()' call
      or
    - Set CSTOOLKIT_WINDOWS_SDK_WARNING to OFF to suppress this warning
")
            endif()
        else()
            message(WARNING "CSToolkit: No Windows SDK found
Set CSTOOLKIT_WINDOWS_SDK_WARNING to OFF to suppress this warning
")
        endif()
    endif()

    # Find VCRedist Installer
    if(MSVC)
        if(CMAKE_GENERATOR_INSTANCE)
            if(IS_DIRECTORY "${CMAKE_GENERATOR_INSTANCE}/VC")
                set(VCINSTALLDIR "${CMAKE_GENERATOR_INSTANCE}/VC")
                file(GLOB _VCToolsRedistDir_subdirs RELATIVE "${VCINSTALLDIR}/Redist/MSVC" "${VCINSTALLDIR}/Redist/MSVC/*")
                foreach(_redist_subdir ${_VCToolsRedistDir_subdirs})
                    if(IS_DIRECTORY "${VCINSTALLDIR}/Redist/MSVC/${_redist_subdir}/debug_nonredist")
                        set(VCToolsRedistDir "${VCINSTALLDIR}/Redist/MSVC/${_redist_subdir}")
                    endif()
                endforeach()
                unset(_VCToolsRedistDir_subdirs)
                if(VCToolsRedistDir)
                    if(EXISTS "${VCToolsRedistDir}/vc_redist.${CSTOOLKIT_HOST_ARCH}.exe")
                        set(CSTOOLKIT_VCREDIST "${VCToolsRedistDir}/vc_redist.${CSTOOLKIT_HOST_ARCH}.exe")
                    endif()
                endif()
            endif()
        elseif(MSVC_VERSION EQUAL 1900)
            cmake_host_system_information(RESULT VCINSTALLDIR QUERY WINDOWS_REGISTRY "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\VisualStudio\\SxS\\VC7" VALUE "14.0")
            cmake_path(SET VCINSTALLDIR NORMALIZE "${VCINSTALLDIR}")
            string(REGEX REPLACE "/$" "" VCINSTALLDIR "${VCINSTALLDIR}")
            if(IS_DIRECTORY "${VCINSTALLDIR}/redist")
                set(VCToolsRedistDir "${VCINSTALLDIR}/redist")
                if(EXISTS "${VCToolsRedistDir}/1033/vcredist_${CSTOOLKIT_HOST_ARCH}.exe")
                    set(CSTOOLKIT_VCREDIST "${VCToolsRedistDir}/1033/vcredist_${CSTOOLKIT_HOST_ARCH}.exe")
                endif()
            endif()
        endif()
    endif()
endif()
