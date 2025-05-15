find_package(Git QUIET)

if(GIT_EXECUTABLE)
    execute_process(COMMAND ${GIT_EXECUTABLE} describe --all --dirty --broken --long --always
                    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                    OUTPUT_VARIABLE CSTOOLKIT_GIT_COMMIT
                    ERROR_QUIET)
    
    execute_process(COMMAND ${GIT_EXECUTABLE} rev-parse HEAD
                    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                    OUTPUT_VARIABLE CSTOOLKIT_GIT_SHA
                    ERROR_QUIET)

    execute_process(COMMAND ${GIT_EXECUTABLE} describe --exact-match --tags
                    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                    OUTPUT_VARIABLE CSTOOLKIT_GIT_TAG
                    ERROR_QUIET)

    execute_process(COMMAND ${GIT_EXECUTABLE} rev-parse --abbrev-ref HEAD
                    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                    OUTPUT_VARIABLE CSTOOLKIT_GIT_BRANCH
                    ERROR_QUIET)

    execute_process(COMMAND ${GIT_EXECUTABLE} log -n 1 --pretty=%cD
                    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                    OUTPUT_VARIABLE CSTOOLKIT_GIT_DATE
                    ERROR_QUIET)

    execute_process(COMMAND ${GIT_EXECUTABLE} remote get-url origin
                    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                    OUTPUT_VARIABLE CSTOOLKIT_GIT_URL
                    ERROR_QUIET)

    string(STRIP "${CSTOOLKIT_GIT_COMMIT}" CSTOOLKIT_GIT_COMMIT)
    string(STRIP "${CSTOOLKIT_GIT_SHA}" CSTOOLKIT_GIT_SHA)
    string(STRIP "${CSTOOLKIT_GIT_TAG}" CSTOOLKIT_GIT_TAG)
    string(STRIP "${CSTOOLKIT_GIT_BRANCH}" CSTOOLKIT_GIT_BRANCH)
    string(STRIP "${CSTOOLKIT_GIT_DATE}" CSTOOLKIT_GIT_DATE)
    string(STRIP "${CSTOOLKIT_GIT_URL}" CSTOOLKIT_GIT_URL)

    if(CSTOOLKIT_GIT_BRANCH STREQUAL "HEAD")
        set(CSTOOLKIT_GIT_BRANCH "")
    endif()
endif()
