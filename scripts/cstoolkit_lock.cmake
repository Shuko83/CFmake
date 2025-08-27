set(USAGE "${CMAKE_COMMAND} -P cstoolkit_lock.cmake -- <lockfile> <command> [args...]")

set(ARGI 0)
while(NOT "${CMAKE_ARGV${ARGI}}" STREQUAL "--" AND ${ARGI} LESS ${CMAKE_ARGC})
    math(EXPR ARGI "${ARGI}+1")
endwhile()

math(EXPR LOCKFILE_INDEX "${ARGI}+1")
math(EXPR COMMAND_INDEX "${ARGI}+2")
math(EXPR ARGC "${CMAKE_ARGC}-1")

if(ARGC LESS COMMAND_INDEX)
    message(FATAL_ERROR "cstoolkit_copy Error: Missing arguments\n"
                        "Usage: ${USAGE}")
    return()
endif()

set(LOCKFILE "${CMAKE_ARGV${LOCKFILE_INDEX}}")
set(COMMAND "${CMAKE_ARGV${COMMAND_INDEX}}")

set(ARGS "")
math(EXPR ARGS_INDEX "${ARGI}+3")

if(ARGC GREATER_EQUAL ARGS_INDEX)
    foreach(i RANGE ${ARGS_INDEX} ${ARGC})
        list(APPEND ARGS "${CMAKE_ARGV${i}}")
    endforeach()
endif()

file(LOCK "${LOCKFILE}" TIMEOUT 600) # TIMEOUT 10mins

execute_process(COMMAND ${COMMAND} ${ARGS} RESULT_VARIABLE RESULT)

file(LOCK "${LOCKFILE}" RELEASE)

if(CMAKE_VERSION VERSION_LESS "3.29")
    if(NOT RESULT EQUAL 0)
        if(RESULT MATCHES "^-?[0-9]+$")
            message(FATAL_ERROR "Error: \"${COMMAND} ${ARGS}\": Return ${RESULT}")
        else()
            message("Error: \"${COMMAND} ${ARGS}\": ${RESULT}")
        endif()
    endif()
else()
    if(NOT RESULT MATCHES "^-?[0-9]+$")
        message("Error: \"${COMMAND} ${ARGS}\": ${RESULT}")
        set(RESULT -1)
    endif()

    # --- Exit with the same code as the command ---
    cmake_language(EXIT ${RESULT})
endif()