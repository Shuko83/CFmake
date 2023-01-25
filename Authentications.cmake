################################################################################
# Get authentication
################################################################################

function(get_authentication NAME)

  # Retrieve from environment variables

  if(DEFINED ENV{${NAME}_USERNAME})
    set(${NAME}_USERNAME $ENV{${NAME}_USERNAME} PARENT_SCOPE)
  else(DEFINED ENV{${NAME}_USERNAME})
    set(${NAME}_USERNAME ${NAME}_USERNAME-NOTFOUND)
  endif(DEFINED ENV{${NAME}_USERNAME})

  if(DEFINED ENV{${NAME}_PASSWORD})
    set(${NAME}_PASSWORD $ENV{${NAME}_PASSWORD} PARENT_SCOPE)
  else(DEFINED ENV{${NAME}_PASSWORD})
    set(${NAME}_PASSWORD ${NAME}_PASSWORD-NOTFOUND)
  endif(DEFINED ENV{${NAME}_PASSWORD})

  # Retrieve from file

  if(NOT ${NAME}_USERNAME OR NOT ${NAME}_PASSWORD)
    include(Logins)
  endif(NOT ${NAME}_USERNAME OR NOT ${NAME}_PASSWORD)

  # Check authentication

  if(NOT ${NAME}_USERNAME OR NOT ${NAME}_PASSWORD)
    message(FATAL_ERROR "${NAME}_USERNAME and ${NAME}_PASSWORD variables undefined.\n"
      "Please add ${NAME}_USERNAME and ${NAME}_PASSWORD to environment variables or Logins.cmake file.\n"
      "set(${NAME}_USERNAME \"value\")\n"
      "set(${NAME}_PASSWORD \"value\")\n")
  endif(NOT ${NAME}_USERNAME OR NOT ${NAME}_PASSWORD)

endfunction(get_authentication)
