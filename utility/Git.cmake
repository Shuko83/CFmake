find_package(Git QUIET)

if(GIT_EXECUTABLE)
    execute_process(COMMAND "${GIT_EXECUTABLE}" describe --all --dirty --broken --long --always
                    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
                    OUTPUT_VARIABLE CSTOOLKIT_GIT_COMMIT
                    ERROR_QUIET
                    OUTPUT_STRIP_TRAILING_WHITESPACE)
    
    execute_process(COMMAND "${GIT_EXECUTABLE}" rev-parse HEAD
                    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
                    OUTPUT_VARIABLE CSTOOLKIT_GIT_SHA
                    ERROR_QUIET
                    OUTPUT_STRIP_TRAILING_WHITESPACE)

    execute_process(COMMAND "${GIT_EXECUTABLE}" describe --exact-match --tags
                    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
                    OUTPUT_VARIABLE CSTOOLKIT_GIT_TAG
                    ERROR_QUIET
                    OUTPUT_STRIP_TRAILING_WHITESPACE)

    execute_process(COMMAND "${GIT_EXECUTABLE}" rev-parse --abbrev-ref HEAD
                    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
                    OUTPUT_VARIABLE CSTOOLKIT_GIT_BRANCH
                    ERROR_QUIET
                    OUTPUT_STRIP_TRAILING_WHITESPACE)

    execute_process(COMMAND "${GIT_EXECUTABLE}" log -n 1 --pretty=%cD
                    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                    OUTPUT_VARIABLE CSTOOLKIT_GIT_DATE
                    ERROR_QUIET
                    OUTPUT_STRIP_TRAILING_WHITESPACE)

    execute_process(COMMAND "${GIT_EXECUTABLE}" remote get-url origin
                    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
                    OUTPUT_VARIABLE CSTOOLKIT_GIT_URL
                    ERROR_QUIET
                    OUTPUT_STRIP_TRAILING_WHITESPACE)

    execute_process(COMMAND "${GIT_EXECUTABLE}" rev-parse --show-toplevel
                    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
                    OUTPUT_VARIABLE CSTOOLKIT_GIT_RELATIVE_TOP_LEVEL
                    ERROR_QUIET
                    OUTPUT_STRIP_TRAILING_WHITESPACE)

    if(CSTOOLKIT_GIT_BRANCH STREQUAL "HEAD")
        set(CSTOOLKIT_GIT_BRANCH "")
    endif()

    if(CSTOOLKIT_GIT_RELATIVE_TOP_LEVEL)
        cmake_path(RELATIVE_PATH CMAKE_SOURCE_DIR BASE_DIRECTORY "${CSTOOLKIT_GIT_RELATIVE_TOP_LEVEL}" OUTPUT_VARIABLE CSTOOLKIT_GIT_RELATIVE_TOP_LEVEL)
    endif()
endif()
