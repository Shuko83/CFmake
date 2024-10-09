include(FetchContent)

macro(cstoolkit_fetch_artifactory FETCH_NAME)
    set(FETCH_OPTIONS QT LEGACY)
    set(FETCH_UNIQUE URL VERSION FOLDER ALIAS)
    set(FETCH_MULTIPLE)
    cmake_parse_arguments(FETCH "${FETCH_OPTIONS}" "${FETCH_UNIQUE}" "${FETCH_MULTIPLE}" ${ARGN})

    if(FETCH_ALIAS)
        set(PACKAGE_NAME ${FETCH_ALIAS})
    else()
        set(PACKAGE_NAME ${FETCH_NAME})
    endif()

    if(NOT FETCH_URL AND NOT FETCH_VERSION)
        message(SEND_ERROR "CSToolkit: cstoolkit_fetch_artifactory: VERSION or URL parameter must be defined.")
    else()
        if(NOT FETCH_URL)
            if(NOT FETCH_FOLDER)
                set(FETCH_FOLDER "thirdParty")
            endif()
            set(FETCH_URL "${CSTOOLKIT_ARTIFACTORY_URL}/${FETCH_FOLDER}/${FETCH_NAME}/${FETCH_VERSION}/${FETCH_NAME}_${FETCH_VERSION}")
            if(FETCH_QT)
                if(FETCH_LEGACY)
                    set(FETCH_URL "${FETCH_URL}_${CSTOOLKIT_BUILD_MKSPECS}-QT${QT_VERSION}")
                else()
                    set(FETCH_URL "${FETCH_URL}_${CSTOOLKIT_BUILD_MKSPECS_QT}")
                endif()
            else()
                set(FETCH_URL "${FETCH_URL}_${CSTOOLKIT_BUILD_MKSPECS}")
            endif()
            if(FETCH_LEGACY)
                set(FETCH_URL "${FETCH_URL}.zip")
            else()
                set(FETCH_URL "${FETCH_URL}.7z")
            endif()
        endif()

        FetchContent_Declare(${PACKAGE_NAME}
            URL "${FETCH_URL}"
            SOURCE_DIR "${CSTOOLKIT_EXTERNALS}/${PACKAGE_NAME}"
        )

        FetchContent_MakeAvailable(${PACKAGE_NAME})

        string(TOUPPER "${PACKAGE_NAME}" PACKAGE_NAME_UPPER)

        if(FETCH_LEGACY)
            if(EXISTS "${CSTOOLKIT_EXTERNALS}/${PACKAGE_NAME}/${PACKAGE_NAME}Config.cmake" AND NOT CSTOOLKIT_FECTH_ARTIFACTORY_LEGACY_${PACKAGE_NAME_UPPER})
                message(STATUS "CSToolkit: Overwriting ${PACKAGE_NAME}Config.cmake with generated legacy suport Config file.")
            endif()
            set(CSTOOLKIT_FECTH_ARTIFACTORY_LEGACY_${PACKAGE_NAME_UPPER} TRUE CACHE INTERNAL "Legacy mode used for ${PACKAGE_NAME}")
            configure_file(${CSTOOLKIT_ROOT_DIR}/templates/FetchLegacyConfig.in "${CSTOOLKIT_EXTERNALS}/${PACKAGE_NAME}/${PACKAGE_NAME}Config.cmake" @ONLY)
        else()
            set(CSTOOLKIT_FECTH_ARTIFACTORY_LEGACY_${PACKAGE_NAME_UPPER} FALSE CACHE INTERNAL "Legacy mode used for ${PACKAGE_NAME}")
        endif()

        find_package(${PACKAGE_NAME} QUIET MODULE GLOBAL)
        if(NOT ${${PACKAGE_NAME}_FOUND})
            if(FETCH_ALIAS) # If ALIAS is Provided find_package is REQUIRED and no try with lowercase
                find_package(${PACKAGE_NAME} REQUIRED GLOBAL NO_DEFAULT_PATH PATHS "${CSTOOLKIT_EXTERNALS}/${PACKAGE_NAME}")
            else()
                find_package(${PACKAGE_NAME} QUIET GLOBAL NO_DEFAULT_PATH PATHS "${CSTOOLKIT_EXTERNALS}/${PACKAGE_NAME}")
                if(NOT ${${PACKAGE_NAME}_FOUND})
                    string(TOLOWER ${PACKAGE_NAME} PACKAGE_NAME_LOWER)
                    if(NOTPACKAGE_NAME STREQUAL PACKAGE_NAME_LOWER)
                        find_package(${PACKAGE_NAME_LOWER} QUIET MODULE GLOBAL)
                        if(NOT ${${PACKAGE_NAME}_FOUND})
                            find_package(${PACKAGE_NAME_LOWER} QUIET GLOBAL NO_DEFAULT_PATH PATHS "${CSTOOLKIT_EXTERNALS}/${PACKAGE_NAME}")
                        endif()
                    endif()
                endif()
            endif()
        endif()
    endif()
endmacro()

function(cstoolkit_download_url URL OUTPUT_VAR)
    set(options NO_EXTRACT)
    set(one_value_args)
    set(multi_value_args)
    cmake_parse_arguments(DOWNLOAD "${options}" "${one_value_args}" "${multi_value_args}" ${ARGN})

    cmake_path(GET URL STEM LAST_ONLY _FILENAME)
    string(MD5 _HASH ${URL})
    string(SUBSTRING ${_HASH} 0 8 _HASH)
    FetchContent_Declare(cstoolkit_dl_${_HASH}
        URL "${URL}"
        SOURCE_DIR "${CSTOOLKIT_EXTERNALS}/${_FILENAME}_${_HASH}"
        DOWNLOAD_NO_EXTRACT ${DOWNLOAD_NO_EXTRACT}
    )

    FetchContent_MakeAvailable(cstoolkit_dl_${_HASH})

    if(DOWNLOAD_NO_EXTRACT)
        cmake_path(GET URL FILENAME _FULLFILENAME)
        set(${OUTPUT_VAR} "${CSTOOLKIT_EXTERNALS}/${_FILENAME}_${_HASH}/${_FULLFILENAME}")
    else()
        set(${OUTPUT_VAR} "${CSTOOLKIT_EXTERNALS}/${_FILENAME}_${_HASH}")
    endif()

    set(${OUTPUT_VAR} ${${OUTPUT_VAR}} PARENT_SCOPE)
endfunction()
