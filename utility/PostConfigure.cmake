include(CMakePackageConfigHelpers)

function(cstoolkit_post_configure)
    #message(STATUS "CSTOOLKIT_POST_CONFIGURE")

    # Fichier Config

    cstoolkit_get_all_targets(ALL_TARGETS)
  
    configure_package_config_file(
        ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/../templates/PackageConfig.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
        INSTALL_DESTINATION ${CMAKE_INSTALL_PREFIX}
    )

    write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMinorVersion
    )

    install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_PREFIX}
    )

    # Verification des dependances
    if(CSTOOLKIT_AUTO_FIND_PACKAGE OR CSTOOLKIT_CHECK_DEPENDENCIES)
        foreach(target ${ALL_TARGETS})
            cstoolkit_check_dependencies(${target})
        endforeach()
    endif()
endfunction()

function(cstoolkit_get_all_targets var)
    set(targets)
    cstoolkit_get_all_targets_recursive(targets ${CMAKE_CURRENT_SOURCE_DIR})
    set(${var} ${targets} PARENT_SCOPE)
endfunction()

macro(cstoolkit_get_all_targets_recursive targets dir)
    get_property(subdirectories DIRECTORY ${dir} PROPERTY SUBDIRECTORIES)
    foreach(subdir ${subdirectories})
        cstoolkit_get_all_targets_recursive(${targets} ${subdir})
    endforeach()
 
    get_property(current_targets DIRECTORY ${dir} PROPERTY BUILDSYSTEM_TARGETS)
    list(APPEND ${targets} ${current_targets})
endmacro()

function(cstoolkit_check_dependencies target)
    set(DEPENDENCIES)

    get_target_property(LINK_LIBRARIES ${target} LINK_LIBRARIES)
    get_target_property(INTERFACE_LINK_LIBRARIES ${target} INTERFACE_LINK_LIBRARIES)
    get_target_property(PLUGINS ${target} PLUGINS)

    if(LINK_LIBRARIES)
        list(APPEND DEPENDENCIES ${LINK_LIBRARIES})
    endif()
    if(INTERFACE_LINK_LIBRARIES)
        list(APPEND DEPENDENCIES ${INTERFACE_LINK_LIBRARIES})
    endif()
    if(PLUGINS)
        list(APPEND DEPENDENCIES ${PLUGINS})
    endif()

    list(REMOVE_DUPLICATES REMOVE_DUPLICATES)

    foreach(dep ${DEPENDENCIES})
        if(TARGET ${dep})
            continue()
        endif()
        string(REGEX MATCH "(.+)::(.+)" HAS_NAMESPACE ${dep})
        if(HAS_NAMESPACE AND CSTOOLKIT_AUTO_FIND_PACKAGE)
            set(PACKAGE_NAME ${CMAKE_MATCH_1})
            set(COMPONENT_NAME ${CMAKE_MATCH_2})
            # message("Namespace: ${PACKAGE_NAME}")
            # message("LibName: ${COMPONENT_NAME}")
            # message(${dep} " target not found")
           
            if(${PACKAGE_NAME} STREQUAL "Boost" AND ${COMPONENT_NAME} STREQUAL "headers")
                # Special case, if dependency is Boost::headers we can't use the regular find_package 
                # with the COMPONENTS option, we just do find_package(Boost)
                find_package(Boost REQUIRED QUIET GLOBAL)
                if(NOT ${PACKAGE_NAME}_FOUND)
                    cstoolkit_check_dependencies_message("${target}: find_package failed for dependency ${dep}")
                endif()
                continue()
            endif()

            find_package(${PACKAGE_NAME} QUIET COMPONENTS ${COMPONENT_NAME} GLOBAL)

            if(${PACKAGE_NAME}_FOUND)
                if(NOT TARGET ${dep})
                    cstoolkit_check_dependencies_message("${target}: find_package success for dependency ${dep} but target is missing")
                endif()
                continue()
            elseif(${PACKAGE_NAME}_NOT_FOUND_MESSAGE)
                cstoolkit_check_dependencies_message("${target}: find_package failed for dependency ${dep}\n"
                                "${${PACKAGE_NAME}_NOT_FOUND_MESSAGE}")
                continue()
            endif()

            cstoolkit_check_dependencies_message("${target}: find_package failed for dependency ${dep}")

        else() # NO NAMESPACE
            if(CSTOOLKIT_AUTO_FIND_PACKAGE)
                find_package(${dep} QUIET GLOBAL)

                if(${dep}_FOUND)
                    if(NOT TARGET ${dep})
                        cstoolkit_check_dependencies_message("${target}: find_package success for dependency ${dep} but target is missing")
                    endif()
                    continue()
                elseif(${dep}_NOT_FOUND_MESSAGE)
                    cstoolkit_check_dependencies_message("${target}: find_package failed for dependency ${dep}\n"
                                    "${${dep}_NOT_FOUND_MESSAGE}")
                    continue()
                endif()

                string(REGEX MATCH "(.*[^-_])[-_]?static$" HAS_STATIC_SUFFIX ${dep})
                if(HAS_STATIC_SUFFIX)
                    set(PACKAGE_NAME ${CMAKE_MATCH_1})
                    find_package(${PACKAGE_NAME} QUIET GLOBAL)
                    if(${PACKAGE_NAME}_FOUND)
                        if(NOT TARGET ${dep})
                            cstoolkit_check_dependencies_message("${target}: find_package success for dependency ${dep} but target is missing")
                        endif()
                        continue()
                    elseif(${PACKAGE_NAME}_NOT_FOUND_MESSAGE)
                        cstoolkit_check_dependencies_message("${target}: find_package failed for dependency ${dep}\n"
                                        "${${PACKAGE_NAME}_NOT_FOUND_MESSAGE}")
                        continue()
                    endif()
                endif()
            endif()

            if(CSTOOLKIT_CHECK_DEPENDENCIES)
                find_library(${dep}_LIB ${dep} NO_CACHE PATHS "${CSTOOLKIT_WINDOWS_KITS_LIB_DIR}")
                if(${${dep}_LIB})
                    continue()
                endif()
                
                cstoolkit_check_dependencies_message("${target}: unknown dependency ${dep}")
            endif()
        endif()
    endforeach()
endfunction()

macro(cstoolkit_check_dependencies_message message)
    if(CSTOOLKIT_CHECK_DEPENDENCIES)
        message(WARNING "CSToolkit: " ${message})
    endif()
endmacro()
