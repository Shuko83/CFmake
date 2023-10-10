################################################################################
# DLPX-HI package libraries finder
################################################################################

function(find_dlpxhi_package_libraries NAME PATTERN LIBNAMES INCPATH LIBPATH_D LIBPATH_R)

  # libfind_pkg_check_modules()

  find_path(${NAME}_INCLUDE_DIR
    NAMES ${INCPATH}
    PATHS ${${NAME}_ROOT}/include/${PATTERN} ${${NAME}_PKGCONF_INCLUDE_DIRS})

  foreach(LIBNAME IN LISTS LIBNAMES)
    if(${NAME} STREQUAL "DlpxHiCommon")
      find_dlpxhi_package_library_1(${NAME} "DlpxHi" ${LIBNAME} ${LIBPATH_D} ${LIBPATH_R})
      string(REPLACE "DlpxHi" "" LIBNAME_S ${LIBNAME})
      list(APPEND ${NAME}_LIBRARY "${NAME}::${LIBNAME_S}")
    else(${NAME} STREQUAL "DlpxHiCommon")
      find_dlpxhi_package_library_2(${NAME} ${PATTERN} ${LIBNAME} ${LIBPATH_D} ${LIBPATH_R})
      string(REPLACE "${PATTERN}" "" LIBNAME_S ${LIBNAME})
      list(APPEND ${NAME}_LIBRARY "${NAME}::${LIBNAME_S}")
    endif(${NAME} STREQUAL "DlpxHiCommon")
  endforeach(LIBNAME IN LISTS LIBNAMES)

  set(${NAME}_PROCESS_INCLUDES ${NAME}_INCLUDE_DIR)
  set(${NAME}_PROCESS_LIBS ${NAME}_LIBRARY)

  libfind_process(${NAME})

  get_filename_component(${NAME}_DIR ${${NAME}_ROOT} ABSOLUTE)
  set(${NAME}_FOUND TRUE PARENT_SCOPE)
  set(${NAME}_DIR ${${NAME}_DIR} PARENT_SCOPE)
  set(${NAME}_INCLUDE_DIR ${${NAME}_INCLUDE_DIR} PARENT_SCOPE)
  set(${NAME}_LIBRARIES ${${NAME}_LIBRARIES} PARENT_SCOPE)

endfunction(find_dlpxhi_package_libraries NAME PATTERN LIBNAMES INCPATH LIBPATH_D LIBPATH_R)

################################################################################
# DLPX-HI package library finder #1
################################################################################

function(find_dlpxhi_package_library_1 NAME PATTERN LIBNAME LIBPATH_D LIBPATH_R)

  string(REPLACE "${PATTERN}" "" LIBNAME_S ${LIBNAME})

  find_library(${NAME}_${LIBNAME_S}_LIB_D
    NAMES ${LIBNAME}d ${LIBNAME}
    PATHS ${${NAME}_ROOT}/${LIBPATH_D} ${${NAME}_PKGCONF_LIBRARY_DIRS})

  find_library(${NAME}_${LIBNAME_S}_LIB_R
    NAMES ${LIBNAME}
    PATHS ${${NAME}_ROOT}/${LIBPATH_R} ${${NAME}_PKGCONF_LIBRARY_DIRS})

  if(WIN32)
    get_filename_component(${NAME}_${LIBNAME_S}_DIR_D ${${NAME}_${LIBNAME_S}_LIB_D} DIRECTORY)
    get_filename_component(${NAME}_${LIBNAME_S}_DIR_R ${${NAME}_${LIBNAME_S}_LIB_R} DIRECTORY)
    set(${NAME}_${LIBNAME_S}_DLL_D ${${NAME}_${LIBNAME_S}_DIR_D}/${LIBNAME}d.dll)
    set(${NAME}_${LIBNAME_S}_DLL_R ${${NAME}_${LIBNAME_S}_DIR_R}/${LIBNAME}.dll)
  else(WIN32)
    set(${NAME}_${LIBNAME_S}_DLL_D ${${NAME}_${LIBNAME_S}_LIB_D})
    set(${NAME}_${LIBNAME_S}_DLL_R ${${NAME}_${LIBNAME_S}_LIB_R})
  endif(WIN32)

  add_library(${NAME}::${LIBNAME_S} SHARED IMPORTED)

  set_target_properties(${NAME}::${LIBNAME_S} PROPERTIES IMPORTED_NO_SONAME TRUE)
  set_target_properties(${NAME}::${LIBNAME_S} PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${${NAME}_INCLUDE_DIR})

  if(${NAME}_${LIBNAME_S}_LIB_D)
    set_property(TARGET ${NAME}::${LIBNAME_S} APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
    set_target_properties(${NAME}::${LIBNAME_S} PROPERTIES
      IMPORTED_IMPLIB_DEBUG ${${NAME}_${LIBNAME_S}_LIB_D}
      IMPORTED_LOCATION_DEBUG ${${NAME}_${LIBNAME_S}_DLL_D})
  else(${NAME}_${LIBNAME_S}_LIB_D)
    message(DEBUG "Could not find debug library ${NAME}::${LIBNAME_S}")
  endif(${NAME}_${LIBNAME_S}_LIB_D)

  if(${NAME}_${LIBNAME_S}_LIB_R)
    set_property(TARGET ${NAME}::${LIBNAME_S} APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE MINSIZEREL RELWITHDEBINFO)
    set_target_properties(${NAME}::${LIBNAME_S} PROPERTIES
      IMPORTED_IMPLIB_RELEASE ${${NAME}_${LIBNAME_S}_LIB_R}
      IMPORTED_IMPLIB_MINSIZEREL ${${NAME}_${LIBNAME_S}_LIB_R}
      IMPORTED_IMPLIB_RELWITHDEBINFO ${${NAME}_${LIBNAME_S}_LIB_R}
      IMPORTED_LOCATION_RELEASE ${${NAME}_${LIBNAME_S}_DLL_R}
      IMPORTED_LOCATION_MINSIZEREL ${${NAME}_${LIBNAME_S}_DLL_R}
      IMPORTED_LOCATION_RELWITHDEBINFO ${${NAME}_${LIBNAME_S}_DLL_R})
  else(${NAME}_${LIBNAME_S}_LIB_R)
    message(DEBUG "Could not find release library ${NAME}::${LIBNAME_S}")
  endif(${NAME}_${LIBNAME_S}_LIB_R)

  set_target_soname(${NAME}::${LIBNAME_S})

  set(${NAME}_${LIBNAME_S}_LIB_D ${${NAME}_${LIBNAME_S}_LIB_D} CACHE INTERNAL "${NAME}::${LIBNAME_S} debug library")
  set(${NAME}_${LIBNAME_S}_LIB_R ${${NAME}_${LIBNAME_S}_LIB_R} CACHE INTERNAL "${NAME}::${LIBNAME_S} release library")

endfunction(find_dlpxhi_package_library_1 NAME PATTERN LIBNAME LIBPATH_D LIBPATH_R)

################################################################################
# DLPX-HI package library finder #2
################################################################################

function(find_dlpxhi_package_library_2 NAME PATTERN LIBNAME LIBPATH_D LIBPATH_R)

  string(REPLACE "${PATTERN}" "" LIBNAME_S ${LIBNAME})

  find_library(${NAME}_${LIBNAME_S}_LIB_D
    NAMES ${LIBNAME}d ${LIBNAME}
    PATHS ${${NAME}_ROOT}/${LIBPATH_D} ${${NAME}_PKGCONF_LIBRARY_DIRS})

  find_library(${NAME}_${LIBNAME_S}_LIB_R
    NAMES ${LIBNAME}
    PATHS ${${NAME}_ROOT}/${LIBPATH_R} ${${NAME}_PKGCONF_LIBRARY_DIRS})

  if(WIN32)
    get_filename_component(${NAME}_${LIBNAME_S}_DIR_D ${${NAME}_${LIBNAME_S}_LIB_D} DIRECTORY)
    get_filename_component(${NAME}_${LIBNAME_S}_DIR_R ${${NAME}_${LIBNAME_S}_LIB_R} DIRECTORY)
    set(${NAME}_${LIBNAME_S}_DLL_D ${${NAME}_${LIBNAME_S}_DIR_D}/${LIBNAME}d.dll)
    set(${NAME}_${LIBNAME_S}_DLL_R ${${NAME}_${LIBNAME_S}_DIR_R}/${LIBNAME}.dll)
  else(WIN32)
    set(${NAME}_${LIBNAME_S}_DLL_D ${${NAME}_${LIBNAME_S}_LIB_D})
    set(${NAME}_${LIBNAME_S}_DLL_R ${${NAME}_${LIBNAME_S}_LIB_R})
  endif(WIN32)

  add_library(${NAME}::${LIBNAME_S} SHARED IMPORTED)

  set_target_properties(${NAME}::${LIBNAME_S} PROPERTIES IMPORTED_NO_SONAME TRUE)
  set_target_properties(${NAME}::${LIBNAME_S} PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${${NAME}_INCLUDE_DIR}/${LIBNAME_S})

  if(${NAME}_${LIBNAME_S}_LIB_D)
    set_property(TARGET ${NAME}::${LIBNAME_S} APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
    set_target_properties(${NAME}::${LIBNAME_S} PROPERTIES
      IMPORTED_IMPLIB_DEBUG ${${NAME}_${LIBNAME_S}_LIB_D}
      IMPORTED_LOCATION_DEBUG ${${NAME}_${LIBNAME_S}_DLL_D})
  else(${NAME}_${LIBNAME_S}_LIB_D)
    message(DEBUG "Could not find debug library ${NAME}::${LIBNAME_S}")
  endif(${NAME}_${LIBNAME_S}_LIB_D)

  if(${NAME}_${LIBNAME_S}_LIB_R)
    set_property(TARGET ${NAME}::${LIBNAME_S} APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE MINSIZEREL RELWITHDEBINFO)
    set_target_properties(${NAME}::${LIBNAME_S} PROPERTIES
      IMPORTED_IMPLIB_RELEASE ${${NAME}_${LIBNAME_S}_LIB_R}
      IMPORTED_IMPLIB_MINSIZEREL ${${NAME}_${LIBNAME_S}_LIB_R}
      IMPORTED_IMPLIB_RELWITHDEBINFO ${${NAME}_${LIBNAME_S}_LIB_R}
      IMPORTED_LOCATION_RELEASE ${${NAME}_${LIBNAME_S}_DLL_R}
      IMPORTED_LOCATION_MINSIZEREL ${${NAME}_${LIBNAME_S}_DLL_R}
      IMPORTED_LOCATION_RELWITHDEBINFO ${${NAME}_${LIBNAME_S}_DLL_R})
  else(${NAME}_${LIBNAME_S}_LIB_R)
    message(DEBUG "Could not find release library ${NAME}::${LIBNAME_S}")
  endif(${NAME}_${LIBNAME_S}_LIB_R)

  set_target_soname(${NAME}::${LIBNAME_S})

  set(${NAME}_${LIBNAME_S}_LIB_D ${${NAME}_${LIBNAME_S}_LIB_D} CACHE INTERNAL "${NAME}::${LIBNAME_S} debug library")
  set(${NAME}_${LIBNAME_S}_LIB_R ${${NAME}_${LIBNAME_S}_LIB_R} CACHE INTERNAL "${NAME}::${LIBNAME_S} release library")

endfunction(find_dlpxhi_package_library_2 NAME PATTERN LIBNAME LIBPATH_D LIBPATH_R)
