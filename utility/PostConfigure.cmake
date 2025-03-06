include(CMakePackageConfigHelpers)

function(cstoolkit_post_configure)
    string(TIMESTAMP CSTOOLKIT_CONFIGURE_TIME "%s%f")
    math(EXPR elapsed "${CSTOOLKIT_CONFIGURE_TIME}-${CSTOOLKIT_START_TIME}")
    math(EXPR second_elapsed "${elapsed}/1000000")
    math(EXPR tenth_elapsed "(${elapsed}-${second_elapsed}*1000000+50000)/100000")
    message(STATUS "CSToolkit: Configure done (${second_elapsed}.${tenth_elapsed}s)")

    get_property(CSTOOLKIT_INSTALL_TARGETS_ALL GLOBAL PROPERTY CSTOOLKIT_INSTALL_TARGETS_ALL)

    # Fichier Config
    set(CSTOOLKIT_INSTALL_TARGETS_MISSING ${CSTOOLKIT_INSTALL_TARGETS})
    list(REMOVE_ITEM CSTOOLKIT_INSTALL_TARGETS_MISSING ${CSTOOLKIT_INSTALL_TARGETS_ALL})

    if(CSTOOLKIT_INSTALL_TARGETS_MISSING)
        list(JOIN CSTOOLKIT_INSTALL_TARGETS_MISSING ", " CSTOOLKIT_INSTALL_TARGETS_MISSING_STRING)
        message(SEND_ERROR "CSToolkit: Could not find specified install targets: ${CSTOOLKIT_INSTALL_TARGETS_MISSING_STRING}")
    elseif(NOT CSTOOLKIT_INSTALL_TARGETS_ALL)
        message(STATUS "CSToolkit: Install mode: No Install")
    else()
        # Detection de la methode d'install
        list(LENGTH CSTOOLKIT_INSTALL_TARGETS CSTOOLKIT_INSTALL_TARGETS_NB)

        if(CSTOOLKIT_INSTALL_TARGETS_NB EQUAL 1) # mode single component
            message(STATUS "CSToolkit: Install mode: Single Component")

            # We use the config of the target directly
            # no need to generate a project level config file
            write_basic_package_version_file(
                ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
                VERSION ${PROJECT_VERSION}
                COMPATIBILITY SameMinorVersion
            )
        
            install(FILES
                ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
                DESTINATION cmake
            )
        else() # mode multi component
            message(STATUS "CSToolkit: Install mode: Multi Component")

            configure_package_config_file(
                ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/../templates/PackageConfig.cmake.in
                ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
                INSTALL_DESTINATION "."
            )

            write_basic_package_version_file(
                ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
                VERSION ${PROJECT_VERSION}
                COMPATIBILITY SameMinorVersion
            )

            install(FILES
                ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
                ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
                DESTINATION "."
            )
        endif()
    endif()

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
    string(TIMESTAMP CSTOOLKIT_POST_CONFIGURE_TIME "%s%f")
    math(EXPR elapsed "${CSTOOLKIT_POST_CONFIGURE_TIME}-${CSTOOLKIT_CONFIGURE_TIME}")
    math(EXPR second_elapsed "${elapsed}/1000000")
    math(EXPR tenth_elapsed "(${elapsed}-${second_elapsed}*1000000+50000)/100000")
    message(STATUS "CSToolkit: Post-Configure done (${second_elapsed}.${tenth_elapsed}s)")
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
                    cstoolkit_check_dependencies_error("${target}: find_package success for dependency ${dep} but target is missing")
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
                        cstoolkit_check_dependencies_error("${target}: find_package success for dependency ${dep} but target is missing")
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
                            cstoolkit_check_dependencies_error("${target}: find_package success for dependency ${dep} but target is missing")
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
                
                cstoolkit_check_dependencies_warning("${target}: unknown dependency ${dep}")
            endif()
        endif()
    endforeach()
endfunction()

macro(cstoolkit_check_dependencies_error)
    if(CSTOOLKIT_CHECK_DEPENDENCIES)
        string(ASCII 27 Esc)
        set(BoldRed "${Esc}[1;31m")
        set(ColourReset "${Esc}[m")
        message(NOTICE ${BoldRed} "CSToolkit: " ${ARGN} ${ColourReset})
    endif()
endmacro()

macro(cstoolkit_check_dependencies_warning)
    if(CSTOOLKIT_CHECK_DEPENDENCIES)
        string(ASCII 27 Esc)
        set(BoldYellow "${Esc}[1;33m")
        set(ColourReset "${Esc}[m")
        message(NOTICE ${BoldYellow} "CSToolkit: " ${ARGN} ${ColourReset})
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
        if(TARGET ${_dep})
            get_target_property(_lib_type ${_dep} TYPE)
            if(NOT _dep IN_LIST ${runtime_dependencies})
                if(_lib_type STREQUAL SHARED_LIBRARY)
                    list(APPEND ${runtime_dependencies} "${_dep}")
                endif()
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

function(cstoolkit_genex_extract input outputvar genexprefix)
    #message("Entry:   ${input}")
    string(REGEX MATCH "\\$<.*>" genex "${input}")
    if(NOT genex)
        #message("Not genex: '${input}'")
        set(${outputvar} "${input}" PARENT_SCOPE)
        set(${genexprefix} "" PARENT_SCOPE)
        return()
    endif()

    set(${genexprefix} "")
    set(genex_cnt 0)
    set(output "")
    set(nesting_level 0)
    foreach(elem ${input})
        #message("Elem! ${elem}")
        #message("nesting_level! ${nesting_level}")
        if(nesting_level EQUAL 0)
            string(FIND "${elem}" "$<" genexStartPos)
            if(genexStartPos EQUAL -1)
                #message("not genex: ${elem}")
                list(APPEND output "${elem}")
                continue()
            endif()
            #start of a genex
            math(EXPR nesting_level "${nesting_level}+1")
            math(EXPR genex_cnt "${genex_cnt}+1")
            set(${genexprefix}${genex_cnt} "${elem}")
            list(APPEND ${genexprefix} "${genexprefix}${genex_cnt}")
            #searching genex end
            math(EXPR genexStartPos "${genexStartPos}+2")
            string(SUBSTRING "${elem}" ${genexStartPos} -1 subelem)
            #message("Substring: '${subelem}'")
            string(FIND "${subelem}" "$<" genexStartPos)
            string(FIND "${subelem}" ">" genexEndPos)
            while(NOT genexStartPos EQUAL genexEndPos)
                if(genexStartPos LESS genexEndPos AND NOT genexStartPos EQUAL -1)
                    math(EXPR nesting_level "${nesting_level}+1")
                    math(EXPR genexStartPos "${genexStartPos}+2")
                    string(SUBSTRING "${subelem}" ${genexStartPos} -1 subelem)
                else()
                    math(EXPR nesting_level "${nesting_level}-1")
                    math(EXPR genexEndPos "${genexEndPos}+1")
                    string(SUBSTRING "${subelem}" ${genexEndPos} -1 subelem)
                endif()
                string(FIND "${subelem}" "$<" genexStartPos)
                string(FIND "${subelem}" ">" genexEndPos)
            endwhile()
        else()
            set(${genexprefix}${genex_cnt} "${${genexprefix}${genex_cnt}};${elem}")
            set(subelem "${elem}")
            string(FIND "${subelem}" "$<" genexStartPos)
            string(FIND "${subelem}" ">" genexEndPos)
            while(NOT genexStartPos EQUAL genexEndPos)
                if(genexStartPos LESS genexEndPos AND NOT genexStartPos EQUAL -1)
                    math(EXPR nesting_level "${nesting_level}+1")
                    math(EXPR genexStartPos "${genexStartPos}+2")
                    string(SUBSTRING "${subelem}" ${genexStartPos} -1 subelem)
                else()
                    math(EXPR nesting_level "${nesting_level}-1")
                    math(EXPR genexEndPos "${genexEndPos}+1")
                    string(SUBSTRING "${subelem}" ${genexEndPos} -1 subelem)
                endif()
                string(FIND "${subelem}" "$<" genexStartPos)
                string(FIND "${subelem}" ">" genexEndPos)
            endwhile()
        endif()
    endforeach()

    set(${outputvar} "${output}" PARENT_SCOPE)
    set(${genexprefix} "${${genexprefix}}" PARENT_SCOPE)
    foreach(elem ${${genexprefix}})
        #message("genex: ${${elem}}")
        set(${elem} "${${elem}}" PARENT_SCOPE)
    endforeach()
    
endfunction()

function(cstoolkit_genex_cleanup input outputvar)
    message("Entry:   ${input}")
    string(REGEX MATCH "\\$<.*>" genex "${input}")
    if(NOT genex)
        message("Not genex: '${input}'")
        set(${outputvar} "${input}" PARENT_SCOPE)
        return()
    endif()

    set(index 0)
    set(literal_start 0)
    set(token_cnt 0)
    set(previous_dollar false)
    set(in_value false)
    string(LENGTH "${input}" input_length)
    set(output "")

    while(index LESS input_length)
        string(SUBSTRING "${input}" ${index} 1 char)
        if(char STREQUAL "$")
            set(previous_dollar true)
        else()
            if(char STREQUAL "<")
                if(token_cnt EQUAL 0)
                    set(params_cnt 0)
                    math(EXPR token_start ${index}+1)
                endif()
                math(EXPR token_cnt "${token_cnt}+1")
            elseif(char STREQUAL ">")
                math(EXPR token_cnt "${token_cnt}-1")
                if(token_cnt EQUAL 0)
                    if(NOT in_value)
                        math(EXPR token_end ${index}-1)
                    endif()

                    #########################################################
                    #START CLEANUP

                    math(EXPR literal_length "${token_start}-${literal_start}")
                    string(SUBSTRING "${input}" ${literal_start} ${literal_length} literal)
                    #message("Starlit: ${literal}")
                    set(output "${output}${literal}")

                    math(EXPR token_length "${token_end}-${token_start}+1")
                    string(SUBSTRING "${input}" ${token_start} ${token_length} token)
                    message("Token:   ${token}")
                    cstoolkit_genex_cleanup("${token}" token)
                    set(output "${output}${token}")
                
                    set(params_index 0)
                    while(params_index LESS params_cnt)
                        if(params_index EQUAL 0)
                            set(output "${output}:")
                        else()
                            set(output "${output},")
                        endif()
                        math(EXPR next_params_index "${params_index}+1")
                        math(EXPR token_value_length "${param${next_params_index}_index}-${param${params_index}_index}-1")
                        string(SUBSTRING "${input}" ${param${params_index}_index} ${token_value_length} token_value)
                        message("Param${params_index}: ${token_value}")
                        cstoolkit_genex_cleanup("${token_value}" token_value)
                        set(output "${output}${token_value}")
                        set(params_index ${next_params_index})
                    endwhile()

                    #END CLEANUP
                    #########################################################

                    set(literal_start ${index})
                endif()
            elseif(char STREQUAL ":" AND token_cnt EQUAL 1 AND NOT in_value)
                set(in_value true)
                math(EXPR token_end ${index}-1)
                math(EXPR param${params_cnt}_index ${index}+1)
                math(EXPR params_cnt "${params_cnt}+1")
                math(EXPR param${params_cnt}_index ${input_length})
            elseif(char STREQUAL "," AND token_cnt EQUAL 1)
                math(EXPR param${params_cnt}_index ${index}+1)
                math(EXPR params_cnt "${params_cnt}+1")
                math(EXPR param${params_cnt}_index ${input_length})
            endif()
            set(previous_dollar false)
        endif()
        math(EXPR index "${index}+1")
    endwhile()

    math(EXPR literal_length "${index}-${literal_start}")
    string(SUBSTRING "${input}" ${literal_start} ${literal_length} literal)
    message("End literal = ${literal}")
    set(output "${output}${literal}")

    message("Cleaned: ${output}")
    set(${outputvar} "${output}" PARENT_SCOPE)
endfunction()
