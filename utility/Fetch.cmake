include(FetchContent)

function(cstoolkit_fetch_artifactory FETCH_NAME)
    set(FETCH_OPTIONS)
    set(FETCH_UNIQUE URL VERSION FOLDER ALIAS)
    set(FETCH_MULTIPLE)
    cmake_parse_arguments(FETCH "${FETCH_OPTIONS}" "${FETCH_UNIQUE}" "${FETCH_MULTIPLE}" ${ARGN})

    if(FETCH_ALIAS)
        set(PACKAGE_NAME ${FETCH_ALIAS})
    else()
        set(PACKAGE_NAME ${FETCH_NAME})
    endif()

    if(NOT FETCH_URL)
        if(NOT FETCH_VERSION)
            message(SEND_ERROR "CSToolkit: cstoolkit_fetch_artifactory: VERSION or URL parameter must be defined.")
            return()
        endif()
        if(NOT FETCH_FOLDER)
            set(FETCH_FOLDER "thirdParty")
        endif()
        set(FETCH_URL "${CSTOOLKIT_ARTIFACTORY_URL}/${FETCH_FOLDER}/${FETCH_NAME}/${FETCH_VERSION}/${FETCH_NAME}_${FETCH_VERSION}_${CSTOOLKIT_BUILD_MKSPECS}.7z")
    endif()

    FetchContent_Declare(${PACKAGE_NAME}
        URL "${FETCH_URL}"
        SOURCE_DIR "${CSTOOLKIT_EXTERNALS}/${PACKAGE_NAME}"
        CONFIGURE_COMMAND ""
        BUILD_COMMAND ""
    )

    FetchContent_MakeAvailable(${PACKAGE_NAME})

    find_package(${PACKAGE_NAME} QUIET MODULE GLOBAL)
    if(${${PACKAGE_NAME}_FOUND})
        return()
    endif()

    if(FETCH_ALIAS) # If ALIAS is Provided find_package is REQUIRED and no try with lowercase
        find_package(${PACKAGE_NAME} REQUIRED GLOBAL NO_DEFAULT_PATH PATHS "${CSTOOLKIT_EXTERNALS}/${PACKAGE_NAME}")
    else()
        find_package(${PACKAGE_NAME} QUIET GLOBAL NO_DEFAULT_PATH PATHS "${CSTOOLKIT_EXTERNALS}/${PACKAGE_NAME}")
        if(${${PACKAGE_NAME}_FOUND})
            return()
        endif()

        string(TOLOWER ${PACKAGE_NAME} PACKAGE_NAME_LOWER)
        if(PACKAGE_NAME STREQUAL PACKAGE_NAME_LOWER)
            return()
        endif()

        find_package(${PACKAGE_NAME_LOWER} QUIET MODULE GLOBAL)
        if(${${PACKAGE_NAME}_FOUND})
            return()
        endif()
        
        find_package(${PACKAGE_NAME_LOWER} QUIET GLOBAL NO_DEFAULT_PATH PATHS "${CSTOOLKIT_EXTERNALS}/${PACKAGE_NAME}")
    endif()
endfunction()

function(cstoolkit_download_url URL)
    cmake_path(GET URL STEM LAST_ONLY _FILENAME)
    string(MD5 _HASH ${URL})
    FetchContent_Declare(cstoolkit_download_${_HASH}
        URL "${URL}"
        SOURCE_DIR "${CSTOOLKIT_EXTERNALS}/${_FILENAME}_${_HASH}"
        BINARY_DIR "${CSTOOLKIT_EXTERNALS}/${_FILENAME}_${_HASH}"
        CONFIGURE_COMMAND ""
        BUILD_COMMAND ""
    )
    FetchContent_Populate(cstoolkit_download_${_HASH})
endfunction()
