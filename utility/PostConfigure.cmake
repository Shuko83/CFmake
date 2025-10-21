include(CMakePackageConfigHelpers)

function(cstoolkit_post_configure)
    cstoolkit_end_timer(CSTOOLKIT_CONFIGURE_TIMER CSTOOLKIT_CONFIGURE_ELAPSED)
    message(STATUS "CSToolkit: Pre-Configure done (${CSTOOLKIT_CONFIGURE_ELAPSED}s)")

    cstoolkit_start_timer(CSTOOLKIT_POST_CONFIGURE_TIMER)

    get_property(CSTOOLKIT_INSTALLED_TARGETS GLOBAL PROPERTY CSTOOLKIT_INSTALLED_TARGETS)

    # Fichier Config
    set(CSTOOLKIT_INSTALL_TARGETS_MISSING ${CSTOOLKIT_INSTALL_TARGETS})
    list(REMOVE_ITEM CSTOOLKIT_INSTALL_TARGETS_MISSING ${CSTOOLKIT_INSTALLED_TARGETS})

    if(CSTOOLKIT_INSTALL_TARGETS_MISSING)
        list(JOIN CSTOOLKIT_INSTALL_TARGETS_MISSING ", " CSTOOLKIT_INSTALL_TARGETS_MISSING_STRING)
        message(SEND_ERROR "CSToolkit: Could not find specified targets in CSTOOLKIT_INSTALL_TARGETS: ${CSTOOLKIT_INSTALL_TARGETS_MISSING_STRING}")
    elseif(NOT CSTOOLKIT_INSTALLED_TARGETS)
        message(STATUS "CSToolkit: Install mode: No Install")
    else()
        # Detection de la methode d'install
        list(LENGTH CSTOOLKIT_INSTALLED_TARGETS CSTOOLKIT_INSTALLED_TARGETS_NB)

        if(CSTOOLKIT_INSTALLED_TARGETS_NB EQUAL 1) # mode single component
            message(STATUS "CSToolkit: Install mode: Single Component")
            set(CSTOOLKIT_CMAKEDIR "cmake")

            get_target_property(TARGET_INSTALL_DESTINATION_DEFAULT ${CSTOOLKIT_INSTALLED_TARGETS} INSTALL_DESTINATION_DEFAULT) # READONLY
            get_target_property(TARGET_INSTALL_DESTINATION ${CSTOOLKIT_INSTALLED_TARGETS} INSTALL_DESTINATION)
            
            if(TARGET_INSTALL_DESTINATION STREQUAL TARGET_INSTALL_DESTINATION_DEFAULT)
                set_target_properties(${CSTOOLKIT_INSTALLED_TARGETS} PROPERTIES INSTALL_DESTINATION ".")
            endif()

            set_target_properties(${CSTOOLKIT_INSTALLED_TARGETS} PROPERTIES INSTALL_CONFIG_NAME "${PROJECT_NAME}")

            set_target_properties(${CSTOOLKIT_INSTALLED_TARGETS} PROPERTIES INSTALL_CMAKEDIR "${CSTOOLKIT_CMAKEDIR}")

            # We use the config of the target directly
            # no need to generate a project level config file
            write_basic_package_version_file(
                ${CMAKE_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
                VERSION ${PROJECT_VERSION}
                COMPATIBILITY SameMinorVersion
            )

            install(FILES
                ${CMAKE_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
                DESTINATION "${CSTOOLKIT_CMAKEDIR}"
            )
        else() # mode multi component
            message(STATUS "CSToolkit: Install mode: Multi Component")

            set(FIND_REQUIRED_COMPONENTS "${CSTOOLKIT_INSTALLED_TARGETS}")
            list(TRANSFORM FIND_REQUIRED_COMPONENTS PREPEND "    set(${PROJECT_NAME}_FIND_REQUIRED_")
            list(TRANSFORM FIND_REQUIRED_COMPONENTS APPEND " 1)")
            list(JOIN FIND_REQUIRED_COMPONENTS "\n" FIND_REQUIRED_COMPONENTS)

            configure_package_config_file(
                ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/../templates/PackageConfig.cmake.in
                ${CMAKE_BINARY_DIR}/${PROJECT_NAME}Config.cmake
                INSTALL_DESTINATION "."
            )

            write_basic_package_version_file(
                ${CMAKE_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
                VERSION ${PROJECT_VERSION}
                COMPATIBILITY SameMinorVersion
            )

            install(FILES
                ${CMAKE_BINARY_DIR}/${PROJECT_NAME}Config.cmake
                ${CMAKE_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
                DESTINATION "."
            )
        endif()
    endif()

    configure_file(${CMAKE_CURRENT_FUNCTION_LIST_DIR}/../templates/gitinfo.in ${CMAKE_BINARY_DIR}/.gitinfo @ONLY)

    install(FILES
        ${CMAKE_BINARY_DIR}/.gitinfo
        DESTINATION "."
    )

    get_property(CSTOOLKIT_INSTALLED_TARGETS_ALL GLOBAL PROPERTY CSTOOLKIT_INSTALLED_TARGETS_ALL)
    foreach(target ${CSTOOLKIT_INSTALLED_TARGETS_ALL})
        cstoolkit_install_export(${target})
    endforeach()

    cstoolkit_get_all_targets(ALL_TARGETS)

    # Verification des dependances
    if(CSTOOLKIT_AUTO_FIND_PACKAGE OR CSTOOLKIT_CHECK_DEPENDENCIES)
        foreach(target ${ALL_TARGETS})
            cstoolkit_check_dependencies(${target})
        endforeach()
    endif()

    # Deployement des dependances runtime et plugins
    foreach(target ${ALL_TARGETS})
        cstoolkit_compute_runtime_dependencies(${target})
    endforeach()

    cstoolkit_internal_print_genex_post_configure()

    cstoolkit_end_timer(CSTOOLKIT_POST_CONFIGURE_TIMER CSTOOLKIT_POST_CONFIGURE_ELAPSED)
    message(STATUS "CSToolkit: Post-Configure done (${CSTOOLKIT_POST_CONFIGURE_ELAPSED}s)")
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

function(cstoolkit_install_export target)
    get_target_property(TARGET_INSTALL_EXCLUDE_FROM_ALL ${target} INSTALL_EXCLUDE_FROM_ALL) # READONLY
    get_target_property(TARGET_INSTALL_CMAKEDIR ${target} INSTALL_CMAKEDIR)
    get_target_property(TARGET_INSTALL_CONFIG_NAME ${target} INSTALL_CONFIG_NAME)
    get_target_property(TARGET_INSTALL_TARGETS_NAME ${target} INSTALL_TARGETS_NAME) # READONLY
    get_target_property(TARGET_INSTALL_COMPONENT ${target} INSTALL_COMPONENT) # READONLY
    get_target_property(TARGET_NAMESPACE ${target} NAMESPACE) # READONLY

    cstoolkit_target_genex_eval(${target} TARGET_INSTALL_CMAKEDIR)
    cstoolkit_target_genex_eval(${target} TARGET_INSTALL_CONFIG_NAME)

    foreach(CMAKEDIR ${TARGET_INSTALL_CMAKEDIR})
        foreach(CONFIG_NAME ${TARGET_INSTALL_CONFIG_NAME})
            install(EXPORT ${TARGET_INSTALL_TARGETS_NAME}
                NAMESPACE ${TARGET_NAMESPACE}
                FILE ${CONFIG_NAME}Targets.cmake
                DESTINATION ${CMAKEDIR}
                COMPONENT ${TARGET_INSTALL_COMPONENT}
                ${TARGET_INSTALL_EXCLUDE_FROM_ALL}
            )
        endforeach()
    endforeach()
endfunction()

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

    cstoolkit_genex_extract("${DEPENDENCIES}" DEPENDENCIES GENEX_DEPENDENCIES_LIST)
    
    list(REMOVE_DUPLICATES DEPENDENCIES)

    foreach(dep ${DEPENDENCIES})
        if(NOT dep)
            continue()
        endif()

        if(TARGET ${dep})
            continue()
        endif()

        string(REGEX MATCH "(.+)::(.+)" HAS_NAMESPACE ${dep})
        if(HAS_NAMESPACE AND CSTOOLKIT_AUTO_FIND_PACKAGE)
            set(PACKAGE_NAME ${CMAKE_MATCH_1})
            set(COMPONENT_NAME ${CMAKE_MATCH_2})
            message(DEBUG "Namespace: ${PACKAGE_NAME}")
            message(DEBUG "LibName: ${COMPONENT_NAME}")
            message(DEBUG ${dep} " target not found")
           
            if(${PACKAGE_NAME} STREQUAL "Boost" AND ${COMPONENT_NAME} STREQUAL "headers")
                # Special case, if dependency is Boost::headers we can't use the regular find_package 
                # with the COMPONENTS option, we just do find_package(Boost)
                find_package(Boost REQUIRED QUIET GLOBAL)
                if(NOT ${PACKAGE_NAME}_FOUND)
                    cstoolkit_check_dependencies_error("${target}: find_package failed for dependency ${dep}")
                endif()
                continue()
            endif()

            find_package(${PACKAGE_NAME} QUIET COMPONENTS ${COMPONENT_NAME} GLOBAL)

            if(${PACKAGE_NAME}_FOUND)
                if(NOT TARGET ${dep})
                    cstoolkit_check_dependencies_error("${target}: find_package success for dependency ${dep} but target is still missing")
                endif()
                continue()
            elseif(${PACKAGE_NAME}_NOT_FOUND_MESSAGE)
                cstoolkit_check_dependencies_error("${target}: find_package failed for dependency ${dep}\n"
                                "${${PACKAGE_NAME}_NOT_FOUND_MESSAGE}")
                continue()
            endif()

            cstoolkit_check_dependencies_error("${target}: find_package failed for dependency ${dep}")

        else() # NO NAMESPACE
            #if("${dep}" IN_LIST CMAKE_C_STANDARD_LIBRARIES)
            #    continue()
            #endif()
            #if("${dep}" IN_LIST CMAKE_CXX_STANDARD_LIBRARIES)
            #    continue()
            #endif()

            if(CSTOOLKIT_AUTO_FIND_PACKAGE)
                find_package(${dep} QUIET GLOBAL)

                if(${dep}_FOUND)
                    if(NOT TARGET ${dep})
                        cstoolkit_check_dependencies_error("${target}: find_package success for dependency ${dep} but target is still missing")
                    endif()
                    continue()
                elseif(${dep}_NOT_FOUND_MESSAGE)
                    cstoolkit_check_dependencies_error("${target}: find_package failed for dependency ${dep}\n"
                                    "${${dep}_NOT_FOUND_MESSAGE}")
                    continue()
                endif()

                string(REGEX MATCH "(.*[^-_])[-_]?[Ss][Tt][Aa][Tt][Ii][Cc]$" HAS_STATIC_SUFFIX ${dep})
                if(HAS_STATIC_SUFFIX)
                    set(PACKAGE_NAME ${CMAKE_MATCH_1})
                    find_package(${PACKAGE_NAME} QUIET GLOBAL)
                    if(${PACKAGE_NAME}_FOUND)
                        if(NOT TARGET ${dep})
                            cstoolkit_check_dependencies_error("${target}: find_package success for dependency ${dep} but target is still missing")
                        endif()
                        continue()
                    elseif(${PACKAGE_NAME}_NOT_FOUND_MESSAGE)
                        cstoolkit_check_dependencies_error("${target}: find_package failed for dependency ${dep}\n"
                                        "${${PACKAGE_NAME}_NOT_FOUND_MESSAGE}")
                        continue()
                    endif()
                endif()
            endif()

            if(CSTOOLKIT_CHECK_DEPENDENCIES)
                find_library(${dep}_LIB ${dep} NO_CACHE PATHS "${CSTOOLKIT_WINDOWS_KITS_LIB_DIR}")
                if(${dep}_LIB)
                    continue()
                endif()

                if(EXISTS ${dep})
                    continue()
                endif()
                
                cstoolkit_check_dependencies_warning("${target}: Unknown dependency ${dep}")
            endif()
        endif()
    endforeach()
endfunction()

macro(cstoolkit_check_dependencies_error)
    if(CSTOOLKIT_CHECK_DEPENDENCIES)
        message(NOTICE ${COLOR_RED_BOLD} "CSToolkit: " ${ARGN} ${COLOR_RESET})
    endif()
endmacro()

macro(cstoolkit_check_dependencies_warning)
    if(CSTOOLKIT_CHECK_DEPENDENCIES)
        message(NOTICE ${COLOR_YELLOW_BOLD} "CSToolkit: " ${ARGN} ${COLOR_RESET})
    endif()
endmacro()

function(cstoolkit_compute_runtime_dependencies target)
    set(TARGET_GENEX_RUNTIME_DEPENDENCIES "")
    cstoolkit_get_runtime_dependencies(${target} TARGET_RUNTIME_DEPENDENCIES TARGET_GENEX_RUNTIME_DEPENDENCIES)
    set_target_properties(${target} PROPERTIES RUNTIME_DEPENDENCIES "${TARGET_RUNTIME_DEPENDENCIES};${TARGET_GENEX_RUNTIME_DEPENDENCIES}")

    get_target_property(PLUGINS_DESTINATION_SIZE ${target} PLUGINS_DESTINATION_SIZE)
    if(NOT PLUGINS_DESTINATION_SIZE)
        return()
    endif()

    set(PLUGINS_DEPENDENCIES) # All dependencies

    get_target_property(PLUGINS_DESTINATIONS ${target} PLUGINS_DESTINATIONS)

    # Calcul des dependences
    set(DESTINATION_INDEX 0)
    while(DESTINATION_INDEX LESS PLUGINS_DESTINATION_SIZE)
        get_target_property(PLUGINS_LIST ${target} PLUGINS_DESTINATION${DESTINATION_INDEX}_LIST)

        list(APPEND PLUGINS_DEPENDENCIES ${PLUGINS_LIST})

        set(PLUGINS_LIST_DEPENDENCIES)
        set(PLUGINS_LIST_GENEX_DEPENDENCIES)

        foreach(_plugin ${PLUGINS_LIST})
            if(NOT TARGET ${_plugin})
                message(NOTICE "CSToolkit: add_target(${target}): Plugin \"${_plugin}\" is not a defined target and will be ignored.")
                continue()
            endif()
            cstoolkit_get_runtime_dependencies(${_plugin} PLUGINS_LIST_DEPENDENCIES PLUGINS_LIST_GENEX_DEPENDENCIES)
        endforeach()

        list(APPEND PLUGINS_DEPENDENCIES ${PLUGINS_LIST_DEPENDENCIES} "${PLUGINS_LIST_GENEX_DEPENDENCIES}")
        list(APPEND PLUGINS_LIST ${PLUGINS_LIST_DEPENDENCIES} "${PLUGINS_LIST_GENEX_DEPENDENCIES}")

        set_target_properties(${target} PROPERTIES PLUGINS_DESTINATION${DESTINATION_INDEX}_LIST "${PLUGINS_LIST}")

        math(EXPR DESTINATION_INDEX "${DESTINATION_INDEX}+1")
    endwhile()

    set(PLUGINS_DEPENDENCIES "$<LIST:REMOVE_DUPLICATES,${PLUGINS_DEPENDENCIES}>")
    set_target_properties(${target} PROPERTIES PLUGINS_DEPENDENCIES "${PLUGINS_DEPENDENCIES}")
endfunction()

function(cstoolkit_get_runtime_dependencies target runtime_dependencies genex_runtime_dependencies)
    get_target_property(IMPORTED ${target} IMPORTED)
    set(_target_dependencies)
    set(_target_genex_dependencies)
    if(IMPORTED)
        if(CMAKE_CONFIGURATION_TYPES)
            foreach(_config ${CMAKE_CONFIGURATION_TYPES})
                string(TOUPPER "${_config}" _config)
                get_target_property(LIBRARIES ${target} IMPORTED_LINK_DEPENDENT_LIBRARIES_${_config})
                if(LIBRARIES)
                    list(APPEND _target_dependencies ${LIBRARIES})
                endif()
            endforeach()
        else()
            string(TOUPPER "${CMAKE_BUILD_TYPE}" _config)
            get_target_property(LIBRARIES ${target} IMPORTED_LINK_DEPENDENT_LIBRARIES_${_config})
            if(LIBRARIES)
                list(APPEND _target_dependencies ${LIBRARIES})
            endif()
        endif()
        get_target_property(LIBRARIES ${target} INTERFACE_LINK_LIBRARIES)
        if(LIBRARIES)
            list(APPEND _target_dependencies ${LIBRARIES})
        endif()
    else()
        get_target_property(LIBRARIES ${target} LINK_LIBRARIES)
        if(LIBRARIES)
            list(APPEND _target_dependencies ${LIBRARIES})
        endif()
        get_target_property(LIBRARIES ${target} INTERFACE_LINK_LIBRARIES)
        if(LIBRARIES)
            list(APPEND _target_dependencies ${LIBRARIES})
        endif()
    endif()
    
    cstoolkit_genex_extract("${_target_dependencies}" _target_dependencies _target_genex_dependencies_list)
    foreach(elem ${_target_genex_dependencies_list})
        list(APPEND _target_genex_dependencies "${${elem}}")
    endforeach()

    list(REMOVE_DUPLICATES _target_dependencies)
    if(_target_genex_dependencies)
        list(APPEND ${genex_runtime_dependencies} "${_target_genex_dependencies}")
    endif()

    foreach(_dep ${_target_dependencies})
        if(TARGET "${_dep}")
            if(NOT _dep IN_LIST ${runtime_dependencies})
                list(APPEND ${runtime_dependencies} "${_dep}")
                cstoolkit_get_runtime_dependencies(${_dep} ${runtime_dependencies} ${genex_runtime_dependencies})
            endif()
        endif()
    endforeach()
    set(${runtime_dependencies} "${${runtime_dependencies}}" PARENT_SCOPE)
    set(${genex_runtime_dependencies} "${${genex_runtime_dependencies}}" PARENT_SCOPE)
endfunction()

function(cstoolkit_list_intersection lista listb outputlist)
    set(aminusb ${${lista}})
    list(REMOVE_ITEM aminusb ${${listb}})

    set(bminusa ${${listb}})
    list(REMOVE_ITEM bminusa ${${lista}})

    set(${outputlist} ${${lista}})
    list(APPEND ${outputlist} ${${listb}})
    list(REMOVE_DUPLICATES ${outputlist})
    list(REMOVE_ITEM ${outputlist} ${aminusb} ${bminusa})

    set(${outputlist} "${${outputlist}}" PARENT_SCOPE)
endfunction()
