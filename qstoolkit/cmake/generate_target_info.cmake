cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

include(CMakeParseArguments)

function(generate_target_info)

    set(options)
    set(one_value_keywords
        TARGET
        VERSION
        PRODUCT
        DESCRIPTION
        COMPANY
        COPYRIGHT)
    set(multi_value_keywords)
    
    cmake_parse_arguments(PARAMS "${options}" "${one_value_keywords}" "${multi_value_keywords}" ${ARGN})

    if(NOT PARAMS_TARGET OR PARAMS_TARGET STREQUAL "")
        message(FATAL_ERROR "TARGET argument must be defined")
    else()
    
        get_target_property(template ${PARAMS_TARGET} TYPE)
        
        if(NOT template STREQUAL "EXECUTABLE" AND 
           NOT template STREQUAL "SHARED_LIBRARY" AND
           NOT template STREQUAL "MODULE_LIBRARY")
            message(FATAL_ERROR "${template} target not supported")
        endif()
        
    endif()
    
    set(PARAMS_MAJOR_VERSION 0)
    set(PARAMS_MINOR_VERSION 0)
    set(PARAMS_PATCH_VERSION 0)
    set(PARAMS_BUILD_VERSION 0)
    
    if(PARAMS_VERSION MATCHES "^([0-9]+)(\\.[0-9]+)?(\\.[0-9]+)?(\\.[0-9]+)?$")

        string(REPLACE "." ";" version_numbers ${PARAMS_VERSION})
        list(LENGTH version_numbers version_nbdigits)

        if(version_nbdigits GREATER 0)
            list(GET version_numbers 0 PARAMS_MAJOR_VERSION)
        endif()
        if(version_nbdigits GREATER 1)
            list(GET version_numbers 1 PARAMS_MINOR_VERSION)
        endif()
        if(version_nbdigits GREATER 2)
            list(GET version_numbers 2 PARAMS_PATCH_VERSION)
        endif()
        if(version_nbdigits GREATER 3)
            list(GET version_numbers 3 PARAMS_BUILD_VERSION)
        endif()
        
        set_target_properties(${PARAMS_TARGET} PROPERTIES VERSION ${PARAMS_MAJOR_VERSION}.${PARAMS_MINOR_VERSION}.${PARAMS_PATCH_VERSION})
        
    else()
        unset(PARAMS_VERSION)
    endif()

    if(WIN32)
    
        set(PARAMS_VERSION ${PARAMS_MAJOR_VERSION}.${PARAMS_MINOR_VERSION}.${PARAMS_PATCH_VERSION}.${PARAMS_BUILD_VERSION})

        if(BORLAND)
            
            add_custom_command(TARGET ${PARAMS_TARGET} POST_BUILD
                               COMMAND $ENV{QMAKEPATH}/tools/verpatch.exe $<TARGET_FILE:${PARAMS_TARGET}>
                                    /va ${PARAMS_VERSION}
                                    /langid 0x409
                                    /pv ${PARAMS_VERSION}
                                    /s CompanyName "${PARAMS_COMPANY}"
                                    /s FileDescription "${PARAMS_DESCRIPTION}"
                                    /s LegalCopyright "${PARAMS_COPYRIGHT}"
                                    /s ProductName "${PARAMS_PRODUCT}")
        
        else()
            
            string(REPLACE "(C)" "\\251" PARAMS_COPYRIGHT ${PARAMS_COPYRIGHT})
            
            set(cmake_version_info ${CMAKE_CURRENT_BINARY_DIR}/CMakeVersionInfo.h)
            set(cmake_version_resource ${CMAKE_CURRENT_BINARY_DIR}/CMakeVersionResource.rc)
            
            file(TO_CMAKE_PATH $ENV{QMAKEPATH} qmakepath)
            configure_file(${qmakepath}/cmake/windows/CMakeVersionInfo.in ${cmake_version_info} @ONLY)
            configure_file(${qmakepath}/cmake/windows/CMakeVersionResource.rc ${cmake_version_resource} COPYONLY)
            
            target_sources(${PARAMS_TARGET} PRIVATE ${cmake_version_info} ${cmake_version_resource})
        
        endif()
    
    elseif(UNIX)
    
        if(NOT PARAMS_VERSION STREQUAL "" AND 
           template STREQUAL "SHARED_LIBRARY")
            set_target_properties(${PARAMS_TARGET} PROPERTIES SOVERSION ${PARAMS_MAJOR_VERSION})
        endif()
                                                          
    else()
        message(FATAL_ERROR "Target informations generation not supported on ${CMAKE_SYSTEM_NAME} platforms")
    endif()
    
endfunction()
