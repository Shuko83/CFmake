################################################################################
# Dependency definition
################################################################################

function(define_dependency)

  # Parse arguments

  set(DEP_OPTIONS CREATEDIR NOFIND CMAKE GIT SVN)
  set(DEP_UNIQUE NAME DIRNAME OUTPUT_PATH REMOTE_PATH FIND_PATH)
  set(DEP_MULTIPLE ARCHS TARGETS COMPONENTS)
  cmake_parse_arguments(DEP "${DEP_OPTIONS}" "${DEP_UNIQUE}" "${DEP_MULTIPLE}" ${ARGN})

  # Variables

  if(DEP_GIT OR DEP_SVN)
    set(DEP_CREATEDIR TRUE)
  endif(DEP_GIT OR DEP_SVN)

  if(NOT DEFINED DEP_DIRNAME)
    set(DEP_DIRNAME ${DEP_NAME})
  endif(NOT DEFINED DEP_DIRNAME)

  if(NOT DEFINED DEP_OUTPUT_PATH)
    set(DEP_OUTPUT_PATH ${CMAKE_PREFIX_PATH})
  endif(NOT DEFINED DEP_OUTPUT_PATH)

  if(NOT DEFINED DEP_FIND_PATH)
    set(DEP_FIND_PATH ${DEP_OUTPUT_PATH}/${DEP_DIRNAME})
  endif(NOT DEFINED DEP_FIND_PATH)

  if(NOT DEFINED DEP_ARCHS)
    list(APPEND DEP_ARCHS "${DD_ARCHS}")
  endif(NOT DEFINED DEP_ARCHS)

  if(NOT DEFINED DEP_TARGETS)
    list(APPEND DEP_TARGETS "${DD_TARGETS}")
  endif(NOT DEFINED DEP_TARGETS)

  set_os_variable(DEP_TARGET_NAME ${TARGET_NAME} "${DEP_TARGETS}-${DEP_ARCHS}")

  # Report (before download and find package)

  set(DEP_FIND FALSE)
  if(NOT DEP_NOFIND)
    set(DEP_FIND TRUE)
  endif(NOT DEP_NOFIND)

  message(STATUS "==================================================")
  message(STATUS "Dependency ${DEP_NAME}")
  message(STATUS "  - Output path: ${DEP_OUTPUT_PATH} ${DEP_FIND_PATH}")
  message(STATUS "  - Remote path: ${DEP_REMOTE_PATH}")
  message(DEBUG "  - Create directory: ${DEP_CREATEDIR} ${DEP_DIRNAME}")
  message(DEBUG "  - Find package: ${DEP_FIND} ${DEP_CMAKE}")
  message(DEBUG "  - Archs: ${DEP_ARCHS}")
  message(DEBUG "  - Targets: ${DEP_TARGETS}")
  message(DEBUG "  - Components: ${DEP_COMPONENTS}")

  # Download

  if(DEFINED DEP_REMOTE_PATH)
    if(NOT DEP_GIT AND NOT DEP_SVN)
      if(NOT DEP_CREATEDIR)
        download_dependency(DIRNAME ${DEP_DIRNAME} OUTPUT_PATH ${DEP_OUTPUT_PATH} REMOTE_PATH ${DEP_REMOTE_PATH} ARCHS ${DEP_ARCHS} TARGETS ${DEP_TARGETS})
      else(NOT DEP_CREATEDIR)
        download_dependency(DIRNAME ${DEP_DIRNAME} OUTPUT_PATH ${DEP_OUTPUT_PATH} REMOTE_PATH ${DEP_REMOTE_PATH} ARCHS ${DEP_ARCHS} TARGETS ${DEP_TARGETS} CREATEDIR)
      endif(NOT DEP_CREATEDIR)
    elseif(DEP_GIT)
      download_dependency(DIRNAME ${DEP_DIRNAME} OUTPUT_PATH ${DEP_OUTPUT_PATH} REMOTE_PATH ${DEP_REMOTE_PATH} ARCHS ${DEP_ARCHS} TARGETS ${DEP_TARGETS} GIT)
    elseif(DEP_SVN)
      download_dependency(DIRNAME ${DEP_DIRNAME} OUTPUT_PATH ${DEP_OUTPUT_PATH} REMOTE_PATH ${DEP_REMOTE_PATH} ARCHS ${DEP_ARCHS} TARGETS ${DEP_TARGETS} SVN)
    endif(NOT DEP_GIT AND NOT DEP_SVN)
  endif(DEFINED DEP_REMOTE_PATH)

  # Find package

  if(NOT DEP_NOFIND)
    message(DEBUG "Searching package ${DEP_NAME} in ${DEP_FIND_PATH}")
    if(NOT DEP_CMAKE)
      set(${DEP_NAME}_ROOT ${DEP_FIND_PATH})
      find_package(${DEP_NAME} REQUIRED)
    else(NOT DEP_CMAKE)
      # Note: Set gRPC dependencies
      if(DEP_NAME STREQUAL "gRPC")
        find_package(absl REQUIRED PATHS ${DEP_FIND_PATH})
        find_package(protobuf REQUIRED PATHS ${DEP_FIND_PATH})
      endif(DEP_NAME STREQUAL "gRPC")
      if(NOT DEFINED DEP_COMPONENTS)
        find_package(${DEP_NAME} REQUIRED PATHS ${DEP_FIND_PATH})
      else(NOT DEFINED DEP_COMPONENTS)
        find_package(${DEP_NAME} COMPONENTS ${DEP_COMPONENTS} REQUIRED PATHS ${DEP_FIND_PATH})
      endif(NOT DEFINED DEP_COMPONENTS)
      # Note: Set GTest libraries
      if(DEP_NAME STREQUAL "GTest")
        set(${DEP_NAME}_LIBRARIES "GTest::gmock;GTest::gmock_main;GTest::gtest;GTest::gtest_main")
      endif(DEP_NAME STREQUAL "GTest")
      # Note: Set Tracy libraries
      if(DEP_NAME STREQUAL "Tracy")
        set(${DEP_NAME}_LIBRARIES "Tracy::TracyClient")
      endif(DEP_NAME STREQUAL "Tracy")
    endif(NOT DEP_CMAKE)
    message(DEBUG "Found ${DEP_NAME} libraries: ${${DEP_NAME}_LIBRARIES}")
    # Set dependency cache variables
    set(${DEP_NAME}_FOUND ${${DEP_NAME}_FOUND} CACHE INTERNAL "${DEP_NAME} found status")
    set(${DEP_NAME}_DIR ${${DEP_NAME}_DIR} CACHE INTERNAL "${DEP_NAME} directory")
    set(${DEP_NAME}_INCLUDE_DIR ${${DEP_NAME}_INCLUDE_DIR} CACHE INTERNAL "${DEP_NAME} include directory")
    set(${DEP_NAME}_LIBRARIES ${${DEP_NAME}_LIBRARIES} CACHE INTERNAL "${DEP_NAME} libraries")
  endif(NOT DEP_NOFIND)

endfunction(define_dependency)

################################################################################
# Dependency download
################################################################################

function(download_dependency)

  # Parse arguments

  set(DD_OPTIONS CREATEDIR GIT SVN)
  set(DD_UNIQUE DIRNAME OUTPUT_PATH REMOTE_PATH)
  set(DD_MULTIPLE ARCHS TARGETS)
  cmake_parse_arguments(DD "${DD_OPTIONS}" "${DD_UNIQUE}" "${DD_MULTIPLE}" ${ARGN})

  # Check output path existence and create if necessary

  if(NOT EXISTS ${DD_OUTPUT_PATH})
    file(MAKE_DIRECTORY ${DD_OUTPUT_PATH})
  endif(NOT EXISTS ${DD_OUTPUT_PATH})

  # Check existing .git directory

  set(DD_GIT_OLD FALSE)
  if(EXISTS ${DD_OUTPUT_PATH}/${DD_DIRNAME}/.git)
    set(DD_GIT_OLD TRUE)
  endif(EXISTS ${DD_OUTPUT_PATH}/${DD_DIRNAME}/.git)

  # Check existing .svn directory

  set(DD_SVN_OLD FALSE)
  if(EXISTS ${DD_OUTPUT_PATH}/${DD_DIRNAME}/.svn)
    set(DD_SVN_OLD TRUE)
  endif(EXISTS ${DD_OUTPUT_PATH}/${DD_DIRNAME}/.svn)

  # Get Google Chrome command

  if(EXISTS "C:/Program Files/Google/Chrome/Application/chrome.exe")
    set(GOOGLECHROME_COMMAND "C:/Program Files/Google/Chrome/Application/chrome.exe")
  elseif(EXISTS "C:/Program Files (x86)/Google/Chrome/Application/chrome.exe")
    set(GOOGLECHROME_COMMAND "C:/Program Files (x86)/Google/Chrome/Application/chrome.exe")
  endif(EXISTS "C:/Program Files/Google/Chrome/Application/chrome.exe")

  # Get remote paths list to download

  foreach(DD_ARCH IN LISTS DD_ARCHS)
    string(REPLACE "%ARCH%" ${DD_ARCH} DD_REMOTE_PATH_ITEM ${DD_REMOTE_PATH})
    foreach(DD_TARGET IN LISTS DD_TARGETS)
      string(REPLACE "%TARGET%" ${DD_TARGET} DD_REMOTE_PATH_ITEM ${DD_REMOTE_PATH_ITEM})
      list(APPEND DD_REMOTE_PATHS ${DD_REMOTE_PATH_ITEM})
    endforeach(DD_TARGET IN LISTS DD_TARGETS)
  endforeach(DD_ARCH IN LISTS DD_ARCHS)

  if(NOT DD_REMOTE_PATHS)
    list(APPEND DD_REMOTE_PATHS ${DD_REMOTE_PATH})
  endif(NOT DD_REMOTE_PATHS)

  # Check update

  set(DD_UPDATE TRUE)
  if(DEFINED ENV{DD_NOUPDATE})
    set(DD_UPDATE FALSE)
  endif(DEFINED ENV{DD_NOUPDATE})

  # Download remote paths

  foreach(DD_REMOTE_PATH IN LISTS DD_REMOTE_PATHS)

    if(NOT DD_GIT AND NOT DD_SVN)

      # Remove existing directory if not created from remote file

      if(EXISTS ${DD_OUTPUT_PATH}/${DD_DIRNAME} AND (DD_GIT_OLD OR DD_SVN_OLD))
        file(REMOVE_RECURSE ${DD_OUTPUT_PATH}/${DD_DIRNAME})
      endif(EXISTS ${DD_OUTPUT_PATH}/${DD_DIRNAME} AND (DD_GIT_OLD OR DD_SVN_OLD))

      # Get remote path name and extension

      get_filename_component(DD_NAME ${DD_REMOTE_PATH} NAME)
      get_filename_component(DD_NAME_EXTENSION ${DD_NAME} LAST_EXT)
      string(REPLACE "." "" DD_NAME_EXTENSION ${DD_NAME_EXTENSION})

      # Check file existence

      set(DD_NOFILE TRUE)
      if(EXISTS ${DD_OUTPUT_PATH}/${DD_NAME})
        set(DD_NOFILE FALSE)
      endif(EXISTS ${DD_OUTPUT_PATH}/${DD_NAME})

      # Get failure, old and remote checksums

      set(DD_CHECKSUM_EMPTY_FILE "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855")
      set(DD_CHECKSUM_ERROR_FILE "6497b75e8c519d72e159a7c8f95c2e0c2275880500cc9dd64224fde3e02a490f")

      if(EXISTS ${DD_OUTPUT_PATH}/${DD_NAME} AND DD_UPDATE)
        file(SHA256 ${DD_OUTPUT_PATH}/${DD_NAME} DD_CHECKSUM_OLD)
      endif(EXISTS ${DD_OUTPUT_PATH}/${DD_NAME} AND DD_UPDATE)

      string(REPLACE "${ARTIFACTORY_URL}" "${ARTIFACTORY_URL}/webapp/#/artifacts/browse/tree/General" DD_REMOTE_PAGE ${DD_REMOTE_PATH})
      if(DEFINED GOOGLECHROME_COMMAND AND (DD_NOFILE OR DD_UPDATE))
        execute_process(COMMAND ${GOOGLECHROME_COMMAND} --headless --disable-gpu --run-all-compositor-stages-before-draw --virtual-time-budget=10000 --dump-dom ${DD_REMOTE_PAGE} OUTPUT_FILE ${DD_OUTPUT_PATH}/${DD_NAME}.html)
        if(EXISTS ${DD_OUTPUT_PATH}/${DD_NAME}.html)
          file(READ ${DD_OUTPUT_PATH}/${DD_NAME}.html DD_REMOTE_PAGE_CONTENT)
          file(REMOVE ${DD_OUTPUT_PATH}/${DD_NAME}.html)
          if(DD_REMOTE_PAGE_CONTENT)
            string(REGEX MATCH "<div class=\"key-cell ng-binding\">SHA-256:</div> [0-9a-z]*" DD_CHECKSUM_REMOTE ${DD_REMOTE_PAGE_CONTENT})
            string(REPLACE "<div class=\"key-cell ng-binding\">SHA-256:</div> " "" DD_CHECKSUM_REMOTE ${DD_CHECKSUM_REMOTE})
          endif(DD_REMOTE_PAGE_CONTENT)
        endif(EXISTS ${DD_OUTPUT_PATH}/${DD_NAME}.html)
      endif(DEFINED GOOGLECHROME_COMMAND AND (DD_NOFILE OR DD_UPDATE))

      # Download remote path

      if(NOT EXISTS ${DD_OUTPUT_PATH}/${DD_NAME})
        # Local file does not exists
        message(STATUS "Download: ${DD_REMOTE_PATH}")
        execute_process(COMMAND curl -O ${DD_REMOTE_PATH} WORKING_DIRECTORY ${DD_OUTPUT_PATH})
      elseif((DD_CHECKSUM_OLD STREQUAL DD_CHECKSUM_EMPTY_FILE OR DD_CHECKSUM_OLD STREQUAL DD_CHECKSUM_ERROR_FILE) AND DD_UPDATE)
        # Local file has failure checksum
        message(STATUS "Update: ${DD_REMOTE_PATH}")
        execute_process(COMMAND curl -O ${DD_REMOTE_PATH} WORKING_DIRECTORY ${DD_OUTPUT_PATH})
      elseif(DEFINED DD_CHECKSUM_REMOTE AND NOT DD_CHECKSUM_OLD STREQUAL DD_CHECKSUM_REMOTE AND DD_UPDATE)
        # Local file has different checksum from remote
        message(STATUS "Update: ${DD_REMOTE_PATH}")
        execute_process(COMMAND curl -O ${DD_REMOTE_PATH} WORKING_DIRECTORY ${DD_OUTPUT_PATH})
      elseif(NOT DEFINED DD_CHECKSUM_REMOTE AND DD_UPDATE)
        # Local file has older timestamp than remote
        message(STATUS "Update: ${DD_REMOTE_PATH}")
        execute_process(COMMAND curl -z ${DD_OUTPUT_PATH}/${DD_NAME} -O ${DD_REMOTE_PATH} WORKING_DIRECTORY ${DD_OUTPUT_PATH})
      endif(NOT EXISTS ${DD_OUTPUT_PATH}/${DD_NAME})

      # Check file existence

      if(NOT EXISTS ${DD_OUTPUT_PATH}/${DD_NAME})
        display_download_file_error(${DD_REMOTE_PATH})
      endif(NOT EXISTS ${DD_OUTPUT_PATH}/${DD_NAME})

      # Get new checksum

      if(DD_NOFILE OR DD_UPDATE)
        file(SHA256 ${DD_OUTPUT_PATH}/${DD_NAME} DD_CHECKSUM_NEW)
      endif(DD_NOFILE OR DD_UPDATE)

      # Check file signature

      if((DD_CHECKSUM_NEW STREQUAL DD_CHECKSUM_EMPTY_FILE OR DD_CHECKSUM_NEW STREQUAL DD_CHECKSUM_ERROR_FILE) AND (DD_NOFILE OR DD_UPDATE))
        display_download_file_error(${DD_REMOTE_PATH})
      endif((DD_CHECKSUM_NEW STREQUAL DD_CHECKSUM_EMPTY_FILE OR DD_CHECKSUM_NEW STREQUAL DD_CHECKSUM_ERROR_FILE) AND (DD_NOFILE OR DD_UPDATE))

      if(DEFINED DD_CHECKSUM_REMOTE AND NOT DD_CHECKSUM_NEW STREQUAL DD_CHECKSUM_REMOTE AND (DD_NOFILE OR DD_UPDATE))
        display_download_file_error(${DD_REMOTE_PATH})
      endif(DEFINED DD_CHECKSUM_REMOTE AND NOT DD_CHECKSUM_NEW STREQUAL DD_CHECKSUM_REMOTE AND (DD_NOFILE OR DD_UPDATE))

      # Create extraction directory if necessary

      if(DD_CREATEDIR AND NOT EXISTS ${DD_OUTPUT_PATH}/${DD_DIRNAME})
        file(MAKE_DIRECTORY ${DD_OUTPUT_PATH}/${DD_DIRNAME})
      endif(DD_CREATEDIR AND NOT EXISTS ${DD_OUTPUT_PATH}/${DD_DIRNAME})

      # Extract file

      # TODO: Should be able to extract different ARCHS and TARGETS in the same extraction directory
      if((NOT DEFINED DD_CHECKSUM_OLD OR NOT DD_CHECKSUM_OLD STREQUAL DD_CHECKSUM_NEW) AND (DD_NOFILE OR DD_UPDATE))
        if(DEFINED DD_CHECKSUM_OLD)
          file(REMOVE_RECURSE ${DD_OUTPUT_PATH}/${DD_DIRNAME})  # Note: Invalid for multiple archs or targets
        endif(DEFINED DD_CHECKSUM_OLD)
        if(DD_NAME_EXTENSION MATCHES "zip|tar|gz|bz2|xz|7z")
          message(STATUS "Extract: ${DD_NAME}")
          if(NOT DD_CREATEDIR)
            execute_process(COMMAND cmake -E tar xzf ${DD_OUTPUT_PATH}/${DD_NAME} WORKING_DIRECTORY ${DD_OUTPUT_PATH})
          else(NOT DD_CREATEDIR)
            execute_process(COMMAND cmake -E tar xzf ${DD_OUTPUT_PATH}/${DD_NAME} WORKING_DIRECTORY ${DD_OUTPUT_PATH}/${DD_DIRNAME})
          endif(NOT DD_CREATEDIR)
        else(DD_NAME_EXTENSION MATCHES "zip|tar|gz|bz2|xz|7z")
          message(STATUS "Copy: ${DD_NAME}")
          file(COPY ${DD_OUTPUT_PATH}/${DD_NAME} DESTINATION ${DD_OUTPUT_PATH}/${DD_DIRNAME})
        endif(DD_NAME_EXTENSION MATCHES "zip|tar|gz|bz2|xz|7z")
      endif((NOT DEFINED DD_CHECKSUM_OLD OR NOT DD_CHECKSUM_OLD STREQUAL DD_CHECKSUM_NEW) AND (DD_NOFILE OR DD_UPDATE))

    elseif(DD_GIT)

      # Remove existing directory if not created from Git

      if(EXISTS ${DD_OUTPUT_PATH}/${DD_DIRNAME} AND NOT DD_GIT_OLD)
        file(REMOVE_RECURSE ${DD_OUTPUT_PATH}/${DD_DIRNAME})
      endif(EXISTS ${DD_OUTPUT_PATH}/${DD_DIRNAME} AND NOT DD_GIT_OLD)

      # TODO: Download and check repository

      message(WARNING "Dependency download from Git currently unavailable.")

    elseif(DD_SVN)

      # Remove existing directory if not created from Subversion

      if(EXISTS ${DD_OUTPUT_PATH}/${DD_DIRNAME} AND NOT DD_SVN_OLD)
        file(REMOVE_RECURSE ${DD_OUTPUT_PATH}/${DD_DIRNAME})
      endif(EXISTS ${DD_OUTPUT_PATH}/${DD_DIRNAME} AND NOT DD_SVN_OLD)

      # Download repository

      if(NOT EXISTS ${DD_OUTPUT_PATH}/${DD_DIRNAME})
        get_authentication(SVN)
        # TODO: Define checkout or export cases
        message(STATUS "Subversion checkout: ${DD_REMOTE_PATH}")
        execute_process(COMMAND svn checkout ${DD_REMOTE_PATH} ${DD_DIRNAME} -q --non-interactive --username ${SVN_USERNAME} --password ${SVN_PASSWORD} WORKING_DIRECTORY ${DD_OUTPUT_PATH})
#        message(STATUS "Subversion export: ${DD_REMOTE_PATH}")
#        execute_process(COMMAND svn export ${DD_REMOTE_PATH} ${DD_DIRNAME} -q --non-interactive --username ${SVN_USERNAME} --password ${SVN_PASSWORD} WORKING_DIRECTORY ${DD_OUTPUT_PATH})
      elseif(DD_UPDATE)
        message(STATUS "Subversion update: ${DD_REMOTE_PATH}")
        execute_process(COMMAND svn switch ${DD_REMOTE_PATH} -q --non-interactive --ignore-ancestry --username ${SVN_USERNAME} --password ${SVN_PASSWORD} WORKING_DIRECTORY ${DD_OUTPUT_PATH}/${DD_DIRNAME})
        execute_process(COMMAND svn update -q --non-interactive --username ${SVN_USERNAME} --password ${SVN_PASSWORD} WORKING_DIRECTORY ${DD_OUTPUT_PATH}/${DD_DIRNAME})
        execute_process(COMMAND svn cleanup -q --non-interactive --remove-unversioned --username ${SVN_USERNAME} --password ${SVN_PASSWORD} WORKING_DIRECTORY ${DD_OUTPUT_PATH}/${DD_DIRNAME})
      endif(NOT EXISTS ${DD_OUTPUT_PATH}/${DD_DIRNAME})

      # Check repository

      if(NOT EXISTS ${DD_OUTPUT_PATH}/${DD_DIRNAME})
        display_svn_repository_error(${DD_REMOTE_PATH})
      endif(NOT EXISTS ${DD_OUTPUT_PATH}/${DD_DIRNAME})

    endif(NOT DD_GIT AND NOT DD_SVN)

  endforeach(DD_REMOTE_PATH IN LISTS DD_REMOTE_PATHS)

endfunction(download_dependency)

###############################################################################
# Display download file error
################################################################################

function(display_download_file_error REMOTE_PATH)

  message(FATAL_ERROR "File download error. Please check:\n"
    "    1) Network connection is available.\n"
    "    2) Remote file '${REMOTE_PATH}' exists and is reachable.\n")

endfunction(display_download_file_error REMOTE_PATH)

###############################################################################
# Display Subversion repository error
################################################################################

function(display_svn_repository_error REMOTE_PATH)

  message(FATAL_ERROR "Subversion repository error. Please check:\n"
    "    1) Network connection is available.\n"
    "    2) Remote Subversion repository '${REMOTE_PATH}' exists and is reachable.\n"
    "    3) Environment variables 'SVN_USERNAME' and 'SVN_PASSWORD' are set and correct.\n"
    "    4) Read ownership for specified username is available.\n"
    "    5) Certificate for specified username has been manually accepted.\n")

endfunction(display_svn_repository_error REMOTE_PATH)

################################################################################
# Get runtime dependencies
################################################################################

function(get_runtime_dependencies COMPONENT_NAME)

  get_target_property(COMPONENT_RUNTIME_DEPS ${COMPONENT_NAME} INTERFACE_LINK_LIBRARIES)
  foreach(COMPONENT_RUNTIME_DEP IN LISTS COMPONENT_RUNTIME_DEPS)
    if(TARGET ${COMPONENT_RUNTIME_DEP})
      if(NOT ${COMPONENT_RUNTIME_DEP} IN_LIST RUNTIME_DEPS)
        list(APPEND RUNTIME_DEPS ${COMPONENT_RUNTIME_DEP})
        get_runtime_dependencies(${COMPONENT_RUNTIME_DEP})
      endif(NOT ${COMPONENT_RUNTIME_DEP} IN_LIST RUNTIME_DEPS)
    endif(TARGET ${COMPONENT_RUNTIME_DEP})
  endforeach(COMPONENT_RUNTIME_DEP IN LISTS COMPONENT_RUNTIME_DEPS)
  list(SORT RUNTIME_DEPS)
  list(REMOVE_DUPLICATES RUNTIME_DEPS)
  set(RUNTIME_DEPS ${RUNTIME_DEPS} PARENT_SCOPE)

endfunction(get_runtime_dependencies)

################################################################################
# Set target SONAME property
################################################################################

function(set_target_soname TARGET_NAME)

  get_target_property(TARGET_LOCATION_D ${TARGET_NAME} IMPORTED_LOCATION_DEBUG)
  get_target_property(TARGET_LOCATION_R ${TARGET_NAME} IMPORTED_LOCATION_RELEASE)
  get_filename_component(TARGET_DIR_D ${TARGET_LOCATION_D} DIRECTORY)
  get_filename_component(TARGET_DIR_R ${TARGET_LOCATION_R} DIRECTORY)
  get_filename_component(TARGET_LIB_D ${TARGET_LOCATION_D} NAME)
  get_filename_component(TARGET_LIB_R ${TARGET_LOCATION_R} NAME)
  get_filename_component(TARGET_SONAME_D ${TARGET_LOCATION_D} NAME)
  get_filename_component(TARGET_SONAME_R ${TARGET_LOCATION_R} NAME)

  file(GLOB TARGET_PATHS_D ${TARGET_DIR_D}/${TARGET_LIB_D}*)
  file(GLOB TARGET_PATHS_R ${TARGET_DIR_R}/${TARGET_LIB_R}*)
  foreach(TARGET_PATH_D IN LISTS TARGET_PATHS_D)
    string(REGEX MATCH "${TARGET_DIR_D}/${TARGET_LIB_D}.[0-9]+" TARGET_MATCH_D ${TARGET_PATH_D})
    if(TARGET_MATCH_D)
      get_filename_component(TARGET_SONAME_D ${TARGET_MATCH_D} NAME)
    endif(TARGET_MATCH_D)
  endforeach(TARGET_PATH_D IN LISTS TARGET_PATHS_D)
  foreach(TARGET_PATH_R IN LISTS TARGET_PATHS_R)
    string(REGEX MATCH "${TARGET_DIR_R}/${TARGET_LIB_R}.[0-9]+" TARGET_MATCH_R ${TARGET_PATH_R})
    if(TARGET_MATCH_R)
      get_filename_component(TARGET_SONAME_R ${TARGET_MATCH_R} NAME)
    endif(TARGET_MATCH_R)
  endforeach(TARGET_PATH_R IN LISTS TARGET_PATHS_R)

  set_target_properties(${TARGET_NAME} PROPERTIES
    IMPORTED_SONAME_DEBUG ${TARGET_SONAME_D}
    IMPORTED_SONAME_RELEASE ${TARGET_SONAME_R}
    IMPORTED_SONAME_MINSIZEREL ${TARGET_SONAME_R}
    IMPORTED_SONAME_RELWITHDEBINFO ${TARGET_SONAME_R})

endfunction(set_target_soname TARGET_NAME)

################################################################################
# Relocate runtime dependency using target SONAME property
################################################################################

function(relocate_runtime_dependency RUNTIME_DEP RUNTIME_DEP_CONFIG RUNTIME_DEP_LOCATION)

  if(NOT WIN32)
    get_filename_component(RUNTIME_DEP_NAME ${RUNTIME_DEP_LOCATION} NAME)
    get_target_property(RUNTIME_DEP_SONAME ${RUNTIME_DEP} IMPORTED_SONAME_${RUNTIME_DEP_CONFIG})
    if(RUNTIME_DEP_SONAME AND NOT RUNTIME_DEP_SONAME STREQUAL ${RUNTIME_DEP_NAME})
      string(REPLACE ${RUNTIME_DEP_NAME} ${RUNTIME_DEP_SONAME} RUNTIME_DEP_LOCATION ${RUNTIME_DEP_LOCATION})
      set(RUNTIME_DEP_LOCATION ${RUNTIME_DEP_LOCATION} PARENT_SCOPE)
    endif(RUNTIME_DEP_SONAME AND NOT RUNTIME_DEP_SONAME STREQUAL ${RUNTIME_DEP_NAME})
  endif(NOT WIN32)

endfunction(relocate_runtime_dependency RUNTIME_DEP RUNTIME_DEP_CONFIG RUNTIME_DEP_LOCATION)
