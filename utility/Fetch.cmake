include(FetchContent)

set(CSTOOLKIT_EXTERNALS "${CMAKE_BINARY_DIR}/externals")

set(CMAKE_PREFIX_PATH ${CSTOOLKIT_EXTERNALS} CACHE PATH "Directories to be searched by find_package()")

set(CSTOOLKIT_ARTIFACTORY_URL "http://artifactory.divst:8081/artifactory")

function(cstoolkit_fetch_artifactory PACKAGE_NAME)
    set(FETCH_OPTIONS)
    set(FETCH_UNIQUE URL VERSION FOLDER NAME)
    set(FETCH_MULTIPLE)
    cmake_parse_arguments(FETCH "${FETCH_OPTIONS}" "${FETCH_UNIQUE}" "${FETCH_MULTIPLE}" ${ARGN})

    if(NOT FETCH_NAME)
        set(FETCH_NAME ${PACKAGE_NAME})
    endif()

    if(NOT FETCH_URL)
        if(NOT FETCH_VERSION)
            message(SEND_ERROR "CSToolkit: cstoolkit_fetch_artifactory: VERSION or URL parameter must be defined.")
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
        FIND_PACKAGE_ARGS GLOBAL
    )

    FetchContent_Populate(${PACKAGE_NAME})
    
    FetchContent_MakeAvailable(${PACKAGE_NAME})
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
