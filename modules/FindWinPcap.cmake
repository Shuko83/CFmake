cmake_minimum_required(VERSION 3.18)

set(DELIVERY_DIR "${CMAKE_BINARY_DIR}/externals/${CMAKE_FIND_PACKAGE_NAME}")

message("--->  Find ${CMAKE_FIND_PACKAGE_NAME} in ${DELIVERY_DIR}")

file(GLOB_RECURSE LIBS_LIST ${DELIVERY_DIR}/*.lib)


foreach(lib ${LIBS_LIST})
    get_filename_component(fileName ${lib} NAME_WE)

    #remove debug extension if exist
    string(REGEX REPLACE "(.+)d$" "\\1" fileName ${fileName})
    set(dirName ${fileName})

    STRING(REPLACE ".lib" ".dll" dll ${lib})
    MESSAGE("Lib : ${lib}")

    if (NOT TARGET ${fileName})
      message("         IMPORT : ${fileName}")
      add_library(${fileName} SHARED IMPORTED)
      set_target_properties(${fileName} PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${DELIVERY_DIR}/include/)
    endif()

    if(lib MATCHES "debug")
      set_target_properties(${fileName} PROPERTIES APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
      set_target_properties(${fileName} PROPERTIES IMPORTED_IMPLIB_DEBUG "${lib}")
      set_target_properties(${fileName} PROPERTIES IMPORTED_LOCATION_DEBUG "${dll}")
    else()
      set_target_properties(${fileName} PROPERTIES APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
      set_target_properties(${fileName} PROPERTIES IMPORTED_IMPLIB_RELEASE "${lib}")
      set_target_properties(${fileName} PROPERTIES IMPORTED_LOCATION_RELEASE "${dll}")
    endif()
endforeach()

set(ALIAS_LIST "")
set(LIBS_LIST_STRING "")
set(DLLS_LIST_STRING "")

set(WinPcap_RUNTIME_DLLS
    "${WinPcap_ROOT}/${CSTOOLKIT_BUILD_MKSPECS}/npptools.dll"
    "${WinPcap_ROOT}/${CSTOOLKIT_BUILD_MKSPECS}/Packet.dll"
)

# handle the QUIETLY and REQUIRED arguments and set xxx_FOUND to TRUE if
# all listed variables are TRUE
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(${CMAKE_FIND_PACKAGE_NAME} DEFAULT_MSG WinPcap_RUNTIME_DLLS)
