set(DELIVERY_DIR "${CMAKE_BINARY_DIR}/externals/${CMAKE_FIND_PACKAGE_NAME}")

message("--->  Find ${CMAKE_FIND_PACKAGE_NAME} in ${DELIVERY_DIR}")

file(GLOB_RECURSE LIBS_LIST ${DELIVERY_DIR}/*.a)


foreach(lib ${LIBS_LIST})
    get_filename_component(fileName ${lib} NAME_WE)

    #remove debug extension if exist
    string(REGEX REPLACE "(.*)d" "\\1" fileName ${fileName})


    STRING(REPLACE ".lib" ".dll" dll ${lib})

    if (NOT TARGET ${fileName})
        message("         IMPORT : ${fileName}")
        add_library(${fileName} STATIC IMPORTED)
        set_target_properties(${fileName} PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${DELIVERY_DIR}/include)

        set_target_properties(${fileName} PROPERTIES APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
        set_target_properties(${fileName} PROPERTIES IMPORTED_IMPLIB_DEBUG "${lib}")
        set_target_properties(${fileName} PROPERTIES IMPORTED_LOCATION_DEBUG "${dll}")
    else()
        message("         target ${fileName} already imported")
    endif()
endforeach()

set(ALIAS_LIST "")
set(LIBS_LIST_STRING "")
set(DLLS_LIST_STRING "")
