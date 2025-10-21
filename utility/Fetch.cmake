macro(cstoolkit_fetch_artifactory fetch_artifactory_name)
    set(fetch_artifactory_OPTIONS QT LEGACY)
    set(fetch_artifactory_UNIQUE URL VERSION FOLDER ALIAS MKSPECS)
    set(fetch_artifactory_MULTIPLE)
    cmake_parse_arguments(_fetch_artifactory "${fetch_artifactory_OPTIONS}" "${fetch_artifactory_UNIQUE}" "${fetch_artifactory_MULTIPLE}" ${ARGN})

    if(_fetch_artifactory_URL)
        message(NOTICE ${COLOR_YELLOW_BOLD} "CSToolkit: cstoolkit_fetch_artifactory(${fetch_artifactory_name}): URL parameter deprecated, use cstoolkit_fetch_package(<package_name> URL <url> <options>)" ${COLOR_RESET})
    endif()
    if(_fetch_artifactory_ALIAS)
        message(NOTICE ${COLOR_YELLOW_BOLD} "CSToolkit: cstoolkit_fetch_artifactory(${fetch_artifactory_name}): ALIAS parameter deprecated, normalize your package" ${COLOR_RESET})
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
    unset(_fetch_artifactory_MKSPECS)
    unset(_fetch_artifactory_package_name)
endmacro()

macro(cstoolkit_fetch_nexus fetch_nexus_name)
    set(fetch_nexus_OPTIONS QT LEGACY ITAR)
    set(fetch_nexus_UNIQUE URL VERSION FOLDER ALIAS MKSPECS)
    set(fetch_nexus_MULTIPLE)
    cmake_parse_arguments(_fetch_nexus "${fetch_nexus_OPTIONS}" "${fetch_nexus_UNIQUE}" "${fetch_nexus_MULTIPLE}" ${ARGN})

    if(_fetch_nexus_URL)
        message(NOTICE ${COLOR_YELLOW_BOLD} "CSToolkit: cstoolkit_fetch_nexus(${fetch_nexus_name}): URL parameter deprecated, use cstoolkit_fetch_package(<package_name> URL <url> <options>)" ${COLOR_RESET})
    endif()
    if(_fetch_nexus_ALIAS)
        message(NOTICE ${COLOR_YELLOW_BOLD} "CSToolkit: cstoolkit_fetch_nexus(${fetch_nexus_name}): ALIAS parameter deprecated, normalize your package" ${COLOR_RESET})
    endif()

    if(NOT _fetch_nexus_URL AND NOT _fetch_nexus_VERSION)
        message(SEND_ERROR "CSToolkit: cstoolkit_fetch_nexus(${fetch_nexus_name}): Missing mandatory parameter VERSION")
    elseif(_fetch_nexus_QT AND (NOT QT_VERSION))
        message(SEND_ERROR "CSToolkit: cstoolkit_fetch_nexus(${fetch_nexus_name}): Qt was not found, unable to use option QT")
    else()
        if(_fetch_nexus_ITAR)
            set(_fetch_nexus_root_url "${CSTOOLKIT_NEXUS_ITAR_URL}")
        else()
            set(_fetch_nexus_root_url "${CSTOOLKIT_NEXUS_DR_URL}")
        endif()

        if(NOT _fetch_nexus_URL)
            _cstoolkit_internal_compute_url(_fetch_nexus_URL ${_fetch_nexus_root_url} ${fetch_nexus_name} ${ARGN})
        endif()

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
    unset(_fetch_nexus_MKSPECS)
    unset(_fetch_nexus_package_name)
endmacro()

macro(cstoolkit_fetch_package fetch_package_name fetch_package_url)
    set(fetch_package_OPTIONS LEGACY MODULE CONFIG NO_MODULE GIT_SRC GIT_BUILD CODX)
    set(fetch_package_UNIQUE URL)
    set(fetch_package_MULTIPLE)
    cmake_parse_arguments(_fetch_params "${fetch_package_OPTIONS}" "${fetch_package_UNIQUE}" "${fetch_package_MULTIPLE}" ${fetch_package_url};${ARGN})

    # URL keyword optionnal
    if(NOT _fetch_params_URL)
        set(_fetch_params_URL "${fetch_package_url}")
    endif()

    set(fetch_dependencies_params "")

    set(download_and_extract_params)
    if(_fetch_params_LEGACY)
        list(APPEND download_and_extract_params LEGACY)
        string(APPEND fetch_dependencies_params " LEGACY")
    endif()
    if(_fetch_params_CODX)
        list(APPEND download_and_extract_params CODX)
        string(APPEND fetch_dependencies_params " CODX")
    endif()

    if(_fetch_params_GIT_SRC OR _fetch_params_GIT_BUILD)
        list(APPEND download_and_extract_params "GIT_SRC")
    endif()

    if(_fetch_params_GIT_BUILD)
        list(APPEND download_and_extract_params "GIT_BUILD")
    endif()

    set(find_package_params)
    if(_fetch_params_MODULE)
        list(APPEND find_package_params MODULE)
        string(APPEND fetch_dependencies_params " MODULE")
    elseif(_fetch_params_CONFIG OR _fetch_params_NO_MODULE)
        list(APPEND find_package_params CONFIG)
        string(APPEND fetch_dependencies_params " CONFIG")
    elseif(_fetch_params_LEGACY OR _fetch_params_CODX)
        list(APPEND find_package_params CONFIG)
    endif()

    if(CSTOOLKIT_FETCH_${fetch_package_name}_URL)
        if(NOT CSTOOLKIT_FETCH_${fetch_package_name}_URL STREQUAL "${_fetch_params_URL}")
            message(NOTICE ${COLOR_GREY} "CSToolkit: Package ${fetch_package_name}: Different URL from ${CMAKE_CURRENT_LIST_FILE} ignored. Using definition ${CSTOOLKIT_FETCH_${fetch_package_name}_LIST_FILE}" ${COLOR_RESET})
        endif()
    else()
        # CMake property names in genex must respect [A-Za-z_][A-Za-z0-9_]*
        string(REGEX REPLACE "[^A-Za-z0-9_]" "_" fetch_package_name_identifier "${fetch_package_name}")
        set_target_properties(CSTOOLKIT PROPERTIES FETCH_${fetch_package_name_identifier}_URL "${_fetch_params_URL}")
        set_target_properties(CSTOOLKIT PROPERTIES FETCH_${fetch_package_name_identifier}_PARAMS "${fetch_dependencies_params}")
        set(CSTOOLKIT_FETCH_${fetch_package_name}_URL "${_fetch_params_URL}")
        set(CSTOOLKIT_FETCH_${fetch_package_name}_LIST_FILE "${CMAKE_CURRENT_LIST_FILE}")
        cstoolkit_download_and_extract_package("${fetch_package_name}" "${_fetch_params_URL}" ${download_and_extract_params})
        if(NOT _fetch_params_GIT_BUILD)
            _cstoolkit_internal_find_package("${fetch_package_name}" "${CSTOOLKIT_EXTERNALS}/${fetch_package_name}" ${find_package_params})
        endif()
    endif()

    unset(fetch_package_OPTIONS)
    unset(fetch_package_UNIQUE)
    unset(fetch_package_MULTIPLE)
    unset(_fetch_params_LEGACY)
    unset(_fetch_params_CODX)
    unset(_fetch_params_MODULE)
    unset(_fetch_params_CONFIG)
    unset(_fetch_params_NO_MODULE)
    unset(_fetch_params_URL)
    unset(fetch_dependencies_params)
    unset(download_and_extract_params)
    unset(find_package_params)
endmacro()

function(cstoolkit_download_and_extract_package fetch_package_name fetch_package_url)
    set(FETCH_OPTIONS LEGACY CODX GIT_SRC GIT_BUILD)
    set(FETCH_UNIQUE)
    set(FETCH_MULTIPLE)
    cmake_parse_arguments(PARSE_ARGV 2 fetch_package "${FETCH_OPTIONS}" "${FETCH_UNIQUE}" "${FETCH_MULTIPLE}")

    set(_fetch_package_download_dir "${CSTOOLKIT_DOWNLOAD_BASE_DIR}/${fetch_package_name}")
    set(_fetch_package_extract_dir "${CSTOOLKIT_EXTERNALS}/${fetch_package_name}")
    set(_fetch_package_src_dir "${CSTOOLKIT_EXTERNALS}/${fetch_package_name}-src")
    set(_fetch_package_binary_dir "${CSTOOLKIT_EXTERNALS}/${fetch_package_name}-build")
    cmake_path(GET fetch_package_url FILENAME _fetch_package_filename)

    # Reconfigure if download.stamp modified
    set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS "${_fetch_package_download_dir}/download.stamp")

    # Get previous URL from download.stamp
    set(_fetch_package_previous_url)
    if(EXISTS "${_fetch_package_download_dir}/download.stamp")
        file(READ "${_fetch_package_download_dir}/download.stamp" _fetch_package_previous_url)
    endif()

    # if a step is redone, all next steps need to be redone
    set(_fetch_package_rebuild 0)

    # Download if previous url different
    if((NOT _fetch_package_previous_url STREQUAL "${fetch_package_url}") OR (NOT EXISTS "${_fetch_package_download_dir}/${_fetch_package_filename}"))
        cstoolkit_start_timer(CSTOOLKIT_FETCH_TIMER)
        message(STATUS "CSToolkit: Download package ${fetch_package_name}")

        set(_fetch_package_rebuild 1)

        _cstoolkit_internal_download("${fetch_package_url}" "${_fetch_package_download_dir}")

        file(WRITE "${_fetch_package_download_dir}/download.stamp" "${fetch_package_url}")

        cstoolkit_end_timer(CSTOOLKIT_FETCH_TIMER CSTOOLKIT_FETCH_ELAPSED)
        message(STATUS "CSToolkit: Download package ${fetch_package_name} done (${CSTOOLKIT_FETCH_ELAPSED}s)")
    endif()

    # Determine if we need to extract git info
    set(_missing_gitinfo 0)
    if((fetch_package_GIT_SRC OR CSTOOLKIT_FETCH_GIT_SRC OR CSTOOLKIT_FETCH_GIT_BUILD) AND NOT EXISTS "${_fetch_package_src_dir}")
        if(EXISTS "${_fetch_package_extract_dir}/.gitinfo")
            _cstoolkit_internal_read_gitinfo("${_fetch_package_extract_dir}/.gitinfo")
            if(NOT _cstoolkit_internal_gitinfo_remote_origin)
                set(_missing_gitinfo 1)
            endif()
        else()
            set(_missing_gitinfo 1)
        endif()
    endif()

    # Extract if previous url different or if extract dir doesn't exists
    # GIT_BUILD only needs gitinfo, no need to extract if we already have gitinfo
    if(_fetch_package_rebuild OR _missing_gitinfo OR (NOT (fetch_package_GIT_BUILD OR CSTOOLKIT_FETCH_GIT_BUILD) AND NOT EXISTS "${_fetch_package_extract_dir}"))
        cstoolkit_start_timer(CSTOOLKIT_FETCH_TIMER)
        message(STATUS "CSToolkit: Extract package ${fetch_package_name}")

        set(_internal_extract_params)
        if(fetch_package_GIT_BUILD)
            set(_internal_extract_params "GITINFO_ONLY")
        elseif(CSTOOLKIT_FETCH_GIT_BUILD)
            set(_internal_extract_params "GITINFO_BEST_EFFORT")
        endif()

        _cstoolkit_internal_extract("${_fetch_package_download_dir}/${_fetch_package_filename}" "${_fetch_package_extract_dir}" ${_internal_extract_params})

        # Overwrite Config file for LEGACY and CODX packages
        # No Config generation with GIT_BUILD option
        if(fetch_package_LEGACY AND NOT fetch_package_GIT_BUILD AND NOT (CSTOOLKIT_FETCH_GIT_BUILD AND _cstoolkit_internal_gitinfo_remote_origin))
            if(EXISTS "${_fetch_package_extract_dir}/${fetch_package_name}Config.cmake")
                message(STATUS "CSToolkit: Overwriting ${fetch_package_name}Config.cmake with generated legacy support Config file.")
            endif()
            configure_file(${CSTOOLKIT_ROOT_DIR}/templates/FetchLegacyConfig.${CSTOOLKIT_HOST_PLATFORM}.in "${_fetch_package_extract_dir}/${fetch_package_name}Config.cmake" @ONLY)
        elseif(fetch_package_CODX AND NOT fetch_package_GIT_BUILD AND NOT (CSTOOLKIT_FETCH_GIT_BUILD AND _cstoolkit_internal_gitinfo_remote_origin))
            if(EXISTS "${_fetch_package_extract_dir}/${fetch_package_name}Config.cmake")
                message(STATUS "CSToolkit: Overwriting ${fetch_package_name}Config.cmake with generated CODX support Config file.")
            endif()
            configure_file(${CSTOOLKIT_ROOT_DIR}/templates/FetchCODXConfig.${CSTOOLKIT_HOST_PLATFORM}.in "${_fetch_package_extract_dir}/${fetch_package_name}Config.cmake" @ONLY)
        endif()

        cstoolkit_end_timer(CSTOOLKIT_FETCH_TIMER CSTOOLKIT_FETCH_ELAPSED)
        message(STATUS "CSToolkit: Extract package ${fetch_package_name} done (${CSTOOLKIT_FETCH_ELAPSED}s)")
    endif()

    # Clone git repository for sources if requested
    # Shallow clone if only GIT_SRC specified
    if((fetch_package_GIT_SRC OR CSTOOLKIT_FETCH_GIT_SRC OR CSTOOLKIT_FETCH_GIT_BUILD) AND (_fetch_package_rebuild OR NOT EXISTS "${_fetch_package_src_dir}"))
        # Missing gitinfo
        if(NOT _cstoolkit_internal_gitinfo_remote_origin)
            # CSTOOLKIT_FETCH_GIT_SRC and CSTOOLKIT_FETCH_GIT_BUILD work as best effort
            if(fetch_package_GIT_SRC)
                message(FATAL_ERROR "CSToolkit: Missing git informations for ${fetch_package_name}")
            endif()
        else()
            cstoolkit_start_timer(CSTOOLKIT_FETCH_TIMER)
            message(STATUS "CSToolkit: Clone git repository ${fetch_package_name}")

            set(_internal_git_clone_params "SHALLOW")
            if(fetch_package_GIT_BUILD OR CSTOOLKIT_FETCH_GIT_BUILD)
                set(_internal_git_clone_params)
            endif()

            _cstoolkit_internal_git_clone(${fetch_package_name} "${_cstoolkit_internal_gitinfo_remote_origin}" "${_cstoolkit_internal_gitinfo_last_commit}" "${_fetch_package_src_dir}" ${_internal_git_clone_params})

            cstoolkit_end_timer(CSTOOLKIT_FETCH_TIMER CSTOOLKIT_FETCH_ELAPSED)
            message(STATUS "CSToolkit: Clone git repository ${fetch_package_name} done (${CSTOOLKIT_FETCH_ELAPSED}s)")
        endif()
    endif()

    # Clean up source directory if no more requested
    if(NOT (fetch_package_GIT_SRC OR CSTOOLKIT_FETCH_GIT_SRC OR CSTOOLKIT_FETCH_GIT_BUILD))
        cstoolkit_git_safe_delete("${_fetch_package_src_dir}")
    endif()

    # Full clone + add_subdirectory
    if(fetch_package_GIT_BUILD OR (CSTOOLKIT_FETCH_GIT_BUILD AND _cstoolkit_internal_gitinfo_remote_origin))
        _cstoolkit_internal_git_unshallow(${fetch_package_name} "${_fetch_package_src_dir}")

        file(REMOVE_RECURSE "${_fetch_package_extract_dir}")

        cstoolkit_start_timer(CSTOOLKIT_FETCH_TIMER)
        message(STATUS "CSToolkit: Configure ${fetch_package_name}")

        add_subdirectory("${_fetch_package_src_dir}" "${_fetch_package_binary_dir}")

        cstoolkit_end_timer(CSTOOLKIT_FETCH_TIMER CSTOOLKIT_FETCH_ELAPSED)
        message(STATUS "CSToolkit: Configure ${fetch_package_name} done (${CSTOOLKIT_FETCH_ELAPSED}s)")
    else()
        file(REMOVE_RECURSE "${_fetch_package_binary_dir}")
    endif()
endfunction()

function(_cstoolkit_internal_compute_url url_var url_root url_package_name)
    set(_compute_url_OPTIONS QT LEGACY)
    set(_compute_url_UNIQUE VERSION FOLDER MKSPECS)
    set(_compute_url_MULTIPLE)
    cmake_parse_arguments(PARSE_ARGV 3 _compute_url "${_compute_url_OPTIONS}" "${_compute_url_UNIQUE}" "${_compute_url_MULTIPLE}")

    if(NOT _compute_url_FOLDER)
        set(_compute_url_FOLDER "thirdParty")
    endif()

    if(NOT _compute_url_MKSPECS)
        set(_compute_url_MKSPECS "${CSTOOLKIT_BUILD_MKSPECS}")
        set(_compute_url_MKSPECS_QT "${CSTOOLKIT_BUILD_MKSPECS_QT}")
    elseif(_compute_url_QT)
        if(Qt5_VERSION_MAJOR LESS_EQUAL 5 AND Qt5_VERSION_MINOR LESS_EQUAL 9)
            set(_compute_url_MKSPECS_QT "${_compute_url_MKSPECS}-Qt${QT_VERSION}")
        else()
            set(_compute_url_MKSPECS_QT "${_compute_url_MKSPECS}-Qt${QT_VERSION_MM}")
        endif()
    endif()
    
    set(${url_var} "${url_root}/${_compute_url_FOLDER}/${url_package_name}/${_compute_url_VERSION}/${url_package_name}_${_compute_url_VERSION}")

    if(_compute_url_QT)
        if(_compute_url_LEGACY)
            set(${url_var} "${${url_var}}_${_compute_url_MKSPECS}-QT${QT_VERSION}")
        else()
            set(${url_var} "${${url_var}}_${_compute_url_MKSPECS_QT}")
        endif()
    else()
        set(${url_var} "${${url_var}}_${_compute_url_MKSPECS}")
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
    cmake_parse_arguments(PARSE_ARGV 2 _internal_download "APPEND" "" "")

    if(NOT _internal_download_APPEND)
        file(REMOVE_RECURSE "${_internal_download_dir}/${_internal_download_filename}")
    endif()
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
        if(_internal_download_APPEND)
            file(REMOVE "${_internal_download_dir}/${_internal_download_filename}")
        else()
            file(REMOVE_RECURSE "${_internal_download_dir}")
        endif()
        message(FATAL_ERROR "CSToolkit: Downloading '${_internal_download_url}' failed
STATUS: ${status_code} ${status_string}
        --- LOG BEGIN ---
${log}        --- LOG END ---
")
    endif()
endfunction()

function(_cstoolkit_internal_extract _internal_extract_file _internal_extract_dir)
    cmake_parse_arguments(PARSE_ARGV 2 _internal_extract "GITINFO_ONLY;APPEND;GITINFO_BEST_EFFORT" "" "PATTERNS")

    cmake_path(GET _internal_extract_file PARENT_PATH _internal_extract_temp_dir)
    set(_internal_extract_temp_dir "${_internal_extract_temp_dir}/tmp")
    file(REMOVE_RECURSE "${_internal_extract_temp_dir}")
    file(MAKE_DIRECTORY "${_internal_extract_temp_dir}")

    file(ARCHIVE_EXTRACT INPUT "${_internal_extract_file}" DESTINATION "${_internal_extract_temp_dir}" PATTERNS ${_internal_extract_PATTERNS} TOUCH)

    # Analyze what came out of the tar file:
    file(GLOB contents "${_internal_extract_temp_dir}/*")
    if(contents STREQUAL "")
        file(REMOVE_RECURSE "${_internal_extract_temp_dir}")
        file(SIZE "${_internal_extract_file}" file_size)
        _cstoolkit_internal_format_size(file_size)
        message(FATAL_ERROR "CSToolkit: Extracting of '${_internal_extract_file}' failed
Output directory empty
File size: ${file_size}")
        return()
    endif()

    file(MAKE_DIRECTORY "${_internal_extract_dir}")
    if(NOT _internal_extract_APPEND)
        file(REMOVE_RECURSE "${_internal_extract_dir}")
    endif()

    # Remove top level directory if exists
    list(LENGTH contents n)
    if(NOT n EQUAL 1 OR NOT IS_DIRECTORY "${contents}")
        set(contents "${_internal_extract_temp_dir}")
    endif()

    #extract gitinfo
    _cstoolkit_internal_read_gitinfo("${contents}/.gitinfo" PARENT_SCOPE)

    if(_internal_extract_GITINFO_ONLY OR (_internal_extract_GITINFO_BEST_EFFORT AND _cstoolkit_internal_gitinfo_remote_origin))
        set(_fetch_package_extract_rv 0)
    elseif(_internal_extract_APPEND)
        set(_fetch_package_extract_rv 0)
        cmake_path(ABSOLUTE_PATH _internal_extract_dir BASE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
        file(COPY "${contents}/" DESTINATION "${_internal_extract_dir}")
        file(REMOVE_RECURSE "${contents}")
    else()
        file(RENAME "${contents}" "${_internal_extract_dir}" RESULT _fetch_package_extract_rv)
    endif()

    file(REMOVE_RECURSE "${_internal_extract_temp_dir}")
    if(NOT _fetch_package_extract_rv EQUAL 0)
        file(REMOVE_RECURSE "${_internal_extract_dir}")
        message(FATAL_ERROR "CSToolkit: Extracting of '${_internal_extract_file}' failed
${_fetch_package_extract_rv}")
        return()
    endif()

    # Delete archive after successful extraction if CSTOOLKIT_KEEP_ARCHIVE is OFF
    if(NOT CSTOOLKIT_KEEP_ARCHIVE)
        file(REMOVE "${_internal_extract_file}")
    endif()
endfunction()

function(_cstoolkit_internal_git_clone git_package git_url git_commit git_src_dir)
    cmake_parse_arguments(PARSE_ARGV 3 git_clone "SHALLOW" "" "")

    cstoolkit_git_safe_delete("${git_src_dir}")

    file(MAKE_DIRECTORY "${git_src_dir}")
    if(git_clone_SHALLOW)
        execute_process(COMMAND "${GIT_EXECUTABLE}" init "."
            WORKING_DIRECTORY "${git_src_dir}"
            RESULT_VARIABLE _git_clone_rv
            OUTPUT_VARIABLE _git_clone_output
            ERROR_VARIABLE  _git_clone_output
        )
        if(NOT _git_clone_rv EQUAL 0)
            file(REMOVE_RECURSE "${git_src_dir}")
            message(FATAL_ERROR "CSToolkit: Clone of ${git_package} failed
${_git_clone_output}")
            return()
        endif()
        execute_process(COMMAND "${GIT_EXECUTABLE}" remote add origin ${git_url}
            WORKING_DIRECTORY "${git_src_dir}"
            RESULT_VARIABLE _git_clone_rv
            OUTPUT_VARIABLE _git_clone_output
            ERROR_VARIABLE  _git_clone_output
        )
        if(NOT _git_clone_rv EQUAL 0)
            file(REMOVE_RECURSE "${git_src_dir}")
            message(FATAL_ERROR "CSToolkit: Clone of ${git_package} failed
${_git_clone_output}")
            return()
        endif()
        execute_process(COMMAND "${GIT_EXECUTABLE}" fetch --depth 1 origin ${git_commit}
            WORKING_DIRECTORY "${git_src_dir}"
            RESULT_VARIABLE _git_clone_rv
            OUTPUT_VARIABLE _git_clone_output
            ERROR_VARIABLE  _git_clone_output
        )
        if(NOT _git_clone_rv EQUAL 0)
            file(REMOVE_RECURSE "${git_src_dir}")
            message(FATAL_ERROR "CSToolkit: Clone of ${git_package} failed
${_git_clone_output}")
            return()
        endif()
        execute_process(COMMAND "${GIT_EXECUTABLE}" checkout FETCH_HEAD
            WORKING_DIRECTORY "${git_src_dir}"
            RESULT_VARIABLE _git_clone_rv
            OUTPUT_VARIABLE _git_clone_output
            ERROR_VARIABLE  _git_clone_output
        )
        if(NOT _git_clone_rv EQUAL 0)
            file(REMOVE_RECURSE "${git_src_dir}")
            message(FATAL_ERROR "CSToolkit: Clone of ${git_package} failed
${_git_clone_output}")
            return()
        endif()

        file(WRITE "${git_src_dir}/.git/hooks/pre-commit"
"#!/bin/sh
echo \"This repository is set as READ ONLY by CSToolkit.\"
exit 1")
        file(CHMOD "${git_src_dir}/.git/hooks/pre-commit" PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE)
    else()
        execute_process(COMMAND "${GIT_EXECUTABLE}" clone ${git_url} .
            WORKING_DIRECTORY "${git_src_dir}"
            RESULT_VARIABLE _git_clone_rv
            OUTPUT_VARIABLE _git_clone_output
            ERROR_VARIABLE  _git_clone_output
        )
        if(NOT _git_clone_rv EQUAL 0)
            file(REMOVE_RECURSE "${git_src_dir}")
            message(FATAL_ERROR "CSToolkit: Clone of ${git_package} failed
${_git_clone_output}")
            return()
        endif()
        execute_process(COMMAND "${GIT_EXECUTABLE}" checkout ${git_commit}
            WORKING_DIRECTORY "${git_src_dir}"
            RESULT_VARIABLE _git_clone_rv
            OUTPUT_VARIABLE _git_clone_output
            ERROR_VARIABLE  _git_clone_output
        )
        if(NOT _git_clone_rv EQUAL 0)
            file(REMOVE_RECURSE "${git_src_dir}")
            message(FATAL_ERROR "CSToolkit: Clone of ${git_package} failed
${_git_clone_output}")
            return()
        endif()
    endif()
endfunction()

function(_cstoolkit_internal_git_unshallow git_package git_src_dir)
    execute_process(COMMAND "${GIT_EXECUTABLE}" rev-parse --is-shallow-repository
        WORKING_DIRECTORY "${git_src_dir}"
        OUTPUT_VARIABLE _git_unshallow_is_shallow
        ERROR_QUIET
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    if(_git_unshallow_is_shallow STREQUAL "false") # not a shallow repository
        return()
    endif()

    cstoolkit_start_timer(CSTOOLKIT_FETCH_TIMER)
    message(STATUS "CSToolkit: Unshallow git repository ${fetch_package_name}")

    execute_process(COMMAND "${GIT_EXECUTABLE}" fetch --unshallow
        WORKING_DIRECTORY "${git_src_dir}"
        RESULT_VARIABLE _git_unshallow_rv
        OUTPUT_VARIABLE _git_unshallow_output
        ERROR_VARIABLE  _git_unshallow_output
    )
    if(NOT _git_unshallow_rv EQUAL 0)
        file(REMOVE_RECURSE "${git_src_dir}")
        message(FATAL_ERROR "CSToolkit: Unshallow of ${git_package} failed
${_git_unshallow_output}")
        return()
    endif()
    file(REMOVE "${git_src_dir}/.git/hooks/pre-commit")

    cstoolkit_end_timer(CSTOOLKIT_FETCH_TIMER CSTOOLKIT_FETCH_ELAPSED)
    message(STATUS "CSToolkit: Unshallow git repository ${fetch_package_name} done (${CSTOOLKIT_FETCH_ELAPSED}s)")
endfunction()

function(cstoolkit_download_url URL OUTPUT_VAR)
    set(options NO_EXTRACT APPEND)
    set(one_value_args)
    if(OUTPUT_VAR STREQUAL "DESTINATION")
        set(one_value_args ${OUTPUT_VAR})
    endif()
    set(multi_value_args PATTERNS)
    cmake_parse_arguments(PARSE_ARGV 1 _download "${options}" "${one_value_args}" "${multi_value_args}")

    if(OUTPUT_VAR STREQUAL "DESTINATION" AND NOT _download_DESTINATION)
        message(SEND_ERROR "CSToolkit: cstoolkit_download_url(${URL}): Empty DESTINATION")
        return()
    endif()

    if(_download_APPEND AND NOT OUTPUT_VAR STREQUAL "DESTINATION")
        message(SEND_ERROR "CSToolkit: cstoolkit_download_url(${URL}): APPEND requires DESTINATION")
        return()
    endif()

    cmake_path(GET URL STEM LAST_ONLY _FILENAME)
    cmake_path(GET URL FILENAME _FULLFILENAME)
    string(MD5 _HASH ${URL})
    string(SUBSTRING ${_HASH} 0 8 _HASH)

            set(download_dir "${CSTOOLKIT_DOWNLOAD_BASE_DIR}/${_FILENAME}_${_HASH}")

    if(NOT OUTPUT_VAR STREQUAL "DESTINATION")
        if(_download_NO_EXTRACT)
            set(_download_DESTINATION "${CSTOOLKIT_EXTERNALS}/${_FILENAME}_${_HASH}")
            set(${OUTPUT_VAR} "${_download_DESTINATION}/${_FULLFILENAME}")
    else()
            set(_download_DESTINATION "${CSTOOLKIT_EXTERNALS}/${_FILENAME}_${_HASH}")
            set(${OUTPUT_VAR} "${_download_DESTINATION}")
        endif()
    
        set(${OUTPUT_VAR} ${${OUTPUT_VAR}} PARENT_SCOPE)
    endif()

    # Reconfigure if download.stamp modified
    set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS "${download_dir}/download.stamp")

    # Get previous URL from download.stamp
    set(_download_previous_url)
    if(EXISTS "${download_dir}/download.stamp")
        file(READ "${download_dir}/download.stamp" _download_previous_url)
    endif()

    set(_fresh_download FALSE)

    if( (NOT _download_previous_url STREQUAL "${URL}") # URL Changed
     OR (_download_NO_EXTRACT AND NOT EXISTS "${_download_DESTINATION}/${_FULLFILENAME}") # File missing (NO_EXTRACT)
     OR (NOT _download_NO_EXTRACT AND NOT EXISTS "${_download_DESTINATION}" AND NOT EXISTS "${download_dir}/${_FULLFILENAME}")) # Extracted directory and original archive missing
        cstoolkit_start_timer(CSTOOLKIT_DOWNLOAD_TIMER)
        message(STATUS "CSToolkit: Download ${URL}")

        if(_download_NO_EXTRACT)
            set(_download_params)
            if(_download_APPEND)
                list(APPEND _download_params APPEND)
            endif()
            _cstoolkit_internal_download("${URL}" "${_download_DESTINATION}" ${_download_params})
        else()
            _cstoolkit_internal_download("${URL}" "${download_dir}")
        endif()
        set(_fresh_download TRUE)

        cstoolkit_end_timer(CSTOOLKIT_DOWNLOAD_TIMER CSTOOLKIT_DOWNLOAD_ELAPSED)
        message(STATUS "CSToolkit: Download ${URL} done (${CSTOOLKIT_DOWNLOAD_ELAPSED}s)")
    endif()

    if((NOT _download_NO_EXTRACT) AND (_fresh_download OR NOT EXISTS "${_download_DESTINATION}"))
        cstoolkit_start_timer(CSTOOLKIT_DOWNLOAD_TIMER)
        message(STATUS "CSToolkit: Extract ${URL}")

        set(_extract_params PATTERNS ${_download_PATTERNS})
        if(_download_APPEND)
            list(APPEND _extract_params APPEND)
        endif()
        _cstoolkit_internal_extract("${download_dir}/${_FULLFILENAME}" "${_download_DESTINATION}" ${_extract_params})

        cstoolkit_end_timer(CSTOOLKIT_DOWNLOAD_TIMER CSTOOLKIT_DOWNLOAD_ELAPSED)
        message(STATUS "CSToolkit: Extract ${URL} done (${CSTOOLKIT_DOWNLOAD_ELAPSED}s)")
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

macro(_cstoolkit_internal_read_gitinfo gitinfo_file)
    foreach(key current_tree;last_commit;last_commit_date;branch;remote_origin;tag;source_dir;timestamp)
        set(_cstoolkit_internal_gitinfo_${key})
    endforeach()

    if(EXISTS "${gitinfo_file}")
        file(READ "${gitinfo_file}" _gitinfo_contents)

        # Split content into lines
        string(REPLACE "\n" ";" GITINFO_LINES "${_gitinfo_contents}")

        foreach(line ${GITINFO_LINES})
            # Skip empty lines or comments
            if(line MATCHES "^[ \t\r\n]*$" OR line MATCHES "^#")
                continue()
            endif()

            # Match key: value pattern
            if(line MATCHES "^([a-zA-Z0-9_]+): (.+)$")
                set(_cstoolkit_internal_gitinfo_key "${CMAKE_MATCH_1}")
                set(_cstoolkit_internal_gitinfo_value "${CMAKE_MATCH_2}")
                string(STRIP "${_cstoolkit_internal_gitinfo_value}" _cstoolkit_internal_gitinfo_value)

                set(_cstoolkit_internal_gitinfo_${_cstoolkit_internal_gitinfo_key} "${_cstoolkit_internal_gitinfo_value}")
            endif()
        endforeach()
    endif()

    set(gitinfo_parent_scope)
    if(${ARGC} GREATER 1)
        list(APPEND gitinfo_parent_scope "${ARGV1}")
    endif()

    foreach(key current_tree;last_commit;last_commit_date;branch;remote_origin;tag;source_dir;timestamp)
        set(_cstoolkit_internal_gitinfo_${key} "${_cstoolkit_internal_gitinfo_${key}}" ${gitinfo_parent_scope})
    endforeach()
endmacro()

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
