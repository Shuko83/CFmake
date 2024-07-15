function(cstoolkit_generate_rc_file)

    set(options)
    set(one_value_keywords
        TARGET
        MANIFEST
        VERSION
        LANG
        CODEPAGE
        COMPANY
        COPYRIGHT)
    set(multi_value_keywords ICONS)
    
    cmake_parse_arguments(PARAMS "${options}" "${one_value_keywords}" "${multi_value_keywords}" ${ARGN})

    message(DEBUG "cstoolkit_generate_rc_file: Valeur Target : " ${PARAMS_TARGET})
    
    get_target_property(_target_type ${PARAMS_TARGET} TYPE)
    get_target_property(_suffix ${PARAMS_TARGET} SUFFIX)

    set(RC_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

    set(IDI_ICON_IN "\nIDI_ICON@ICON_NUMBER@\tICON\tDISCARDABLE\t@ICON_PATH@")
    set(ICON_NUMBER 1)
    foreach(ICON_PATH ${PARAMS_ICONS})
        string(CONFIGURE IDI_ICON_IN IDI_ICON @ONLY)
        string(APPEND IDI_ICONS IDI_ICON)
        math(EXPR ICON_NUMBER "${ICON_NUMBER}+1")
    endforeach()
    
    if(PARAMS_MANIFEST)
        if (_target_type STREQUAL "EXECUTABLE")
            set(RT_MANIFEST "CREATEPROCESS_MANIFEST_RESOURCE_ID")
        else()
            set(RT_MANIFEST "ISOLATIONAWARE_MANIFEST_RESOURCE_ID")
        endif()
        string(APPEND RT_MANIFEST " RT_MANIFEST \"${PARAMS_MANIFEST}\"")
    endif()

    if (_target_type STREQUAL "EXECUTABLE")
        set(FILETYPE "FILETYPE VFT_APP")
    else()
        set(FILETYPE "FILETYPE VFT_DLL")
    endif()

    if(NOT PARAMS_VERSION)
        set(PARAMS_VERSION "0.0.0.0") # default: English(USA)
    endif()
    string(REPLACE "." "," PARAMS_VERSION_COMA "${PARAMS_VERSION}")

    if(NOT PARAMS_LANG)
        set(PARAMS_LANG "1033") # default: English(USA)
    endif()
    math(EXPR PARAMS_LANG "${PARAMS_LANG}" OUTPUT_FORMAT HEXADECIMAL)
    string(SUBSTRING "${PARAMS_LANG}" 2 -1 PARAMS_LANG)
    string(REGEX REPLACE "^.*(....)$" "\\1" PARAMS_LANG "0000${PARAMS_LANG}") # padding on 4 char

    if(NOT PARAMS_CODEPAGE)
        set(PARAMS_CODEPAGE "1200") # default: Unicode
    endif()
    math(EXPR PARAMS_CODEPAGE_HEXA "${PARAMS_CODEPAGE}" OUTPUT_FORMAT HEXADECIMAL)
    string(SUBSTRING "${PARAMS_CODEPAGE_HEXA}" 2 -1 PARAMS_CODEPAGE_HEXA)
    string(REGEX REPLACE "^.*(....)$" "\\1" PARAMS_CODEPAGE "0000${PARAMS_CODEPAGE}") # padding on 4 char
    string(REGEX REPLACE "^.*(....)$" "\\1" PARAMS_CODEPAGE_HEXA "0000${PARAMS_CODEPAGE_HEXA}") # padding on 4 char

    # Grosse partie pour gerer les differentes config car source_group ne prend pas de generator expression
    if(NOT _suffix)
        if (_target_type STREQUAL "EXECUTABLE")
            set(_suffix "${CMAKE_EXECUTABLE_SUFFIX}")
        else()
            set(_suffix "${CMAKE_SHARED_LIBRARY_SUFFIX}")
        endif()
    endif()

    if(CMAKE_CONFIGURATION_TYPES)
        foreach(_config ${CMAKE_CONFIGURATION_TYPES})
            string(TOUPPER "${_config}" _config)
            string(TOLOWER "${_config}" _config_lower)
            set(FILENAME "${PARAMS_TARGET}${CMAKE_${_config}_POSTFIX}${_suffix}")
            configure_file(${CMAKE_CURRENT_FUNCTION_LIST_DIR}/../templates/Target_resource.rc.in
                ${RC_DIRECTORY}/${PARAMS_TARGET}_resource-${_config_lower}.rc
                @ONLY)
            target_sources(${PARAMS_TARGET} PRIVATE $<$<CONFIG:${_config}>:${RC_DIRECTORY}/${PARAMS_TARGET}_resource-${_config_lower}.rc>)
            source_group(TREE ${RC_DIRECTORY} PREFIX "Resource Files" FILES ${RC_DIRECTORY}/${PARAMS_TARGET}_resource-${_config_lower}.rc)
        endforeach()
    else()
        string(TOUPPER "${CMAKE_BUILD_TYPE}" _config)
        string(TOLOWER "${_config}" _config_lower)
        set(FILENAME "${PARAMS_TARGET}${CMAKE_${_config}_POSTFIX}${_suffix}")
        configure_file(${CMAKE_CURRENT_FUNCTION_LIST_DIR}/../templates/Target_resource.rc.in
            ${RC_DIRECTORY}/${PARAMS_TARGET}_resource-${_config_lower}.rc
            @ONLY)
        target_sources(${PARAMS_TARGET} PRIVATE ${RC_DIRECTORY}/${PARAMS_TARGET}_resource-${_config_lower}.rc)
        source_group(TREE ${RC_DIRECTORY} PREFIX "Resource Files" FILES ${RC_DIRECTORY}/${PARAMS_TARGET}_resource-${_config_lower}.rc)
    endif()
    message(DEBUG "Target RC generation success !!")
endfunction()
