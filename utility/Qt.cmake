set_property(GLOBAL PROPERTY AUTOGEN_SOURCE_GROUP "Generated Files")

find_package(Qt5 COMPONENTS Core OPTIONAL_COMPONENTS Widgets QUIET) # Widgets car on a besoin de uic.exe
if(NOT Qt5_FOUND OR NOT TARGET Qt5::Core)
    find_package(Qt5Core QUIET)
endif()

if(NOT Qt5_FOUND OR NOT TARGET Qt5::Core)
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
                    ERROR_QUIET
                    OUTPUT_STRIP_TRAILING_WHITESPACE)
                if(NOT return_code)
                    set(Qt5_ROOT "${_output}")
                endif()
                unset(_return_code)
                unset(_output)
            endif()
        endif()
    endif()

    find_package(Qt5 COMPONENTS Core OPTIONAL_COMPONENTS Widgets QUIET) # Widgets car on a besoin de uic.exe
    if(NOT Qt5_FOUND OR NOT TARGET Qt5::Core)
        find_package(Qt5Core QUIET)
    endif()
endif()

if(NOT Qt5_FOUND OR NOT TARGET Qt5::Core)

set(QT_VERSION "Qt5-NOTFOUND")
set(QT_VERSION_MM "${QT_VERSION}")

set(CSTOOLKIT_BUILD_MKSPECS_QT "${CSTOOLKIT_BUILD_MKSPECS}-${QT_VERSION}")

function(cstoolkit_qt_wrap_cpp outfiles)
    message(SEND_ERROR "CSToolkit: Qt was not found, unable to use cstoolkit_qt_wrap_cpp()")
endfunction()

function(cstoolkit_qt_wrap_ui outfiles)
    message(SEND_ERROR "CSToolkit: Qt was not found, unable to use cstoolkit_qt_wrap_ui()")
endfunction()

function(cstoolkit_qt_add_resources outcppfiles outrscfiles)
    message(SEND_ERROR "CSToolkit: Qt was not found, unable to use cstoolkit_qt_add_resources()")
endfunction()

function(cstoolkit_qt_generate_repc outfiles infile outputtype)
    message(SEND_ERROR "CSToolkit: Qt was not found, unable to use cstoolkit_qt_generate_repc()")
endfunction()

function(cstoolkit_filter_moc)
    message(SEND_ERROR "CSToolkit: Qt was not found, unable to use cstoolkit_filter_moc()")
endfunction()

function(cstoolkit_qt_generate_deploy_app_script)
    message(SEND_ERROR "CSToolkit: Qt was not found, unable to use cstoolkit_qt_generate_deploy_app_script()")
endfunction()

else() # Qt5 Found

set(QT_VERSION "${Qt5_VERSION}")
set(QT_VERSION_MM "${Qt5_VERSION_MAJOR}.${Qt5_VERSION_MINOR}")

if(Qt5_VERSION_MAJOR LESS_EQUAL 5 AND Qt5_VERSION_MINOR LESS_EQUAL 9)
    set(CSTOOLKIT_BUILD_MKSPECS_QT "${CSTOOLKIT_BUILD_MKSPECS}-Qt${QT_VERSION}")
else()
    set(CSTOOLKIT_BUILD_MKSPECS_QT "${CSTOOLKIT_BUILD_MKSPECS}-Qt${QT_VERSION_MM}")
endif()

set(Qt5_INSTALL_PREFIX "${_qt5Core_install_prefix}")

message(STATUS "CSToolkit: Selecting Qt ${QT_VERSION} in: ${Qt5_INSTALL_PREFIX}")

set(CMAKE_VS_DEBUGGER_ENVIRONMENT "PATH=${Qt5_INSTALL_PREFIX}/bin;%PATH%")

if(WIN32)
    if(NOT TARGET Qt5::windeployqt)
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
    set_property(TARGET Qt5::Core APPEND PROPERTY INTERFACE_COMPILE_OPTIONS -wd4127 -wd4512 -wd4714 $<$<NOT:$<CONFIG:Debug>>:-wd4718>)
endif()

# qt5_wrap_cpp(outfiles inputfile ... )
# partially copied from Qt5CoreMacros.cmake
function(cstoolkit_qt_wrap_cpp outfiles)
    
    set(_moc_flags)
    
    if(WIN32)
        set(_moc_flags ${_moc_flags} -DWIN32)
        set(_moc_flags ${_moc_flags} -D_WINDOWS)
        set(_moc_flags ${_moc_flags} $<$<NOT:$<CONFIG:Debug>>:-DNDEBUG>)
    endif()
    if(MSVC)
        set(_moc_flags ${_moc_flags} --compiler-flavor=msvc)
    endif()

    set(options TESTS)
    set(oneValueArgs TARGET)
    set(multiValueArgs OPTIONS DEPENDS)

    cmake_parse_arguments(PARSE_ARGV 1 _WRAP_CPP "${options}" "${oneValueArgs}" "${multiValueArgs}")

    set(moc_files ${_WRAP_CPP_UNPARSED_ARGUMENTS})
    set(moc_options ${_WRAP_CPP_OPTIONS})
    set(moc_target ${_WRAP_CPP_TARGET})
    set(moc_depends ${_WRAP_CPP_DEPENDS})

    if(_WRAP_CPP_TESTS)
        set(moc_tests "/tests")
        set(_moc_include_file ${CMAKE_CURRENT_BINARY_DIR}${moc_tests}/generated/moc/$<LOWER_CASE:$<CONFIG>>/mocinclude_${moc_target}.tmp)
    else()
        set(moc_tests)
        set(_moc_include_file ${CMAKE_CURRENT_BINARY_DIR}/generated/moc/$<LOWER_CASE:$<CONFIG>>/mocinclude.tmp)
    endif()

    #filter generator expression moc_files
    cstoolkit_genex_extract("${moc_files}" moc_files _null)

    if(moc_target)
        set(targetdefines "$<LIST:TRANSFORM,$<TARGET_PROPERTY:${moc_target},COMPILE_DEFINITIONS>,PREPEND,-D>")

        set(_moc_flags ${targetdefines} ${_moc_flags})

        # get include dirs
        set(targetincludes "$<TARGET_PROPERTY:${moc_target},INCLUDE_DIRECTORIES>")
        set(targetincludes "$<$<BOOL:${targetincludes}>:-I$<JOIN:${targetincludes},\n-I>\n>")
    endif()

    file(GENERATE
        OUTPUT ${_moc_include_file}
        CONTENT "${targetincludes}${moc_options}\n"
    )

    foreach(it ${moc_files})
        get_filename_component(outfile ${it} NAME_WE)
        get_filename_component(infile ${it} ABSOLUTE)
        get_filename_component(extension ${it} LAST_EXT)
        list(TRANSFORM CSTOOLKIT_CXX_SOURCE_FILE_EXTENSIONS PREPEND "." OUTPUT_VARIABLE CXX_SOURCES_EXTENSIONS)
        if(extension IN_LIST CXX_SOURCES_EXTENSIONS)
            set(outfile ${CMAKE_CURRENT_BINARY_DIR}${moc_tests}/generated/moc/$<LOWER_CASE:$<CONFIG>>/${outfile}.moc)
            set(COMMAND_DEPENDENCIES DEPENDS ${infile} ${moc_depends} ${_moc_include_file})
        else()
            set(outfile ${CMAKE_CURRENT_BINARY_DIR}${moc_tests}/generated/moc/$<LOWER_CASE:$<CONFIG>>/moc_${outfile}.cpp)
            set(COMMAND_DEPENDENCIES MAIN_DEPENDENCY ${infile} DEPENDS ${moc_depends} ${_moc_include_file})
        endif()
        cmake_path(RELATIVE_PATH infile BASE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} OUTPUT_VARIABLE relpath)

        file(STRINGS ${infile} _contains_macro REGEX "Q_OBJECT|Q_GADGET|Q_NAMESPACE|Q_NAMESPACE_EXPORT")
        file(STRINGS ${infile} _moc_includes REGEX "(^|\n)[ \t]*#[ \t]*include[ \t]+(\"(moc_.+\\.cpp|.+\\.moc)\"|<(moc_.+\\.cpp|.+\\.moc)>)")

        foreach(_moc_include ${_moc_includes})
            string(REGEX MATCH "\"(.+)\"|<(.+)>" HAS_MATCHED "${_moc_include}")
            if(CMAKE_MATCH_1)
                list(APPEND _filtered_mocs "${CMAKE_MATCH_1}")
            else()
                list(APPEND _filtered_mocs "${CMAKE_MATCH_2}")
            endif()
        endforeach()

        if(NOT _contains_macro)
            continue()
        endif()

        add_custom_command(OUTPUT ${outfile}
            COMMAND ${Qt5Core_MOC_EXECUTABLE}
            ARGS ${_moc_flags} @${_moc_include_file} "${infile}" -o "${outfile}"
            ${COMMAND_DEPENDENCIES}
            COMMENT "MOC ${relpath}"
            VERBATIM
            COMMAND_EXPAND_LISTS)
        set_source_files_properties(${infile} PROPERTIES SKIP_AUTOMOC ON)
        cstoolkit_set_genex_source_file_properties(${outfile} PROPERTIES SKIP_AUTOMOC ON)
        cstoolkit_set_genex_source_file_properties(${outfile} PROPERTIES SKIP_AUTOUIC ON)
        list(APPEND ${outfiles} ${outfile})
    endforeach()

    list(REMOVE_DUPLICATES _filtered_mocs)
    foreach(_moc ${${outfiles}})
        get_filename_component(_shortmoc "${_moc}" NAME)
        if("${_shortmoc}" IN_LIST _filtered_mocs)
            cstoolkit_set_genex_source_file_properties("${_moc}" PROPERTIES HEADER_FILE_ONLY TRUE)
        endif()
    endforeach()

    set(${outfiles} ${${outfiles}} PARENT_SCOPE)
endfunction()

if(NOT Qt5Widgets_UIC_EXECUTABLE)
function(cstoolkit_qt_wrap_ui outfiles)
    message(SEND_ERROR "CSToolkit: Qt5::uic was not found, unable to use cstoolkit_qt_wrap_ui()")
endfunction()
else()
# qt5_wrap_ui(outfiles inputfile ... )
# partially copied from Qt5WidgetsMacros.cmake
function(cstoolkit_qt_wrap_ui outfiles)
    set(options)
    set(oneValueArgs)
    set(multiValueArgs OPTIONS)

    cmake_parse_arguments(PARSE_ARGV 1 _WRAP_UI "${options}" "${oneValueArgs}" "${multiValueArgs}")

    set(ui_files ${_WRAP_UI_UNPARSED_ARGUMENTS})
    set(ui_options ${_WRAP_UI_OPTIONS})

    foreach(it ${ui_files})
        get_filename_component(outfile ${it} NAME_WE)
        set(outfile ${CMAKE_CURRENT_BINARY_DIR}/generated/uic/$<LOWER_CASE:$<CONFIG>>/ui_${outfile}.h)
        get_filename_component(infile ${it} ABSOLUTE)
        cmake_path(RELATIVE_PATH infile BASE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} OUTPUT_VARIABLE relpath)

        add_custom_command(OUTPUT ${outfile}
            COMMAND ${CMAKE_COMMAND} ARGS -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/generated/uic/$<LOWER_CASE:$<CONFIG>>
            COMMAND ${Qt5Widgets_UIC_EXECUTABLE}
            ARGS ${ui_options} -o ${outfile} ${infile}
            MAIN_DEPENDENCY ${infile}
            VERBATIM
            COMMENT "UIC ${relpath}")
        set_source_files_properties(${infile} PROPERTIES SKIP_AUTOUIC ON)
        cstoolkit_set_genex_source_file_properties(${outfile} PROPERTIES SKIP_AUTOMOC ON)
        cstoolkit_set_genex_source_file_properties(${outfile} PROPERTIES SKIP_AUTOUIC ON)
        list(APPEND ${outfiles} ${outfile})
    endforeach()

    set(${outfiles} ${${outfiles}} PARENT_SCOPE)
endfunction()
endif()

# qt5_add_resources(outfiles inputfile ... )
# partially copied from Qt5CoreMacros.cmake
function(cstoolkit_qt_add_resources outcppfiles outrscfiles)
    set(options)
    set(oneValueArgs)
    set(multiValueArgs OPTIONS)

    cmake_parse_arguments(PARSE_ARGV 2 _RCC "${options}" "${oneValueArgs}" "${multiValueArgs}")

    set(rcc_files ${_RCC_UNPARSED_ARGUMENTS})
    set(rcc_options ${_RCC_OPTIONS})

    foreach(it ${rcc_files})
        get_filename_component(outfilename ${it} NAME_WE)
        get_filename_component(infile ${it} ABSOLUTE)
        cmake_path(RELATIVE_PATH infile BASE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} OUTPUT_VARIABLE relpath)
        set(_rc_depends)

        # Parsing qrc
        #_qt5_parse_qrc_file(${infile} _out_depends _rc_depends)
        set(RC_TOTAL_FILE_SIZE 0) # Total size of all referenced resources file in octet
        if(EXISTS "${infile}")
            get_filename_component(rc_path ${infile} PATH)
            #  parse file for dependencies
            #  all files are absolute paths or relative to the location of the qrc file
            file(READ "${infile}" RC_FILE_CONTENTS)
            string(REGEX MATCHALL "<file[^<]+" RC_FILES "${RC_FILE_CONTENTS}")
            foreach(RC_FILE ${RC_FILES})
                string(REGEX REPLACE "^<file[^>]*>" "" RC_FILE "${RC_FILE}")
                cmake_path(ABSOLUTE_PATH RC_FILE BASE_DIRECTORY "${rc_path}" NORMALIZE)
                if(IS_DIRECTORY "${RC_FILE}")
                    file(GLOB_RECURSE RC_RECURSIVE_FILES CONFIGURE_DEPENDS "${RC_FILE}/*" )
                    foreach(RC_RECURSIVE_FILE ${RC_RECURSIVE_FILES})
                        file(SIZE "${RC_RECURSIVE_FILE}" RC_FILE_SIZE)
                        math(EXPR RC_TOTAL_FILE_SIZE "${RC_TOTAL_FILE_SIZE}+${RC_FILE_SIZE}")
                        list(APPEND _rc_depends "${RC_RECURSIVE_FILE}")
                    endforeach()
                elseif(EXISTS "${RC_FILE}")
                    list(APPEND _rc_depends "${RC_FILE}")
                    file(SIZE "${RC_FILE}" RC_FILE_SIZE)
                    math(EXPR RC_TOTAL_FILE_SIZE "${RC_TOTAL_FILE_SIZE}+${RC_FILE_SIZE}")
                else()
                    list(APPEND _rc_depends "${RC_FILE}")
                    message(NOTICE ${COLOR_YELLOW_BOLD} "CSToolkit: cstoolkit_qt_add_resources: Warning in '${infile}': Cannot find file '${RC_FILE}'" ${COLOR_RESET})
                endif()
            endforeach()
        endif()

        set_source_files_properties(${infile} PROPERTIES SKIP_AUTORCC ON)

        set(outdir ${CMAKE_CURRENT_BINARY_DIR}/generated/rcc/$<LOWER_CASE:$<CONFIG>>)

        if(RC_TOTAL_FILE_SIZE GREATER CSTOOLKIT_QT_BIG_RESOURCES_THRESHOLD) # big_resources
            # File name must be different between configuration because of CMake bug regarding $<TARGET_OBJECTS:${rcctarget}>
            # https://gitlab.kitware.com/cmake/cmake/-/issues/26601
            set(outfile ${outdir}/qrc_${outfilename}-$<LOWER_CASE:$<CONFIG>>${CMAKE_C_OUTPUT_EXTENSION})
            set(tmpoutfile ${outdir}/qrc_${outfilename}-$<LOWER_CASE:$<CONFIG>>.cpp)
            if(TARGET_NAME)
                if(TARGET_NAME STREQUAL outfilename)
                    set(rcctarget ${TARGET_NAME}_rcc)
                else()
                    set(rcctarget ${TARGET_NAME}_rcc_${outfilename})
                endif()
            else()
                set(rcctarget rcc_${outfilename})
            endif()
            add_custom_command(OUTPUT ${tmpoutfile}
                            COMMAND ${CMAKE_COMMAND} ARGS -E make_directory ${outdir}
                            COMMAND ${Qt5Core_RCC_EXECUTABLE}
                            ARGS ${rcc_options} --name ${outfilename} --pass 1 --output ${tmpoutfile} ${infile}
                            DEPENDS ${infile} ${_rc_depends}
                            VERBATIM
                            COMMENT "RCC PASS1 ${relpath}")

            add_library(${rcctarget} OBJECT ${tmpoutfile})
            set_target_properties(${rcctarget} PROPERTIES AUTOMOC OFF)
            set_target_properties(${rcctarget} PROPERTIES AUTOUIC OFF)
            target_link_libraries(${rcctarget} PUBLIC Qt5::Core)
            set_target_properties(${rcctarget} PROPERTIES FOLDER "QtBigResources")

            add_custom_command(OUTPUT ${outfile}
                            COMMAND ${Qt5Core_RCC_EXECUTABLE}
                            ARGS ${rcc_options} --name ${outfilename} --pass 2 --temp $<TARGET_OBJECTS:${rcctarget}> --output ${outfile} ${infile}
                            DEPENDS ${rcctarget} $<TARGET_OBJECTS:${rcctarget}>
                            VERBATIM
                            COMMENT "RCC PASS2 ${relpath}")
        else()
            set(outfile ${outdir}/qrc_${outfilename}.cpp)
            add_custom_command(OUTPUT ${outfile}
                            COMMAND ${CMAKE_COMMAND} ARGS -E make_directory ${outdir}
                            COMMAND ${Qt5Core_RCC_EXECUTABLE}
                            ARGS ${rcc_options} --name ${outfilename} --output ${outfile} ${infile}
                            MAIN_DEPENDENCY ${infile}
                            DEPENDS ${_rc_depends}
                            VERBATIM
                            COMMENT "RCC ${relpath}")
        endif()
        cstoolkit_set_genex_source_file_properties(${outfile} PROPERTIES SKIP_AUTOMOC ON)
        cstoolkit_set_genex_source_file_properties(${outfile} PROPERTIES SKIP_AUTOUIC ON)
        list(APPEND ${outcppfiles} ${outfile})
        list(APPEND ${outrscfiles} ${_rc_depends})
    endforeach()
    set(${outcppfiles} ${${outcppfiles}} PARENT_SCOPE)
    set(${outrscfiles} ${${outrscfiles}} PARENT_SCOPE)
endfunction()

# qt5_generate_repc(<VAR> rep_file output_type)
# partially copied from Qt5RemoteObjectsMacros.cmake
function(cstoolkit_qt_generate_repc outfiles infile outputtype)
    # get include dirs and flags
    get_filename_component(abs_infile ${infile} ABSOLUTE)
    get_filename_component(infile_name "${infile}" NAME)
    string(REPLACE ".rep" "" _infile_base ${infile_name})
    if(${outputtype} STREQUAL "SOURCE")
        set(_outfile_base "rep_${_infile_base}_source")
        set(_repc_args -o source)
    elseif(${outputtype} STREQUAL "MERGED")
        set(_outfile_base "rep_${_infile_base}_merged")
        set(_repc_args -o merged)
    else()
        set(_outfile_base "rep_${_infile_base}_replica")
        set(_repc_args -o replica)
    endif()
    set(_outfile_header "${CMAKE_CURRENT_BINARY_DIR}/${_outfile_base}.h")
    add_custom_command(OUTPUT ${_outfile_header}
        DEPENDS ${abs_infile}
        COMMAND ${Qt5RemoteObjects_REPC_EXECUTABLE} ${abs_infile} ${_repc_args} ${_outfile_header}
        VERBATIM)
    set_source_files_properties(${_outfile_header} PROPERTIES
                                                GENERATED TRUE
                                                SKIP_AUTOMOC ON
                                                SKIP_AUTOUIC ON)

    qt5_get_moc_flags(_moc_flags)
    # Make sure we get the compiler flags from the Qt5::RemoteObjects target (for includes)
    # (code adapted from QT5_GET_MOC_FLAGS)
    foreach(_current ${Qt5RemoteObjects_INCLUDE_DIRS})
        if("${_current}" MATCHES "\\.framework/?$")
            string(REGEX REPLACE "/[^/]+\\.framework" "" framework_path "${_current}")
            set(_moc_flags ${_moc_flags} "-F${framework_path}")
        else()
            set(_moc_flags ${_moc_flags} "-I${_current}")
        endif()
    endforeach()

    set(_moc_outfile "${CMAKE_CURRENT_BINARY_DIR}/moc_${_outfile_base}.cpp")
    qt5_create_moc_command(${_outfile_header} ${_moc_outfile} "${_moc_flags}" "" "" "")
    list(APPEND ${outfiles} "${_outfile_header}" ${_moc_outfile})
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
        message(SEND_ERROR "CSToolkit: cstoolkit_qt_generate_deploy_app_script(): Unkown arguments \"${arg_UNPARSED_ARGUMENTS}\"")
        return()
    endif()
    if(NOT arg_TARGET)
        message(SEND_ERROR "CSToolkit: cstoolkit_qt_generate_deploy_app_script(): Missing mandatory parameter TARGET")
        return()
    endif()
    if(NOT arg_INSTALL_DIR)
        message(SEND_ERROR "CSToolkit: cstoolkit_qt_generate_deploy_app_script(): Missing mandatory parameter INSTALL_DIR")
        return()
    endif()
    if(NOT arg_OUTPUT_SCRIPT)
        message(SEND_ERROR "CSToolkit: cstoolkit_qt_generate_deploy_app_script(): Missing mandatory parameter OUTPUT_SCRIPT")
        return()
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
