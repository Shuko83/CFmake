cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

include(CMakeParseArguments)
include($ENV{QMAKEPATH}/cmake/generate_target_info.cmake)

function(standard_project_info)

    set(options)
    set(one_value_keywords
        TARGET
        VERSION
        PRODUCT
        DESCRIPTION)
    set(multi_value_keywords)
    
    cmake_parse_arguments(PARAMS "${options}" "${one_value_keywords}" "${multi_value_keywords}" ${ARGN})

    if((NOT PARAMS_VERSION OR PARAMS_VERSION STREQUAL "") AND DEFINED ENV{VERSION})
    
        set(PARAMS_VERSION "$ENV{VERSION}")
        
        if(DEFINED ENV{BUILDNUMBER})
            set(PARAMS_VERSION "${PARAMS_VERSION}.$ENV{BUILDNUMBER}")
        endif()

    endif()

    if((NOT PARAMS_PRODUCT OR PARAMS_PRODUCT STREQUAL "") AND DEFINED ENV{PRODUCT})
        set(PARAMS_PRODUCT "$ENV{PRODUCT}")
    endif()
    
    string(TIMESTAMP year "%Y")

    generate_target_info(
        TARGET ${PARAMS_TARGET}
        VERSION "${PARAMS_VERSION}"
        PRODUCT "${PARAMS_PRODUCT}"
        DESCRIPTION "${PARAMS_DESCRIPTION}"
        COMPANY "Diginext"
        COPYRIGHT "Copyright (C) ${year} Diginext"
    )

endfunction()
