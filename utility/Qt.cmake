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

#qt {
#    qtGreaterThan( 5.9.9 ) : BUILD_MKSPEC = $${TOOLKIT.mkspec}-QT$${QT_VERSION_MM}
#    else : BUILD_MKSPEC = $${TOOLKIT.mkspec}-QT$$[QT_VERSION]
#}
set(CSTOOLKIT_BUILD_MKSPECS_QT "${CSTOOLKIT_BUILD_MKSPECS}-Qt${QT_VERSION_MM}")
set(Qt5_INSTALL_PREFIX "${_qt5Core_install_prefix}")

if(WIN32)
    if (NOT TARGET Qt5::windeployqt)
        add_executable(Qt5::windeployqt IMPORTED)

        set(imported_location "${_qt5Core_install_prefix}/bin/windeployqt.exe")
        _qt5_Core_check_file_exists(${imported_location})

        set_target_properties(Qt5::windeployqt PROPERTIES
            IMPORTED_LOCATION ${imported_location}
        )
        # For Deploy feature
        get_target_property(QT_DEPLOY_TOOL Qt5::windeployqt LOCATION)
    endif()
endif()

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

        set(_moc_include_file ${CMAKE_CURRENT_BINARY_DIR}/generated/moc/mocinclude-$<LOWER_CASE:$<CONFIG>>.tmp)

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
            COMMAND ${CMAKE_COMMAND} ARGS -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/generated/uic
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

# qt6_generate_deploy_app_script()
# partially copied from Qt6CoreMacros.cmake
function(cstoolkit_qt_generate_deploy_app_script)
    # We use a TARGET keyword option instead of taking the target as the first
    # positional argument. This is to keep open the possibility of deploying
    # an app for which we don't have a target (e.g. an application from a
    # third party project that the caller may want to include in their own
    # package). We would add an EXECUTABLE keyword for that, which would be
    # mutually exclusive with the TARGET keyword.
    set(no_value_options
        NO_TRANSLATIONS
        NO_COMPILER_RUNTIME
        NO_UNSUPPORTED_PLATFORM_ERROR
    )
    set(single_value_options
        TARGET
        INSTALL_DIR
        OUTPUT_SCRIPT
    )
    set(multi_value_options
        RUNTIME_DEPENDENCIES
        DEPLOY_TOOL_OPTIONS
    )
    cmake_parse_arguments(PARSE_ARGV 0 arg
        "${no_value_options}" "${single_value_options}" "${multi_value_options}"
    )
    if(arg_UNPARSED_ARGUMENTS)
        message(FATAL_ERROR "Unexpected arguments: ${arg_UNPARSED_ARGUMENTS}")
    endif()
    if(NOT arg_TARGET)
        message(FATAL_ERROR "TARGET must be specified")
    endif()
    if(NOT arg_INSTALL_DIR)
        message(FATAL_ERROR "INSTALL_DIR must be specified")
    endif()
    if(NOT arg_OUTPUT_SCRIPT)
        message(FATAL_ERROR "OUTPUT_SCRIPT must be specified")
    endif()

    set(deploy_script "${CMAKE_CURRENT_BINARY_DIR}/${arg_TARGET}_qt_deploy-$<LOWER_CASE:$<CONFIG>>.cmake")

    if(WIN32)
        file(READ ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/../templates/Target_qt_deploy.cmake.in _Target_qt_deploy_content)
        string(CONFIGURE "${_Target_qt_deploy_content}" _content @ONLY)
        file(GENERATE OUTPUT ${deploy_script} CONTENT "${_content}")
    elseif(NOT arg_NO_UNSUPPORTED_PLATFORM_ERROR)
        # Error out by default unless the project opted out of the error.
        message(FATAL_ERROR
            "Support for installing runtime dependencies is not implemented for "
            "this target platform (${CMAKE_SYSTEM_NAME})."
        )
    else()
        file(GENERATE OUTPUT ${deploy_script}
            CONTENT "
message(STATUS
    \"Skipping Qt runtime deployment steps. \"
    \"Support for installing runtime dependencies is not implemented for \"
    \"this target platform (${CMAKE_SYSTEM_NAME}).\"
)")
    endif()

    set(${arg_OUTPUT_SCRIPT} "${deploy_script}" PARENT_SCOPE)
endfunction()

endif()
