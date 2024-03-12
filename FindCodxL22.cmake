cmake_minimum_required(VERSION 3.18)

set(DELIVERY_DIR "C:/tt4-cmake/Starlinx/external/CODX/L22ED6/Delivery")

message("Endroit : ${DELIVERY_DIR}")
message("Package name running : ${CMAKE_FIND_PACKAGE_NAME}")

add_library(${CMAKE_FIND_PACKAGE_NAME} SHARED IMPORTED)

file(GLOB_RECURSE LIBS_LIST ${DELIVERY_DIR}/*.lib)
file(GLOB_RECURSE DLLS_LIST ${DELIVERY_DIR}/*.dll)

foreach(dll ${DLLS_LIST})
	get_filename_component(fileName ${dll} NAME_WE)
  message("File Name : ${fileName}")
	list(APPEND ALIAS_LIST ${fileName})
endforeach()

list(REMOVE_DUPLICATES ALIAS_LIST)
list(REMOVE_ITEM ALIAS_LIST ${CMAKE_FIND_PACKAGE_NAME})

message("List of Alias : ${ALIAS_LIST}")

foreach(alias ${ALIAS_LIST})
	add_library(${alias} ALIAS ${CMAKE_FIND_PACKAGE_NAME})
endforeach()

set_target_properties(${CMAKE_FIND_PACKAGE_NAME} PROPERTIES
  IMPORTED_IMPLIB_DEBUG ${LIBS_LIST}
  IMPORTED_LOCATION_DEBUG ${DLLS_LIST}
  INTERFACE_INCLUDE_DIRECTORIES ${DELIVERY_DIR}/include
)

set(ALIAS_LIST "")