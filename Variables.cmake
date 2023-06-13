################################################################################
# Set variable
################################################################################

macro(set_variable NAME VALUE)

  if(NOT DEFINED ${NAME})
    set(${NAME} ${VALUE})
  endif(NOT DEFINED ${NAME})

endmacro(set_variable NAME VALUE)

################################################################################
# Set cache variable
################################################################################

macro(set_cache_variable NAME VALUE TYPE DESCRIPTION)

  if(NOT DEFINED ${NAME})
    set(${NAME} ${VALUE} CACHE ${TYPE} "${DESCRIPTION}")
  endif(NOT DEFINED ${NAME})

endmacro(set_cache_variable NAME VALUE TYPE DESCRIPTION)

################################################################################
# Set operating system variable
################################################################################

macro(set_os_variable NAME WIN32VALUE LINUXVALUE)

  if(NOT DEFINED ${NAME})
    if(WIN32)
      set(${NAME} ${WIN32VALUE})
    else(WIN32)
      set(${NAME} ${LINUXVALUE})
    endif(WIN32)
  endif(NOT DEFINED ${NAME})

endmacro(set_os_variable NAME WIN32VALUE LINUXVALUE)

################################################################################
# Set operating system cache variable
################################################################################

macro(set_os_cache_variable NAME WIN32VALUE LINUXVALUE TYPE DESCRIPTION)

  if(NOT DEFINED ${NAME})
    if(WIN32)
      set(${NAME} ${WIN32VALUE} CACHE ${TYPE} "${DESCRIPTION}")
    else(WIN32)
      set(${NAME} ${LINUXVALUE} CACHE ${TYPE} "${DESCRIPTION}")
    endif(WIN32)
  endif(NOT DEFINED ${NAME})

endmacro(set_os_cache_variable NAME WIN32VALUE LINUXVALUE TYPE DESCRIPTION)

################################################################################
# Display variables
################################################################################

function(display_variables)

  get_cmake_property(VARIABLES_NAMES VARIABLES)
  list(SORT VARIABLES_NAMES)
  foreach(VARIABLE_NAME IN LISTS VARIABLES_NAMES)
    message(STATUS "${VARIABLE_NAME} = ${${VARIABLE_NAME}}")
  endforeach(VARIABLE_NAME IN LISTS VARIABLES_NAMES)

endfunction(display_variables)

################################################################################
# Display matching variables
################################################################################

function(display_matching_variables PATTERN)

  get_cmake_property(VARIABLES_NAMES VARIABLES)
  list(SORT VARIABLES_NAMES)
  foreach(VARIABLE_NAME IN LISTS VARIABLES_NAMES)
    string(REGEX MATCH ".*${PATTERN}.*" PATTERN_FOUND ${VARIABLE_NAME})
    if(PATTERN_FOUND)
      message(STATUS "${VARIABLE_NAME} = ${${VARIABLE_NAME}}")
    endif(PATTERN_FOUND)
  endforeach(VARIABLE_NAME IN LISTS VARIABLES_NAMES)

endfunction(display_matching_variables PATTERN)
