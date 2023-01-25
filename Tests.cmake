################################################################################
# Tests definition
################################################################################

function(define_tests)

  # Check tests

  if(NOT BUILD_TESTING)
    return()
  endif(NOT BUILD_TESTING)

  # Check GoogleTest

  if(NOT GTest_FOUND)
    return()
  endif(NOT GTest_FOUND)

  # Set GoogleTest configuration

  include(GoogleTest)
  include(CTest)
  enable_testing()
  add_subdirectory(test)

endfunction(define_tests)
