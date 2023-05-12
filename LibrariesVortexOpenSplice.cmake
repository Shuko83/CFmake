################################################################################
# VortexOpenSplice package libraries finder
################################################################################

function(find_vos_package_libraries NAME LIBNAMES INCPATH LIBPATH BINPATH)

  # libfind_pkg_check_modules()

  find_path(${NAME}_INCLUDE_DIR
    NAMES ${INCPATH}
    PATHS ${${NAME}_ROOT} ${${NAME}_PKGCONF_INCLUDE_DIRS})

  foreach(LIBNAME IN LISTS LIBNAMES)
    find_vos_package_library(${NAME} ${LIBNAME} ${LIBPATH} ${BINPATH})
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

endfunction(find_vos_package_libraries NAME LIBNAMES INCPATH LIBPATH BINPATH)

################################################################################
# VortexOpenSplice package library finder
################################################################################

function(find_vos_package_library NAME LIBNAME LIBPATH BINPATH)

  find_library(${NAME}_${LIBNAME}_LIB_D
    NAMES ${LIBNAME}.lib
    PATHS ${${NAME}_ROOT}/${LIBPATH}_debug ${${NAME}_PKGCONF_LIBRARY_DIRS}
    NO_DEFAULT_PATH)

  find_library(${NAME}_${LIBNAME}_LIB_R
    NAMES ${LIBNAME}.lib
    PATHS ${${NAME}_ROOT}/${LIBPATH} ${${NAME}_PKGCONF_LIBRARY_DIRS}
    NO_DEFAULT_PATH)

  get_filename_component(${NAME}_${LIBNAME}_LIBPATH_D ${${NAME}_${LIBNAME}_LIB_D} DIRECTORY)
  get_filename_component(${NAME}_${LIBNAME}_LIBPATH_R ${${NAME}_${LIBNAME}_LIB_R} DIRECTORY)

  add_library(${NAME}::${LIBNAME} SHARED IMPORTED)

  set_target_properties(${NAME}::${LIBNAME} PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${${NAME}_INCLUDE_DIR})

  if(${NAME}_${LIBNAME}_LIB_D)
    set_property(TARGET ${NAME}::${LIBNAME} APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
    set_target_properties(${NAME}::${LIBNAME} PROPERTIES
      IMPORTED_IMPLIB_DEBUG ${${NAME}_${LIBNAME}_LIB_D}
      IMPORTED_LOCATION_DEBUG ${${NAME}_${LIBNAME}_LIBPATH_D}/../${BINPATH}_debug/${LIBNAME}.dll)
  else(${NAME}_${LIBNAME}_LIB_D)
    message(DEBUG "Could not find debug library ${NAME}::${LIBNAME}")
  endif(${NAME}_${LIBNAME}_LIB_D)

  if(${NAME}_${LIBNAME}_LIB_R)
    set_property(TARGET ${NAME}::${LIBNAME} APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE DEBUG MINSIZEREL RELWITHDEBINFO)
    set_target_properties(${NAME}::${LIBNAME} PROPERTIES
      IMPORTED_IMPLIB_RELEASE ${${NAME}_${LIBNAME}_LIB_R}
      IMPORTED_IMPLIB_MINSIZEREL ${${NAME}_${LIBNAME}_LIB_R}
      IMPORTED_IMPLIB_RELWITHDEBINFO ${${NAME}_${LIBNAME}_LIB_R}
      IMPORTED_LOCATION_RELEASE ${${NAME}_${LIBNAME}_LIBPATH_R}/../${BINPATH}/${LIBNAME}.dll
      IMPORTED_LOCATION_MINSIZEREL ${${NAME}_${LIBNAME}_LIBPATH_R}/../${BINPATH}/${LIBNAME}.dll
      IMPORTED_LOCATION_RELWITHDEBINFO ${${NAME}_${LIBNAME}_LIBPATH_R}/../${BINPATH}/${LIBNAME}.dll)
  else(${NAME}_${LIBNAME}_LIB_R)
    message(DEBUG "Could not find release library ${NAME}::${LIBNAME}")
  endif(${NAME}_${LIBNAME}_LIB_R)

  set(${NAME}_${LIBNAME}_LIB_D ${${NAME}_${LIBNAME}_LIB_D} CACHE INTERNAL "${NAME}::${LIBNAME} debug library")
  set(${NAME}_${LIBNAME}_LIB_R ${${NAME}_${LIBNAME}_LIB_R} CACHE INTERNAL "${NAME}::${LIBNAME} release library")

endfunction(find_vos_package_library NAME LIBNAME LIBPATH BINPATH)
