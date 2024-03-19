cmake_minimum_required(VERSION 3.18)
 
set(DELIVERY_DIR "C:/tt4-cmake/Starlinx/Modules/DLPX/HI/Models/MdlpConsolidated/Delivery")
 

 
#add_library(${CMAKE_FIND_PACKAGE_NAME} SHARED IMPORTED)
 
file(GLOB_RECURSE LIBS_LIST ${DELIVERY_DIR}/*.lib)
#file(GLOB_RECURSE DLLS_LIST ${DELIVERY_DIR}/*.dll)
 
foreach(lib ${LIBS_LIST})
if (${lib} MATCHES ".*.elease.*")
continue()
endif()
	get_filename_component(fileName ${lib} NAME_WE)
  string(REGEX REPLACE "(.*)d" "\\1" fileName ${fileName})


  STRING(REPLACE ".lib" ".dll" dll ${lib})

  if (NOT TARGET ${fileName})
add_library(${fileName} SHARED IMPORTED)
  set_property(TARGET ${fileName} APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
  set_target_properties(${fileName} PROPERTIES IMPORTED_IMPLIB_DEBUG "${lib}")
  set_target_properties(${fileName} PROPERTIES IMPORTED_LOCATION_DEBUG "${dll}")
  set_target_properties(${fileName} PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${DELIVERY_DIR}/include;${DELIVERY_DIR}/include/MDlpConsolidated/Objects;${DELIVERY_DIR}/include/MDlpConsolidated/OrderReaders;${DELIVERY_DIR}/include/MDlpConsolidated/OrderWriters;${DELIVERY_DIR}/include/MDlpConsolidated/ReportReaders;${DELIVERY_DIR}/include/MDlpConsolidated/ReportWriters")
endif()
endforeach()
 
set(ALIAS_LIST "")
set(LIBS_LIST_STRING "")
set(DLLS_LIST_STRING "")
