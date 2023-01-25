################################################################################
# Documentation definition
################################################################################

function(define_documentation)

  # Check documentation

  if(NOT BUILD_DOCUMENTATION)
    return()
  endif(NOT BUILD_DOCUMENTATION)

  # Check Doxygen

  find_package(Doxygen REQUIRED dot)
  if(NOT DOXYGEN_FOUND)
    return()
  endif(NOT DOXYGEN_FOUND)

  # Check sub-project

  if(NOT PROJECT_NAME STREQUAL CMAKE_PROJECT_NAME)
    return()
  endif(NOT PROJECT_NAME STREQUAL CMAKE_PROJECT_NAME)

  # Set Doxygen variables

  set(DOXYGEN_PROJECT_NAME ${CMAKE_PROJECT_NAME})
  set(DOXYGEN_PROJECT_NUMBER ${CMAKE_PROJECT_VERSION})
  set(DOXYGEN_INPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR})

  # Set Doxygen configuration

  if(NOT ENABLE_DOCUMENTATION_FULL)
    set(DOXYGEN_IN ${CMAKE_MODULE_TEMPLATES_PATH}/ExternalDocumentation.doxyfile.in)
  else(NOT ENABLE_DOCUMENTATION_FULL)
    set(DOXYGEN_IN ${CMAKE_MODULE_TEMPLATES_PATH}/InternalDocumentation.doxyfile.in)
  endif(NOT ENABLE_DOCUMENTATION_FULL)
  set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Documentation.doxyfile)
  configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
  add_custom_target(${CMAKE_PROJECT_NAME}Documentation ALL
    COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
  install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html DESTINATION doc)

endfunction(define_documentation)
