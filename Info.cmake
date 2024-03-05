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

    #MESSAGE("Valeur Target : " ${PARAMS_TARGET})

    if(NOT PARAMS_TARGET OR PARAMS_TARGET STREQUAL "")
        message(FATAL_ERROR "TARGET argument must be defined")
    else()
    
        get_target_property(template ${PARAMS_TARGET} TYPE)

        #MESSAGE("Valeur template : " ${template})

        if(NOT template STREQUAL "EXECUTABLE" AND 
           NOT template STREQUAL "SHARED_LIBRARY" AND
           NOT template STREQUAL "MODULE_LIBRARY")
            message(FATAL_ERROR "generate_target_info : ${template} target not supported")
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
        get_target_property(AUTOGEN_BUILD_DIR ${PARAMS_TARGET} AUTOGEN_BUILD_DIR)

        if(AUTOGEN_BUILD_DIR AND NOT "${AUTOGEN_BUILD_DIR}" STREQUAL "")
            set(INFO_DIRECTORY ${AUTOGEN_BUILD_DIR}/info)
        else()
            set(INFO_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${PARAMS_TARGET}_autogen/info)
        endif()
        
        set(PARAMS_VERSION ${PARAMS_MAJOR_VERSION}.${PARAMS_MINOR_VERSION}.${PARAMS_PATCH_VERSION}.${PARAMS_BUILD_VERSION})
        set(cmake_target_cpp_info ${INFO_DIRECTORY}/info_${PARAMS_TARGET}.cpp)
        set(cmake_target_h_info ${INFO_DIRECTORY}/info_${PARAMS_TARGET}.h)
        string(REPLACE "(C)" "\\251" PARAMS_COPYRIGHT ${PARAMS_COPYRIGHT})         

        #message("Target info path cpp : " ${cmake_target_cpp_info})
        #message("Target info path h : " ${cmake_target_h_info})

        if(template STREQUAL "EXECUTABLE")
                    
            file(TO_CMAKE_PATH $ENV{QMAKEPATH} qmakepath)
            configure_file(${CMAKE_CURRENT_FUNCTION_LIST_DIR}/templates/info_exe_target.cpp.in ${cmake_target_cpp_info} @ONLY)
            configure_file(${CMAKE_CURRENT_FUNCTION_LIST_DIR}/templates/info_exe_target.h.in ${cmake_target_h_info} @ONLY)

            target_sources(${PARAMS_PRODUCT} PRIVATE ${cmake_target_cpp_info} )
            target_sources(${PARAMS_PRODUCT}
                PRIVATE FILE_SET "info" TYPE HEADERS BASE_DIRS ${INFO_DIRECTORY} FILES ${cmake_target_h_info}
            )
            source_group(TREE ${INFO_DIRECTORY}/.. PREFIX "Generated Files" FILES ${cmake_target_cpp_info} ${cmake_target_h_info})

        else()
                    
            file(TO_CMAKE_PATH $ENV{QMAKEPATH} qmakepath)
            configure_file(${CMAKE_CURRENT_FUNCTION_LIST_DIR}/templates/info_lib_target.cpp.in ${cmake_target_cpp_info} @ONLY)
            configure_file(${CMAKE_CURRENT_FUNCTION_LIST_DIR}/templates/info_lib_target.h.in ${cmake_target_h_info} @ONLY)



            target_sources(${PARAMS_PRODUCT} PRIVATE ${cmake_target_cpp_info} )
            target_sources(${PARAMS_PRODUCT}
                PRIVATE FILE_SET "info" TYPE HEADERS BASE_DIRS ${INFO_DIRECTORY} FILES ${cmake_target_h_info}
            )
            source_group(TREE ${INFO_DIRECTORY}/.. PREFIX "Generated Files" FILES ${cmake_target_cpp_info} ${cmake_target_h_info})

            #message("Target informations generation success !!")
        endif()

    elseif(UNIX)
    
        if(NOT PARAMS_VERSION STREQUAL "" AND 
           template STREQUAL "SHARED_LIBRARY")
            set_target_properties("${PARAMS_TARGET}" PROPERTIES SOVERSION ${PARAMS_MAJOR_VERSION})
        endif()
                                                          
    else()
        message(FATAL_ERROR "Target informations generation not supported on ${CMAKE_SYSTEM_NAME} platforms")
    endif()
    
endfunction()
