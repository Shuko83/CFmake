cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

include(CMakeParseArguments)
include($ENV{QMAKEPATH}/cmake/parse_json.cmake)
include($ENV{QMAKEPATH}/cmake/generate_target_info.cmake)

function(package_project_info)

    set(options)
    set(one_value_keywords
        TARGET
        PACKAGE
        DESCRIPTION)
    set(multi_value_keywords)
    
    cmake_parse_arguments(PARAMS "${options}" "${one_value_keywords}" "${multi_value_keywords}" ${ARGN})

    if(EXISTS "${PARAMS_PACKAGE}/package.json")
    
        file(READ ${PARAMS_PACKAGE}/package.json jsonfile)
        parse_json(properties jsonfile)
    
    else()
        message(FATAL_ERROR "'${PARAMS_PACKAGE}/package.json' is not a valid package.json file")
    endif()
    
    if(NOT properties.artifact OR properties.artifact STREQUAL "")
        message(FATAL_ERROR "'artifact' property not found in '${PARAMS_PACKAGE}/package.json'")
    endif()
    
    if(NOT properties.version OR properties.version STREQUAL "")
        message(FATAL_ERROR "'version' property not found in '${PARAMS_PACKAGE}/package.json'")
    endif()
    
    if(NOT properties.organization.name OR properties.organization.name STREQUAL "")
        message(FATAL_ERROR "'organization.name' property not found in '${PARAMS_PACKAGE}/package.json'")
    endif() 
    
    if(NOT properties.year OR properties.year STREQUAL "")
        message(FATAL_ERROR "'year' property not found in '${PARAMS_PACKAGE}/package.json'")
    endif()

    generate_target_info(
        TARGET ${PARAMS_TARGET}
        VERSION "${properties.version}"
        PRODUCT "${properties.artifact}"
        DESCRIPTION "${PARAMS_DESCRIPTION}"
        COMPANY "${properties.organization.name}"
        COPYRIGHT "Copyright (C) ${properties.year} ${properties.organization.name}"
    )
    
    clear_json(properties)

endfunction()
