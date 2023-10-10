################################################################################
# Package libraries finder
################################################################################

function(find_package_libraries NAME LIBNAMES INCPATH LIBPATH_D LIBPATH_R)

  # libfind_pkg_check_modules()

  find_path(${NAME}_INCLUDE_DIR
    NAMES ${INCPATH}
    PATHS ${${NAME}_ROOT} ${${NAME}_PKGCONF_INCLUDE_DIRS})

  foreach(LIBNAME IN LISTS LIBNAMES)
    find_package_library(${NAME} ${LIBNAME} ${LIBPATH_D} ${LIBPATH_R})
    list(APPEND ${NAME}_LIBRARY "${NAME}::${LIBNAME}")
  endforeach(LIBNAME IN LISTS LIBNAMES)

  set(${NAME}_PROCESS_INCLUDES ${NAME}_INCLUDE_DIR)
  set(${NAME}_PROCESS_LIBS ${NAME}_LIBRARY)

  libfind_process(${NAME})

  get_filename_component(${NAME}_DIR ${${NAME}_ROOT} ABSOLUTE)
  set(${NAME}_FOUND TRUE PARENT_SCOPE)
  set(${NAME}_DIR ${${NAME}_DIR} PARENT_SCOPE)
  set(${NAME}_INCLUDE_DIR ${${NAME}_INCLUDE_DIR} PARENT_SCOPE)
  set(${NAME}_LIBRARIES ${${NAME}_LIBRARIES} PARENT_SCOPE)

endfunction(find_package_libraries NAME LIBNAMES INCPATH LIBPATH_D LIBPATH_R)

################################################################################
# Package library finder
################################################################################

function(find_package_library NAME LIBNAME LIBPATH_D LIBPATH_R)

  find_library(${NAME}_${LIBNAME}_LIB_D
    NAMES ${LIBNAME}d ${LIBNAME}
    PATHS ${${NAME}_ROOT}/${LIBPATH_D} ${${NAME}_PKGCONF_LIBRARY_DIRS})

  find_library(${NAME}_${LIBNAME}_LIB_R
    NAMES ${LIBNAME}
    PATHS ${${NAME}_ROOT}/${LIBPATH_R} ${${NAME}_PKGCONF_LIBRARY_DIRS})

  if(WIN32)
    get_filename_component(${NAME}_${LIBNAME}_DIR_D ${${NAME}_${LIBNAME}_LIB_D} DIRECTORY)
    get_filename_component(${NAME}_${LIBNAME}_DIR_R ${${NAME}_${LIBNAME}_LIB_R} DIRECTORY)
    set(${NAME}_${LIBNAME}_DLL_D ${${NAME}_${LIBNAME}_DIR_D}/${LIBNAME}d.dll)
    set(${NAME}_${LIBNAME}_DLL_R ${${NAME}_${LIBNAME}_DIR_R}/${LIBNAME}.dll)
  else(WIN32)
    set(${NAME}_${LIBNAME}_DLL_D ${${NAME}_${LIBNAME}_LIB_D})
    set(${NAME}_${LIBNAME}_DLL_R ${${NAME}_${LIBNAME}_LIB_R})
  endif(WIN32)

  add_library(${NAME}::${LIBNAME} SHARED IMPORTED)

  set_target_properties(${NAME}::${LIBNAME} PROPERTIES IMPORTED_NO_SONAME TRUE)
  set_target_properties(${NAME}::${LIBNAME} PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${${NAME}_INCLUDE_DIR})

  if(${NAME}_${LIBNAME}_LIB_D)
    set_property(TARGET ${NAME}::${LIBNAME} APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
    set_target_properties(${NAME}::${LIBNAME} PROPERTIES
      IMPORTED_IMPLIB_DEBUG ${${NAME}_${LIBNAME}_LIB_D}
      IMPORTED_LOCATION_DEBUG ${${NAME}_${LIBNAME}_DLL_D})
  else(${NAME}_${LIBNAME}_LIB_D)
    message(DEBUG "Could not find debug library ${NAME}::${LIBNAME}")
  endif(${NAME}_${LIBNAME}_LIB_D)

  if(${NAME}_${LIBNAME}_LIB_R)
    set_property(TARGET ${NAME}::${LIBNAME} APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE MINSIZEREL RELWITHDEBINFO)
    set_target_properties(${NAME}::${LIBNAME} PROPERTIES
      IMPORTED_IMPLIB_RELEASE ${${NAME}_${LIBNAME}_LIB_R}
      IMPORTED_IMPLIB_MINSIZEREL ${${NAME}_${LIBNAME}_LIB_R}
      IMPORTED_IMPLIB_RELWITHDEBINFO ${${NAME}_${LIBNAME}_LIB_R}
      IMPORTED_LOCATION_RELEASE ${${NAME}_${LIBNAME}_DLL_R}
      IMPORTED_LOCATION_MINSIZEREL ${${NAME}_${LIBNAME}_DLL_R}
      IMPORTED_LOCATION_RELWITHDEBINFO ${${NAME}_${LIBNAME}_DLL_R})
  else(${NAME}_${LIBNAME}_LIB_R)
    message(DEBUG "Could not find release library ${NAME}::${LIBNAME}")
  endif(${NAME}_${LIBNAME}_LIB_R)

  set_target_soname(${NAME}::${LIBNAME})

  set(${NAME}_${LIBNAME}_LIB_D ${${NAME}_${LIBNAME}_LIB_D} CACHE INTERNAL "${NAME}::${LIBNAME} debug library")
  set(${NAME}_${LIBNAME}_LIB_R ${${NAME}_${LIBNAME}_LIB_R} CACHE INTERNAL "${NAME}::${LIBNAME} release library")

endfunction(find_package_library NAME LIBNAME LIBPATH_D LIBPATH_R)

################################################################################
# Package unique libraries finder
################################################################################

function(find_package_unique_libraries NAME LIBNAMES INCPATH LIBPATH)

  # libfind_pkg_check_modules()

  find_path(${NAME}_INCLUDE_DIR
    NAMES ${INCPATH}
    PATHS ${${NAME}_ROOT} ${${NAME}_PKGCONF_INCLUDE_DIRS})

  foreach(LIBNAME IN LISTS LIBNAMES)
    find_package_unique_library(${NAME} ${LIBNAME} ${LIBPATH})
    list(APPEND ${NAME}_LIBRARY "${NAME}::${LIBNAME}")
  endforeach(LIBNAME IN LISTS LIBNAMES)

  set(${NAME}_PROCESS_INCLUDES ${NAME}_INCLUDE_DIR)
  set(${NAME}_PROCESS_LIBS ${NAME}_LIBRARY)

  libfind_process(${NAME})

  get_filename_component(${NAME}_DIR ${${NAME}_ROOT} ABSOLUTE)
  set(${NAME}_FOUND TRUE PARENT_SCOPE)
  set(${NAME}_DIR ${${NAME}_DIR} PARENT_SCOPE)
  set(${NAME}_INCLUDE_DIR ${${NAME}_INCLUDE_DIR} PARENT_SCOPE)
  set(${NAME}_LIBRARIES ${${NAME}_LIBRARIES} PARENT_SCOPE)

endfunction(find_package_unique_libraries NAME LIBNAMES INCPATH LIBPATH)

################################################################################
# Package unique library finder
################################################################################

function(find_package_unique_library NAME LIBNAME LIBPATH)

  find_library(${NAME}_${LIBNAME}_LIB
    NAMES ${LIBNAME}
    PATHS ${${NAME}_ROOT}/${LIBPATH} ${${NAME}_PKGCONF_LIBRARY_DIRS})

  if(WIN32)
    get_filename_component(${NAME}_${LIBNAME}_DIR ${${NAME}_${LIBNAME}_LIB} DIRECTORY)
    set(${NAME}_${LIBNAME}_DLL ${${NAME}_${LIBNAME}_DIR}/${LIBNAME}.dll)
  else(WIN32)
    set(${NAME}_${LIBNAME}_DLL ${${NAME}_${LIBNAME}_LIB})
  endif(WIN32)

  add_library(${NAME}::${LIBNAME} SHARED IMPORTED)

  set_target_properties(${NAME}::${LIBNAME} PROPERTIES IMPORTED_NO_SONAME TRUE)
  set_target_properties(${NAME}::${LIBNAME} PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${${NAME}_INCLUDE_DIR})

  if(${NAME}_${LIBNAME}_LIB)
    set_property(TARGET ${NAME}::${LIBNAME} APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG RELEASE MINSIZEREL RELWITHDEBINFO)
    set_target_properties(${NAME}::${LIBNAME} PROPERTIES
      IMPORTED_IMPLIB_DEBUG ${${NAME}_${LIBNAME}_LIB}
      IMPORTED_IMPLIB_RELEASE ${${NAME}_${LIBNAME}_LIB}
      IMPORTED_IMPLIB_MINSIZEREL ${${NAME}_${LIBNAME}_LIB}
      IMPORTED_IMPLIB_RELWITHDEBINFO ${${NAME}_${LIBNAME}_LIB}
      IMPORTED_LOCATION_DEBUG ${${NAME}_${LIBNAME}_DLL}
      IMPORTED_LOCATION_RELEASE ${${NAME}_${LIBNAME}_DLL}
      IMPORTED_LOCATION_MINSIZEREL ${${NAME}_${LIBNAME}_DLL}
      IMPORTED_LOCATION_RELWITHDEBINFO ${${NAME}_${LIBNAME}_DLL})
  else(${NAME}_${LIBNAME}_LIB)
    message(DEBUG "Could not find unique library ${NAME}::${LIBNAME}")
  endif(${NAME}_${LIBNAME}_LIB)

  set_target_soname(${NAME}::${LIBNAME})

  set(${NAME}_${LIBNAME}_LIB ${${NAME}_${LIBNAME}_LIB} CACHE INTERNAL "${NAME}::${LIBNAME} unique library")

endfunction(find_package_unique_library NAME LIBNAME LIBPATH)
