#set(CMAKE_AUTOMOC ON)
#set(CMAKE_AUTORCC ON)
#set(CMAKE_AUTOUIC ON)

set_property(GLOBAL PROPERTY AUTOGEN_SOURCE_GROUP "Generated Files")

set(Qt5_VERSION "5.9.6" CACHE INTERNAL "Qt5 version")
set(Qt5_ARCH "msvc2015_64" CACHE INTERNAL "Qt5 architecture")
set(Qt5_DIR "C:/Qt/Qt${Qt5_VERSION}/${Qt5_VERSION}/${Qt5_ARCH}/lib/cmake/Qt5")

find_package(Qt5 ${Qt5_VERSION} COMPONENTS Widgets)

#qt {
#  
#    QMAKE_CXXFLAGS *= -wd4127 -wd4512 -wd4714                               # hushes some known Qt warnings
#    QMAKE_CXXFLAGS_RELEASE *= -wd4718                                       # hushes some known Qt warnings in release mode
#    QMAKE_CXXFLAGS_RELEASE_WITH_DEBUGINFO *= -wd4718
#}

# qt5_wrap_ui(outfiles inputfile ... )
# partially copied from Qt5WidgetsMacros.cmake
function(cstoolkit_qt5_wrap_cpp outfiles)
    
    set(_moc_flags)
    
    if (WIN32)
        set(_moc_flags ${_moc_flags} -DWIN32)
        set(_moc_flags ${_moc_flags} -D_WINDOWS)
        set(_moc_flags ${_moc_flags} $<$<NOT:$<CONFIG:Debug>>:-DNDEBUG>)
    endif()
    if(MSVC)
        set(_moc_flags ${_moc_flags} --compiler-flavor=msvc)
    endif()

    set(options)
    set(oneValueArgs TARGET)
    set(multiValueArgs OPTIONS DEPENDS)

    cmake_parse_arguments(_WRAP_CPP "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    set(moc_files ${_WRAP_CPP_UNPARSED_ARGUMENTS})
    set(moc_options ${_WRAP_CPP_OPTIONS})
    set(moc_target ${_WRAP_CPP_TARGET})
    set(moc_depends ${_WRAP_CPP_DEPENDS})

    set(_moc_include_file ${CMAKE_CURRENT_BINARY_DIR}/generated/moc/mocinclude.tmp)

    if(moc_target)
        set(targetdefines "$<LIST:TRANSFORM,$<TARGET_PROPERTY:${moc_target},COMPILE_DEFINITIONS>,PREPEND,-D>")

        set(_moc_flags ${targetdefines} ${_moc_flags})

        # get include dirs
        set(targetincludes "$<TARGET_PROPERTY:${moc_target},INCLUDE_DIRECTORIES>")
        set(targetincludes "$<$<BOOL:${targetincludes}>:-I$<JOIN:${targetincludes},\n-I>\n>")

        set(_moc_include_file ${CMAKE_CURRENT_BINARY_DIR}/generated/moc/mocinclude$<CONFIG>.tmp)

        file (GENERATE
            OUTPUT ${_moc_include_file}
            CONTENT "${targetincludes}${moc_options}\n"
        )
    else()
        file(WRITE ${_moc_include_file} "${moc_options}\n")
    endif()

    foreach(it ${moc_files})
        get_filename_component(outfile ${it} NAME_WE)
        set(outfile ${CMAKE_CURRENT_BINARY_DIR}/generated/moc/moc_${outfile}.cpp)
        get_filename_component(infile ${it} ABSOLUTE)
        cmake_path(RELATIVE_PATH infile BASE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} OUTPUT_VARIABLE relpath)

        file(STRINGS ${infile} _contains_macro REGEX "Q_OBJECT|Q_GADGET|Q_NAMESPACE|Q_NAMESPACE_EXPORT")
        message("REGEX parse of ${it}: ${_contains_macro}")
        if(NOT _contains_macro)
            continue()
        endif()

        add_custom_command(OUTPUT ${outfile}
            COMMAND ${Qt5Core_MOC_EXECUTABLE}
            ARGS ${_moc_flags} @${_moc_include_file} "${infile}" -o "${outfile}"
            MAIN_DEPENDENCY ${infile} DEPENDS ${moc_depends} ${_moc_include_file}
            COMMENT "MOC ${relpath}"
            COMMAND_EXPAND_LISTS)
        set_source_files_properties(${infile} PROPERTIES SKIP_AUTOUIC ON)
        set_source_files_properties(${outfile} PROPERTIES SKIP_AUTOMOC ON)
        set_source_files_properties(${outfile} PROPERTIES SKIP_AUTOUIC ON)
        list(APPEND ${outfiles} ${outfile})
    endforeach()
    set(${outfiles} ${${outfiles}} PARENT_SCOPE)
endfunction()

# qt5_wrap_ui(outfiles inputfile ... )
# partially copied from Qt5WidgetsMacros.cmake
function(cstoolkit_qt5_wrap_ui outfiles)
    set(options)
    set(oneValueArgs)
    set(multiValueArgs OPTIONS)

    cmake_parse_arguments(_WRAP_UI "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    set(ui_files ${_WRAP_UI_UNPARSED_ARGUMENTS})
    set(ui_options ${_WRAP_UI_OPTIONS})

    foreach(it ${ui_files})
        get_filename_component(outfile ${it} NAME_WE)
        set(outfile ${CMAKE_CURRENT_BINARY_DIR}/generated/uic/ui_${outfile}.h)
        get_filename_component(infile ${it} ABSOLUTE)
        cmake_path(RELATIVE_PATH infile BASE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} OUTPUT_VARIABLE relpath)

        add_custom_command(OUTPUT ${outfile}
            COMMAND ${Qt5Widgets_UIC_EXECUTABLE}
            ARGS ${ui_options} -o ${outfile} ${infile}
            MAIN_DEPENDENCY ${infile} VERBATIM
            COMMENT "UIC ${relpath}")
        set_source_files_properties(${infile} PROPERTIES SKIP_AUTOUIC ON)
        set_source_files_properties(${outfile} PROPERTIES SKIP_AUTOMOC ON)
        set_source_files_properties(${outfile} PROPERTIES SKIP_AUTOUIC ON)
        list(APPEND ${outfiles} ${outfile})
    endforeach()

    set(${outfiles} ${${outfiles}} PARENT_SCOPE)
endfunction()
