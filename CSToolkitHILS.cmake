set(CSTOOLKIT_USE_GIT_TAG_VERSION ON)
set(CSTOOLKIT_CPACK_RULES ON)
set(CSTOOLKIT_AUTO_DEPLOY_QT OFF)

set(CSTOOLKIT_DEFAULT_PUBLIC_HEADERS_DIRS "inc")
set(CSTOOLKIT_DEFAULT_PRIVATE_HEADERS_DIRS "inc")
set(CSTOOLKIT_DEFAULT_SOURCES_DIRS "src")
set(CSTOOLKIT_DEFAULT_UI_DIRS "forms")
set(CSTOOLKIT_DEFAULT_RESOURCES_DIRS "resources")

if(IS_DIRECTORY "${CMAKE_SOURCE_DIR}/externals")
    message(STATUS "Externals installed in Source directory")
    set(_externals_installed ON)
else()
    set(_externals_installed OFF)
endif()

if (_externals_installed)
    set(CSTOOLKIT_EXTERNALS "${CMAKE_SOURCE_DIR}/externals")
endif()

### CMake variables
set(CMAKE_WIN32_EXECUTABLE TRUE)

### HILS custom functions
macro(hils_find_package PACKAGE_NAME FETCH_MODE)
    if(PACKAGE_NAME STREQUAL "")
        message(SEND_ERROR "CSToolkit: find_package(): No NAME defined for target")
        return()
    endif()

    # If the option to search package in artifactory is set
    if (NOT _externals_installed)
        if (NOT ${FETCH_MODE} STREQUAL "FETCH_ARTIFACTORY" AND NOT ${FETCH_MODE} STREQUAL "FETCH_PACKAGE" AND NOT ${FETCH_MODE} STREQUAL "FETCH_NEXUS")
            message(SEND_ERROR "CSToolkit: find_package(${PACKAGE_NAME}): invalid param FETCH_MODE: ${FETCH_MODE}")
            return()
        endif()

        if (${FETCH_MODE} STREQUAL "FETCH_ARTIFACTORY")
            cstoolkit_fetch_artifactory(${PACKAGE_NAME} "${ARGN}")
        elseif(${FETCH_MODE} STREQUAL "FETCH_NEXUS")
            cstoolkit_fetch_nexus(${PACKAGE_NAME} "${ARGN}")
        else()
            cstoolkit_fetch_package(${PACKAGE_NAME} "${ARGN}")
        endif()
    # Otherwise, look for externals in <project_root_dir>/externals with find_package
    else()
        find_package(${PACKAGE_NAME} QUIET)
    endif()
endmacro(hils_find_package)

function(hils_install_target_artefacts)
    # Get the number of arguments passed
    set(options INSTALL_LINKER_FILE INSTALL_PDB_FILE)
    set(oneValueArgs DESTINATION)
    set(multiValueArgs TARGETS)

    cmake_parse_arguments(PARSE_ARGV 0 arg "${options}" "${oneValueArgs}" "${multiValueArgs}")

    if(DEFINED arg_UNPARSED_ARGUMENTS)
        message(SEND_ERROR "CSToolkitHILS: hils_install_target_artefacts(): Unkown arguments \"${arg_UNPARSED_ARGUMENTS}\"")
        return()
    endif()

    foreach(TARGET IN LISTS arg_TARGETS)
        get_target_property(target_type ${TARGET} TYPE)

        if (target_type STREQUAL "INTERFACE_LIBRARY")
            # Get the list of libraries the interface target links, and add them to the list of targets to install
            get_target_property(linked_targets ${TARGET} INTERFACE_LINK_LIBRARIES)

            if(linked_targets)
                hils_install_target_artefacts(TARGETS ${linked_targets} DESTINATION ${arg_DESTINATION})
            endif()
        else()
            install(FILES
                $<TARGET_FILE:${TARGET}>
                DESTINATION ${arg_DESTINATION}
                CONFIGURATIONS Release Debug
            )

            if (target_type STREQUAL "SHARED_LIBRARY" AND arg_INSTALL_LINKER_FILE)
                install(FILES
                    $<TARGET_LINKER_FILE:${TARGET}>
                    DESTINATION ${arg_DESTINATION}
                    CONFIGURATIONS Release Debug
                )

                get_target_property(is_imported ${TARGET} IMPORTED)

                if(NOT is_imported AND arg_INSTALL_PDB_FILE)
                    install(FILES
                        $<TARGET_PDB_FILE:${TARGET}>
                        DESTINATION ${arg_DESTINATION}
                        CONFIGURATIONS Debug
                    )
                endif()

            endif()
        endif()
    endforeach()

endfunction(hils_install_target_artefacts)

function(hils_install_dependencies PACKAGE_NAME PACKAGE_DESTINATION)

    install(FILES
        $<TARGET_RUNTIME_DLLS:${PACKAGE_NAME}>
        DESTINATION ${PACKAGE_DESTINATION}
        CONFIGURATIONS Release Debug
    )

endfunction(hils_install_dependencies)

### End of HILS specific CMake configuration
include(${CMAKE_CURRENT_LIST_DIR}/CSToolkit.cmake)
