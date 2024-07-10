set_property(GLOBAL PROPERTY AUTOGEN_SOURCE_GROUP "Generated Files")

# Finding QT_ROOT
if(NOT Qt5_ROOT AND NOT Qt5_DIR)
    if(DEFINED ENV{QTDIR})
        set(Qt5_ROOT $ENV{QTDIR})
    else()
        find_program(CSTOOLKIT_QMAKE_EXECUTABLE qmake)
        if(NOT CSTOOLKIT_QMAKE_EXECUTABLE STREQUAL "CSTOOLKIT_QMAKE_EXECUTABLE-NOTFOUND")
            execute_process(COMMAND "${CSTOOLKIT_QMAKE_EXECUTABLE}" -query QT_INSTALL_PREFIX
                RESULT_VARIABLE _return_code
                OUTPUT_VARIABLE _output
                OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_STRIP_TRAILING_WHITESPACE)
            if(NOT return_code)
                set(Qt5_ROOT "${_output}")
            endif()
            unset(_return_code)
            unset(_output)
        endif()
    endif()
endif()

find_package(Qt5 COMPONENTS Widgets QUIET) # Widgets car on a besoin de uic.exe

if(NOT Qt5_FOUND)

function(cstoolkit_qt_wrap_cpp outfiles)
    message(SEND_ERROR "CSToolkit: Qt was not found, unable to use cstoolkit_qt_wrap_cpp")
endfunction()

function(cstoolkit_qt_wrap_ui outfiles)
    message(SEND_ERROR "CSToolkit: Qt was not found, unable to use cstoolkit_qt_wrap_ui")
endfunction()

function(cstoolkit_qt_add_resources outcppfiles outrscfiles)
    message(SEND_ERROR "CSToolkit: Qt was not found, unable to use cstoolkit_qt_add_resources")
endfunction()

function(cstoolkit_filter_moc)
    message(SEND_ERROR "CSToolkit: Qt was not found, unable to use cstoolkit_filter_moc")
endfunction()

else()
set(QT_VERSION "${Qt5_VERSION}")
set(QT_VERSION_MM "${Qt5_VERSION_MAJOR}.${Qt5_VERSION_MINOR}")

set(CSTOOLKIT_BUILD_MKSPECS_QT "${CSTOOLKIT_BUILD_MKSPECS}-Qt${QT_VERSION_MM}")
set(Qt5_INSTALL_PREFIX "${_qt5Core_install_prefix}")

#qt {
#    qtGreaterThan( 5.9.9 ) : BUILD_MKSPEC = $${TOOLKIT.mkspec}-QT$${QT_VERSION_MM}
#    else : BUILD_MKSPEC = $${TOOLKIT.mkspec}-QT$$[QT_VERSION]
#}
if(MSVC)
    # hushes some known Qt warnings
    set_target_properties(Qt5::Core PROPERTIES "INTERFACE_COMPILE_OPTIONS" "-wd4127;-wd4512;-wd4714;$<$<NOT:$<CONFIG:Debug>>:-wd4718>")
endif()

# qt5_wrap_cpp(outfiles inputfile ... )
# partially copied from Qt5CoreMacros.cmake
function(cstoolkit_qt_wrap_cpp outfiles)
    
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

    endif()

    file (GENERATE
        OUTPUT ${_moc_include_file}
        CONTENT "${targetincludes}${moc_options}\n"
    )

    foreach(it ${moc_files})
        get_filename_component(outfile ${it} NAME_WE)
        set(outfile ${CMAKE_CURRENT_BINARY_DIR}/generated/moc/moc_${outfile}.cpp)
        get_filename_component(infile ${it} ABSOLUTE)
        cmake_path(RELATIVE_PATH infile BASE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} OUTPUT_VARIABLE relpath)

        file(STRINGS ${infile} _contains_macro REGEX "Q_OBJECT|Q_GADGET|Q_NAMESPACE|Q_NAMESPACE_EXPORT")

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
function(cstoolkit_qt_wrap_ui outfiles)
    set(options)
    set(oneValueArgs TARGET)
    set(multiValueArgs OPTIONS)

    cmake_parse_arguments(_WRAP_UI "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    set(ui_files ${_WRAP_UI_UNPARSED_ARGUMENTS})
    set(ui_options ${_WRAP_UI_OPTIONS})
    set(ui_target ${_WRAP_CPP_TARGET})

    if(ui_target AND ui_files)
        add_custom_command(TARGET ${ui_target}
            PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/generated/uic
        )
    endif()

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

# qt5_add_resources(outfiles inputfile ... )
# partially copied from Qt5CoreMacros.cmake
function(cstoolkit_qt_add_resources outcppfiles outrscfiles)
    set(options)
    set(oneValueArgs)
    set(multiValueArgs OPTIONS)

    cmake_parse_arguments(_RCC "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    set(rcc_files ${_RCC_UNPARSED_ARGUMENTS})
    set(rcc_options ${_RCC_OPTIONS})

    foreach(it ${rcc_files})
        get_filename_component(outfilename ${it} NAME_WE)
        get_filename_component(infile ${it} ABSOLUTE)
        set(outfile ${CMAKE_CURRENT_BINARY_DIR}/generated/rcc/qrc_${outfilename}.cpp)
        cmake_path(RELATIVE_PATH infile BASE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} OUTPUT_VARIABLE relpath)
        set(out_depends)
        set(_rc_depends)

        # Parsing qrc
        #_qt5_parse_qrc_file(${infile} _out_depends _rc_depends)
        if(EXISTS "${infile}")
            get_filename_component(rc_path ${infile} PATH)
            #  parse file for dependencies
            #  all files are absolute paths or relative to the location of the qrc file
            file(READ "${infile}" RC_FILE_CONTENTS)
            string(REGEX MATCHALL "<file[^<]+" RC_FILES "${RC_FILE_CONTENTS}")
            foreach(RC_FILE ${RC_FILES})
                string(REGEX REPLACE "^<file[^>]*>" "" RC_FILE "${RC_FILE}")
                if(NOT IS_ABSOLUTE "${RC_FILE}")
                    set(RC_FILE "${rc_path}/${RC_FILE}")
                endif()
                list(APPEND _rc_depends ${RC_FILE})
            endforeach()
            # Since this cmake macro is doing the dependency scanning for these files,
            # let's make a configured file and add it as a dependency so cmake is run
            # again when dependencies need to be recomputed.
            set(out_depends ${CMAKE_CURRENT_BINARY_DIR}/generated/rcc/${outfilename}.qrc.depends)
            configure_file("${infile}" "${out_depends}" COPYONLY)
        endif()

        set_source_files_properties(${infile} PROPERTIES SKIP_AUTORCC ON)

        add_custom_command(OUTPUT ${outfile}
                        COMMAND ${Qt5Core_RCC_EXECUTABLE}
                        ARGS ${rcc_options} --name ${outfilename} --output ${outfile} ${infile}
                        MAIN_DEPENDENCY ${infile}
                        DEPENDS ${_rc_depends} "${out_depends}" VERBATIM
                        COMMENT "RCC ${relpath}")
        set_source_files_properties(${outfile} PROPERTIES SKIP_AUTOMOC ON)
        set_source_files_properties(${outfile} PROPERTIES SKIP_AUTOUIC ON)
        list(APPEND ${outcppfiles} ${outfile})
        list(APPEND ${outrscfiles} ${_rc_depends})
    endforeach()
    set(${outcppfiles} ${${outcppfiles}} PARENT_SCOPE)
    set(${outrscfiles} ${${outrscfiles}} PARENT_SCOPE)
endfunction()

# filter_moc : check all sources files if a moc is included
# if it's the case, remove this moc from the list
# This process was made by QMake
function(cstoolkit_filter_moc)

    set(sourceFiles ${ARGV})

    set(MOC_REGEX "^#include.[<\"](moc_.+)[>\"]$")

    # Parse each line of source file
    foreach(sourceFile ${sourceFiles})
        file(READ ${sourceFile} fileContent)
        string(REPLACE "\n" ";" fileContent ${fileContent})
        
        # Check if the line match the regex for the include
        foreach(line ${fileContent})
            string(REGEX MATCH ${MOC_REGEX} HAS_MATCHED ${line})

            # If matched, add the moc to the exclude list
            if (HAS_MATCHED)
                list(APPEND MOC_TO_EXCLUDE ${CMAKE_MATCH_1})
            endif()
        endforeach()           
    endforeach()
    
    unset(HAS_MATCHED)

    set(TEMP_MOC_FILES ${MOC_FILES})

    # Check for each moc files if they match with moc to exclude
    foreach(moc ${MOC_FILES})
        foreach(mocToExclude ${MOC_TO_EXCLUDE})
            string(REGEX MATCH "(.+${mocToExclude})" HAS_MATCHED ${moc})
            if (HAS_MATCHED)
                list(REMOVE_ITEM TEMP_MOC_FILES ${CMAKE_MATCH_1})
            endif()
        endforeach()
    endforeach()

    set(MOC_FILES ${TEMP_MOC_FILES} PARENT_SCOPE)
endfunction()

endif()