macro(cstoolkit_fetch_artifactory fetch_artifactory_name)
    set(fetch_artifactory_OPTIONS QT LEGACY)
    set(fetch_artifactory_UNIQUE URL VERSION FOLDER ALIAS)
    set(fetch_artifactory_MULTIPLE)
    cmake_parse_arguments(_fetch_artifactory "${fetch_artifactory_OPTIONS}" "${fetch_artifactory_UNIQUE}" "${fetch_artifactory_MULTIPLE}" ${ARGN})

    if(_fetch_artifactory_URL)
        message(NOTICE ${COLOR_YELLOW_BOLD} "CSToolkit: cstoolkit_fetch_artifactory(${fetch_artifactory_name}): URL parameter deprecated, use cstoolkit_fetch_package(<package_name> <url> <options>)" ${COLOR_RESET})
    endif()
    if(_fetch_artifactory_ALIAS)
        message(NOTICE ${COLOR_YELLOW_BOLD} "CSToolkit: cstoolkit_fetch_artifactory(${fetch_artifactory_name}): ALIAS parameter deprecated, use cstoolkit_fetch_package(<package_name> <url> <options>)" ${COLOR_RESET})
    endif()

    if(NOT _fetch_artifactory_URL AND NOT _fetch_artifactory_VERSION)
        message(SEND_ERROR "CSToolkit: cstoolkit_fetch_artifactory(${fetch_artifactory_name}): Missing mandatory parameter VERSION")
    elseif(_fetch_artifactory_QT AND (NOT QT_VERSION))
        message(SEND_ERROR "CSToolkit: cstoolkit_fetch_artifactory(${fetch_artifactory_name}): Qt was not found, unable to use option QT")
    else()
        if(NOT _fetch_artifactory_URL)
            _cstoolkit_internal_compute_url(_fetch_artifactory_URL ${CSTOOLKIT_ARTIFACTORY_URL} ${fetch_artifactory_name} ${ARGN})
        endif()

        if(_fetch_artifactory_ALIAS)
            set(_fetch_artifactory_package_name ${_fetch_artifactory_ALIAS})
        else()
            set(_fetch_artifactory_package_name ${fetch_artifactory_name})
        endif()

        cstoolkit_fetch_package(${_fetch_artifactory_package_name} ${_fetch_artifactory_URL} ${ARGN})
    endif()
    unset(fetch_artifactory_OPTIONS)
    unset(fetch_artifactory_UNIQUE)
    unset(fetch_artifactory_MULTIPLE)
    unset(_fetch_artifactory_QT)
    unset(_fetch_artifactory_LEGACY)
    unset(_fetch_artifactory_URL)
    unset(_fetch_artifactory_VERSION)
    unset(_fetch_artifactory_FOLDER)
    unset(_fetch_artifactory_ALIAS)
    unset(_fetch_artifactory_package_name)
endmacro()

macro(cstoolkit_fetch_nexus fetch_nexus_name)
    set(fetch_nexus_OPTIONS QT LEGACY ITAR)
    set(fetch_nexus_UNIQUE VERSION FOLDER ALIAS URL)
    set(fetch_nexus_MULTIPLE)
    cmake_parse_arguments(_fetch_nexus "${fetch_nexus_OPTIONS}" "${fetch_nexus_UNIQUE}" "${fetch_nexus_MULTIPLE}" ${ARGN})

    if(_fetch_artifactory_URL)
        message(NOTICE ${COLOR_YELLOW_BOLD} "CSToolkit: cstoolkit_fetch_artifactory(${fetch_artifactory_name}): URL parameter deprecated, use cstoolkit_fetch_package(<package_name> <url> <options>)" ${COLOR_RESET})
    endif()
    if(_fetch_artifactory_ALIAS)
        message(NOTICE ${COLOR_YELLOW_BOLD} "CSToolkit: cstoolkit_fetch_nexus(${fetch_artifactory_name}): ALIAS parameter deprecated, use cstoolkit_fetch_package(<package_name> <url> <options>)" ${COLOR_RESET})
    endif()

    if(NOT _fetch_nexus_VERSION)
        message(SEND_ERROR "CSToolkit: cstoolkit_fetch_nexus(${fetch_artifactory_name}): Missing mandatory parameter VERSION")
    elseif(_fetch_nexus_QT AND (NOT QT_VERSION))
        message(SEND_ERROR "CSToolkit: cstoolkit_fetch_nexus(${fetch_artifactory_name}): Qt was not found, unable to use option QT")
    else()
        # Calculating URL
        if(NOT _fetch_nexus_FOLDER)
            set(_fetch_nexus_FOLDER "thirdParty")
        endif()

        if(_fetch_nexus_ITAR)
            set(_fetch_nexus_root_url "${CSTOOLKIT_NEXUS_DR_URL}")
        else()
            set(_fetch_nexus_root_url "${CSTOOLKIT_NEXUS_ITAR_URL}")
        endif()

        _cstoolkit_internal_compute_url(_fetch_nexus_URL ${_fetch_nexus_root_url} ${fetch_nexus_name} ${ARGN})

        if(_fetch_nexus_ALIAS)
            set(_fetch_nexus_package_name ${_fetch_nexus_ALIAS})
        else()
            set(_fetch_nexus_package_name ${fetch_nexus_name})
        endif()

        cstoolkit_fetch_package(${_fetch_nexus_package_name} ${_fetch_nexus_URL} ${ARGN})
    endif()
    unset(fetch_nexus_OPTIONS)
    unset(fetch_nexus_UNIQUE)
    unset(fetch_nexus_MULTIPLE)
    unset(_fetch_nexus_QT)
    unset(_fetch_nexus_LEGACY)
    unset(_fetch_nexus_URL)
    unset(_fetch_nexus_VERSION)
    unset(_fetch_nexus_FOLDER)
    unset(_fetch_nexus_ALIAS)
    unset(_fetch_nexus_package_name)
endmacro()

macro(cstoolkit_fetch_package fetch_package_name fetch_package_url)
    set(fetch_package_OPTIONS LEGACY MODULE CONFIG NO_MODULE)
    set(fetch_package_UNIQUE URL)
    set(fetch_package_MULTIPLE)
    cmake_parse_arguments(_fetch_params "${fetch_package_OPTIONS}" "${fetch_package_UNIQUE}" "${fetch_package_MULTIPLE}" ${fetch_package_url};${ARGN})

    # URL keyword optionnal
    if(NOT _fetch_params_URL)
        set(_fetch_params_URL "${fetch_package_url}")
    endif()

    set(download_and_extract_params)
    if(_fetch_params_LEGACY)
        list(APPEND download_and_extract_params LEGACY)
    endif()

    set(find_package_params)
    if(_fetch_params_MODULE)
        list(APPEND find_package_params MODULE)
    elseif(_fetch_params_CONFIG OR _fetch_params_NO_MODULE)
        list(APPEND find_package_params CONFIG)
    elseif(_fetch_params_LEGACY)
        list(APPEND find_package_params CONFIG)
    endif()

    if(CSTOOLKIT_FETCH_${fetch_package_name}_URL)
        if(NOT CSTOOLKIT_FETCH_${fetch_package_name}_URL STREQUAL "${_fetch_params_URL}")
            message(NOTICE ${COLOR_GREY} "CSToolkit: Package ${fetch_package_name}: Different URL from ${CMAKE_CURRENT_LIST_FILE} ignored. Using definition ${CSTOOLKIT_FETCH_${fetch_package_name}_LIST_FILE}" ${COLOR_RESET})
        endif()
    else()
        set_target_properties(CSTOOLKIT PROPERTIES FETCH_${fetch_package_name}_URL "${_fetch_params_URL}")
        set(CSTOOLKIT_FETCH_${fetch_package_name}_URL "${_fetch_params_URL}")
        set(CSTOOLKIT_FETCH_${fetch_package_name}_LIST_FILE "${CMAKE_CURRENT_LIST_FILE}")
        cstoolkit_download_and_extract_package("${fetch_package_name}" "${_fetch_params_URL}" ${download_and_extract_params})
        _cstoolkit_internal_find_package("${fetch_package_name}" "${CSTOOLKIT_EXTERNALS}/${fetch_package_name}" ${find_package_params})
    endif()
endmacro()

function(cstoolkit_download_and_extract_package fetch_package_name fetch_package_url)
    set(FETCH_OPTIONS LEGACY)
    set(FETCH_UNIQUE)
    set(FETCH_MULTIPLE)
    cmake_parse_arguments(PARSE_ARGV 2 fetch_package "${FETCH_OPTIONS}" "${FETCH_UNIQUE}" "${FETCH_MULTIPLE}")

    set(_fetch_package_download_dir "${CSTOOLKIT_DOWNLOAD_BASE_DIR}/${fetch_package_name}")
    set(_fetch_package_extract_dir "${CSTOOLKIT_EXTERNALS}/${fetch_package_name}")
    cmake_path(GET fetch_package_url FILENAME _fetch_package_filename)

    # Reconfigure if file modified
    set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS "${_fetch_package_download_dir}/download.stamp")

    # Get previous URL
    if(EXISTS "${_fetch_package_download_dir}/download.stamp")
        file(READ "${_fetch_package_download_dir}/download.stamp" _fetch_package_previous_url)
    endif()

    # Download if previous url different
    if((NOT _fetch_package_previous_url STREQUAL "${fetch_package_url}") OR (NOT EXISTS "${_fetch_package_download_dir}/${_fetch_package_filename}"))
        cstoolkit_start_timer(CSTOOLKIT_FETCH_TIMER)
        message(STATUS "CSToolkit: Download package ${fetch_package_name}")

        _cstoolkit_internal_download("${fetch_package_url}" "${_fetch_package_download_dir}")

        file(WRITE "${_fetch_package_download_dir}/download.stamp" "${fetch_package_url}")

        cstoolkit_end_timer(CSTOOLKIT_FETCH_TIMER CSTOOLKIT_FETCH_ELAPSED)
        message(STATUS "CSToolkit: Download package ${fetch_package_name} done (${CSTOOLKIT_FETCH_ELAPSED}s)")
    endif()

    # Extract if previous url different or if extract dir doesn't exists
    if((NOT _fetch_package_previous_url STREQUAL "${fetch_package_url}") OR (NOT EXISTS "${_fetch_package_extract_dir}"))
        cstoolkit_start_timer(CSTOOLKIT_FETCH_TIMER)
        message(STATUS "CSToolkit: Extract package ${fetch_package_name}")

        _cstoolkit_internal_extract("${_fetch_package_download_dir}/${_fetch_package_filename}" "${_fetch_package_extract_dir}")

        if(fetch_package_LEGACY)
            if(EXISTS "${_fetch_package_extract_dir}/${fetch_package_name}Config.cmake")
                message(STATUS "CSToolkit: Overwriting ${fetch_package_name}Config.cmake with generated legacy support Config file.")
            endif()
            configure_file(${CSTOOLKIT_ROOT_DIR}/templates/FetchLegacyConfig.${CSTOOLKIT_HOST_PLATFORM}.in "${_fetch_package_extract_dir}/${fetch_package_name}Config.cmake" @ONLY)
        endif()

        cstoolkit_end_timer(CSTOOLKIT_FETCH_TIMER CSTOOLKIT_FETCH_ELAPSED)
        message(STATUS "CSToolkit: Extract package ${fetch_package_name} done (${CSTOOLKIT_FETCH_ELAPSED}s)")
    endif()
endfunction()

function(_cstoolkit_internal_compute_url url_var url_root url_package_name)
    set(_compute_url_OPTIONS QT LEGACY)
    set(_compute_url_UNIQUE VERSION FOLDER)
    set(_compute_url_MULTIPLE)
    cmake_parse_arguments(PARSE_ARGV 3 _compute_url "${_compute_url_OPTIONS}" "${_compute_url_UNIQUE}" "${_compute_url_MULTIPLE}")

    if(NOT _compute_url_FOLDER)
        set(_compute_url_FOLDER "thirdParty")
    endif()
    
    set(${url_var} "${url_root}/${_compute_url_FOLDER}/${url_package_name}/${_compute_url_VERSION}/${url_package_name}_${_compute_url_VERSION}")

    if(_compute_url_QT)
        if(_compute_url_LEGACY)
            set(${url_var} "${${url_var}}_${CSTOOLKIT_BUILD_MKSPECS}-QT${QT_VERSION}")
        else()
            set(${url_var} "${${url_var}}_${CSTOOLKIT_BUILD_MKSPECS_QT}")
        endif()
    else()
        set(${url_var} "${${url_var}}_${CSTOOLKIT_BUILD_MKSPECS}")
    endif()

    if(_compute_url_LEGACY)
        set(${url_var} "${${url_var}}.zip")
    else()
        set(${url_var} "${${url_var}}.7z")
    endif()

    set(${url_var} ${${url_var}} PARENT_SCOPE)
endfunction()

macro(_cstoolkit_internal_find_package _internal_find_package_name _internal_find_package_dir)
    # PARAMETER 3: MODE: MODULE / CONFIG|NO_MODULE
    set(${_internal_find_package_name}_ROOT "${_internal_find_package_dir}" CACHE INTERNAL "Defined by call to cstoolkit_fetch_XXX")
    set(FETCH_PACKAGE_HINTS "${${_internal_find_package_name}_ROOT}")

    if("${ARGV2}" STREQUAL "MODULE")
        find_package(${_internal_find_package_name} QUIET MODULE GLOBAL)
    elseif("${ARGV2}" STREQUAL "CONFIG" OR "${ARGV2}" STREQUAL "NO_MODULE")
        find_package(${_internal_find_package_name} QUIET CONFIG GLOBAL NO_DEFAULT_PATH HINTS "${FETCH_PACKAGE_HINTS}")
    elseif(CSTOOLKIT_FETCH_PACKAGE_PREFER_MODULE)
        find_package(${_internal_find_package_name} QUIET MODULE GLOBAL)
        if(NOT ${${_internal_find_package_name}_FOUND})
            find_package(${_internal_find_package_name} QUIET CONFIG GLOBAL NO_DEFAULT_PATH HINTS "${FETCH_PACKAGE_HINTS}")
        endif()
    else()
        find_package(${_internal_find_package_name} QUIET CONFIG GLOBAL NO_DEFAULT_PATH HINTS "${FETCH_PACKAGE_HINTS}")
        if(NOT ${${_internal_find_package_name}_FOUND})
            find_package(${_internal_find_package_name} QUIET MODULE GLOBAL)
        endif()
    endif()
    unset(FETCH_PACKAGE_HINTS)
endmacro()

function(_cstoolkit_internal_download _internal_download_url _internal_download_dir)
    file(REMOVE_RECURSE "${_internal_download_dir}/${_internal_download_filename}")
    file(MAKE_DIRECTORY "${_internal_download_dir}")
    cmake_path(GET _internal_download_url FILENAME _internal_download_filename)

    # Autentication method
    # Use FIND instead of MATCHES because URL contains "."
    string(FIND "${_internal_download_url}" "${CSTOOLKIT_NEXUS_DR_URL}" NEXUS_DR_INDEX)
    string(FIND "${_internal_download_url}" "${CSTOOLKIT_NEXUS_ITAR_URL}" NEXUS_ITAR_INDEX)
    
    if(NEXUS_DR_INDEX EQUAL 0)
        if(DEFINED ENV{NEXUS_DR_AUTH})
            set(_internal_download_header "HTTPHEADER;Authorization: Basic $ENV{NEXUS_DR_AUTH}")
        endif()
    elseif(NEXUS_ITAR_INDEX EQUAL 0)
        if(DEFINED ENV{NEXUS_ITAR_AUTH})
            set(_internal_download_header "HTTPHEADER;Authorization: Basic $ENV{NEXUS_ITAR_AUTH}")
        endif()
    endif()

    # Add prompt to get credential if Autentication failed.

    # In part copied from FetchContect download-<name>-populate.cmake
    file(DOWNLOAD "${_internal_download_url}" "${_internal_download_dir}/${_internal_download_filename}"
        ${_internal_download_header}
        # no TIMEOUT
        # no INACTIVITY_TIMEOUT
        STATUS status
        LOG log
    )
    list(GET status 0 status_code)
    list(GET status 1 status_string)

    if(NOT status_code EQUAL 0)
        file(REMOVE_RECURSE "${_internal_download_dir}")
        message(FATAL_ERROR "CSToolkit: Downloading '${_internal_download_url}' failed
STATUS: ${status_code} ${status_string}
        --- LOG BEGIN ---
${log}        --- LOG END ---
")
    endif()
endfunction()

function(_cstoolkit_internal_extract _internal_extract_file _internal_extract_dir)
    cmake_path(GET _internal_extract_file PARENT_PATH _internal_extract_temp_dir)
    set(_internal_extract_temp_dir "${_internal_extract_temp_dir}/tmp")
    file(REMOVE_RECURSE "${_internal_extract_temp_dir}")
    file(MAKE_DIRECTORY "${_internal_extract_temp_dir}")
    execute_process(COMMAND ${CMAKE_COMMAND} -E tar x "${_internal_extract_file}" --touch
        WORKING_DIRECTORY "${_internal_extract_temp_dir}"
        RESULT_VARIABLE _fetch_package_extract_rv
        OUTPUT_VARIABLE _fetch_package_extract_output
        ERROR_VARIABLE  _fetch_package_extract_output
    )

    if(NOT _fetch_package_extract_rv EQUAL 0)
        file(REMOVE_RECURSE "${_internal_extract_temp_dir}")
        message(FATAL_ERROR "CSToolkit: Extracting of '${_internal_extract_file}' failed
${_fetch_package_extract_output}")
        return()
    endif()

    # Analyze what came out of the tar file:
    file(GLOB contents "${_internal_extract_temp_dir}/*")
    if(contents STREQUAL "")
        file(REMOVE_RECURSE "${_internal_extract_temp_dir}")
        file(SIZE "_internal_extract_file}" file_size)
        _cstoolkit_internal_format_size(file_size)
        message(FATAL_ERROR "CSToolkit: Extracting of '${_internal_extract_file}' failed
Output directory empty
File size: ${file_size}")
        return()
    endif()

    file(MAKE_DIRECTORY "${_internal_extract_dir}")
    file(REMOVE_RECURSE "${_internal_extract_dir}")

    # Remove top level directory if exists
    list(LENGTH contents n)
    if(NOT n EQUAL 1 OR NOT IS_DIRECTORY "${contents}")
        set(contents "${_internal_extract_temp_dir}")
    endif()
    file(RENAME "${contents}" "${_internal_extract_dir}" RESULT _fetch_package_extract_rv)
    file(REMOVE_RECURSE "${_internal_extract_temp_dir}")
    if(NOT _fetch_package_extract_rv EQUAL 0)
        file(REMOVE_RECURSE "${_internal_extract_dir}")
        message(FATAL_ERROR "CSToolkit: Extracting of '${_internal_extract_file}' failed
${_fetch_package_extract_rv}")
        return()
    endif()
endfunction()

function(cstoolkit_download_url URL OUTPUT_VAR)
    set(options NO_EXTRACT)
    set(one_value_args)
    set(multi_value_args)
    cmake_parse_arguments(PARSE_ARGV 2 DOWNLOAD "${options}" "${one_value_args}" "${multi_value_args}")

    cmake_path(GET URL STEM LAST_ONLY _FILENAME)
    cmake_path(GET URL FILENAME _FULLFILENAME)
    string(MD5 _HASH ${URL})
    string(SUBSTRING ${_HASH} 0 8 _HASH)

    if(DOWNLOAD_NO_EXTRACT)
        set(download_dir "${CSTOOLKIT_EXTERNALS}/${_FILENAME}_${_HASH}")
        set(${OUTPUT_VAR} "${CSTOOLKIT_EXTERNALS}/${_FILENAME}_${_HASH}/${_FULLFILENAME}")
    else()
        set(download_dir "${CSTOOLKIT_DOWNLOAD_BASE_DIR}/${_FILENAME}_${_HASH}")
        set(${OUTPUT_VAR} "${CSTOOLKIT_EXTERNALS}/${_FILENAME}_${_HASH}")
    endif()

    set(${OUTPUT_VAR} ${${OUTPUT_VAR}} PARENT_SCOPE)

    if(NOT EXISTS "${download_dir}/${_FULLFILENAME}")
        cstoolkit_start_timer(CSTOOLKIT_DOWNLOAD_TIMER)
        message(STATUS "CSToolkit: Download ${URL}")

        _cstoolkit_internal_download("${URL}" "${download_dir}")

        cstoolkit_end_timer(CSTOOLKIT_DOWNLOAD_TIMER CSTOOLKIT_DOWNLOAD_ELAPSED)
        message(STATUS "CSToolkit: Download ${URL} done (${CSTOOLKIT_FETCH_ELAPSED}s)")
    endif()

    if((NOT DOWNLOAD_NO_EXTRACT) AND (NOT EXISTS "${${OUTPUT_VAR}}"))
        cstoolkit_start_timer(CSTOOLKIT_DOWNLOAD_TIMER)
        message(STATUS "CSToolkit: Extract ${URL}")

        _cstoolkit_internal_extract("${download_dir}/${_FULLFILENAME}" "${${OUTPUT_VAR}}")

        cstoolkit_end_timer(CSTOOLKIT_DOWNLOAD_TIMER CSTOOLKIT_DOWNLOAD_ELAPSED)
        message(STATUS "CSToolkit: Extract ${URL} done (${CSTOOLKIT_FETCH_ELAPSED}s)")
    endif()
endfunction()

function(_cstoolkit_internal_format_size _internal_size)
    if("${${file_size}}" GREATER 1073741824)
        math(EXPR size_gb_x10 "${${file_size}} * 10 / 1073741824")
        math(EXPR whole_part "${size_gb_x10} / 10")
        math(EXPR decimal_part "${size_gb_x10} % 10")
        set(${file_size} "${whole_part}.${decimal_part} GB" PARENT_SCOPE)
    elseif("${${file_size}}" GREATER 1048576)
        math(EXPR size_mb_x10 "${${file_size}} * 10 / 1048576")
        math(EXPR whole_part "${size_mb_x10} / 10")
        math(EXPR decimal_part "${size_mb_x10} % 10")
        set(${file_size} "${whole_part}.${decimal_part} MB" PARENT_SCOPE)
    elseif("${${file_size}}" GREATER 1024)
        math(EXPR size_kb_x10 "${${file_size}} * 10 / 1024")
        math(EXPR whole_part "${size_kb_x10} / 10")
        math(EXPR decimal_part "${size_kb_x10} % 10")
        set(${file_size} "${whole_part}.${decimal_part} KB" PARENT_SCOPE)
    else()
        set(${file_size} "${${file_size}} bytes" PARENT_SCOPE)
    endif()
endfunction()

macro(cstoolkit_fetch_dependency fetch_dependency_name fetch_dependency_url)
    if(CSTOOLKIT_FETCH_DEPENDENCY)
        if(CSTOOLKIT_FETCH_${fetch_dependency_name}_URL)
            if(NOT CSTOOLKIT_FETCH_${fetch_dependency_name}_URL STREQUAL "${fetch_dependency_url}")
                message(NOTICE ${COLOR_GREY} "CSToolkit: Package ${fetch_dependency_name}: Different URL from ${CMAKE_CURRENT_LIST_FILE} ignored. Using definition ${CSTOOLKIT_FETCH_${fetch_dependency_name}_LIST_FILE}" ${COLOR_RESET})
            endif()
        else()
            # no set_target_property, we dont store urls of dependencies
            set(CSTOOLKIT_FETCH_${fetch_dependency_name}_URL "${fetch_dependency_url}")
            set(CSTOOLKIT_FETCH_${fetch_dependency_name}_LIST_FILE "${CMAKE_CURRENT_LIST_FILE}")
            cstoolkit_download_and_extract_package("${fetch_dependency_name}" "${fetch_dependency_url}" ${ARGN})
            _cstoolkit_internal_find_package("${fetch_dependency_name}" "${CSTOOLKIT_EXTERNALS}/${fetch_dependency_name}")
        endif()
    endif()
endmacro()
