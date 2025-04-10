set(USAGE "${CMAKE_COMMAND} -P cstoolkit_copy.cmake -- [-e] [-d] <file|directory>... <destination_dir>")
# -e : Raise error if a source does not exists
# -d : All sources are concidered directories

set(ARGI 0)
while(NOT "${CMAKE_ARGV${ARGI}}" STREQUAL "--" AND ${ARGI} LESS ${CMAKE_ARGC})
    math(EXPR ARGI "${ARGI}+1")
endwhile()
math(EXPR ARGI "${ARGI}+1")

# Parsing Options
while(ARGI LESS CMAKE_ARGC)
    string(REGEX MATCH "^-.*$" IS_OPTION "${CMAKE_ARGV${ARGI}}")    
    if(IS_OPTION)
        if(CMAKE_ARGV${ARGI} STREQUAL "-d")
            set(DIRECTORY_MODE ON)
            message(DEBUG "cstoolkit_copy: DIRECTORY_MODE ON")
        elseif(CMAKE_ARGV${ARGI} STREQUAL "-e")
            set(STOP_ON_ERROR ON)
            message(DEBUG "cstoolkit_copy: STOP_ON_ERROR ON")
        else()
            message(FATAL_ERROR "cstoolkit_copy Error: Unknown option \"${CMAKE_ARGV${ARGI}}\"\n"
                                "Usage: ${USAGE}")
            # cmake_language(EXIT 10) only cmake > 3.29
            return()
        endif()
    else()
        break()   
    endif()

    math(EXPR ARGI "${ARGI}+1")
endwhile()

math(EXPR DESTINATION_INDEX "${CMAKE_ARGC}-1")

if(ARGI GREATER DESTINATION_INDEX)
    message(FATAL_ERROR "cstoolkit_copy Error: Missing arguments\n"
                        "Usage: ${USAGE}")
    return()
endif()

set(SOURCE_LIST)
while(ARGI LESS DESTINATION_INDEX)
    list(APPEND SOURCE_LIST "${CMAKE_ARGV${ARGI}}")
    math(EXPR ARGI "${ARGI}+1")
endwhile()

set(DESTINATION "${CMAKE_ARGV${DESTINATION_INDEX}}")

# Creating destination if needed
if(NOT EXISTS ${DESTINATION})
    message(DEBUG "Creating directory ${DESTINATION}")
    file(MAKE_DIRECTORY ${DESTINATION})
elseif(NOT IS_DIRECTORY ${DESTINATION})
    message(FATAL_ERROR "cstoolkit_copy Error: Destination \"${DESTINATION}\" already exists and is not a directory")
    return()
endif()

if(NOT SOURCE_LIST)
    # Nothing to copy
    return()
endif()

foreach(SOURCE ${SOURCE_LIST})
    if(EXISTS ${SOURCE})
        if(DIRECTORY_MODE AND NOT IS_DIRECTORY ${SOURCE})
            message(FATAL_ERROR "cstoolkit_copy Error: Source \"${SOURCE}\" is not a directory")
            return()
        elseif(NOT DIRECTORY_MODE AND IS_DIRECTORY ${SOURCE})
            message(FATAL_ERROR "cstoolkit_copy Error: Source \"${SOURCE}\" is not a file")
            return()
        endif()
        list(APPEND FILTERED_SOURCE_LIST ${SOURCE})
    elseif(STOP_ON_ERROR)
        message(FATAL_ERROR "cstoolkit_copy Error: Source \"${SOURCE}\" does not exists")
        return()
    endif()
endforeach()

if(NOT FILTERED_SOURCE_LIST)
    return()
endif()

if(DIRECTORY_MODE)
    foreach(_destdir ${FILTERED_SOURCE_LIST})
        get_filename_component(_lastcomponent "${_destdir}" NAME)
        execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different "${_destdir}" "${DESTINATION}/${_lastcomponent}")
    endforeach( )
else()
    execute_process(COMMAND ${CMAKE_COMMAND} -E copy_if_different ${FILTERED_SOURCE_LIST} "${DESTINATION}")
endif()
