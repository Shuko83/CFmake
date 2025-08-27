set(DELIVERY_DIR "${CMAKE_BINARY_DIR}/externals/${CMAKE_FIND_PACKAGE_NAME}")

message("--->  Find ${CMAKE_FIND_PACKAGE_NAME} in ${DELIVERY_DIR}")


if (NOT TARGET ${CMAKE_FIND_PACKAGE_NAME})
  message("         IMPORT : ${CMAKE_FIND_PACKAGE_NAME}, ${DELIVERY_DIR}")
  add_library(${CMAKE_FIND_PACKAGE_NAME} INTERFACE)
  set_target_properties(${CMAKE_FIND_PACKAGE_NAME} PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${DELIVERY_DIR})
else()
  message("         target ${CMAKE_FIND_PACKAGE_NAME} already imported")
endif()

# handle the QUIETLY and REQUIRED arguments and set xxx_FOUND to TRUE if
# all listed variables are TRUE
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(${CMAKE_FIND_PACKAGE_NAME} DEFAULT_MSG)