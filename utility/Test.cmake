# cstoolkit_add_test(<target> <framework> [FILES_MODE <mode>]
#                                         [SOURCES_DIRS <dirs>] [DATA_DIRS <dirs>]
#                                         [SOURCES_FILES <files>] [DATA_FILES <files>]
#                                         [EXCLUDE_OBJECTS <files>])
#
# Generate one test executable per test source file discovered for a target
# previously created with cstoolkit_add_target(). Each generated test target is
# compiled with the same sources, headers, definitions, options, generated files
# and link libraries as the tested target, plus the test framework runtime.
#
# EXCLUDE_OBJECTS lists source file names (e.g. "main.cpp") whose compiled object
# is dropped from the reused tested-target objects. Use it when testing an
# executable target to remove the translation unit defining main(), which would
# otherwise clash with the main() provided by the test framework.
function(cstoolkit_add_test TARGET_NAME FRAMEWORK)

    # Parse arguments

    set(TEST_OPTIONS)
    set(TEST_UNIQUE FILES_MODE)
    set(TEST_MULTIPLE
        SOURCES_DIRS
        DATA_DIRS
        SOURCES_FILES
        DATA_FILES
        EXCLUDE_OBJECTS
    )
    cmake_parse_arguments(PARSE_ARGV 2 TEST "${TEST_OPTIONS}" "${TEST_UNIQUE}" "${TEST_MULTIPLE}")

    if(TARGET_NAME STREQUAL "")
        message(SEND_ERROR "CSToolkit: cstoolkit_add_test(): No NAME defined for target")
        return()
    endif()

    if(NOT TARGET ${TARGET_NAME})
        message(SEND_ERROR "CSToolkit: cstoolkit_add_test(${TARGET_NAME}): target does not exist, call cstoolkit_add_target() first")
        return()
    endif()

    if(DEFINED TEST_UNPARSED_ARGUMENTS)
        message(SEND_ERROR "CSToolkit: cstoolkit_add_test(${TARGET_NAME}): Unkown arguments \"${TEST_UNPARSED_ARGUMENTS}\"")
        return()
    endif()

    if(FRAMEWORK STREQUAL "")
        message(SEND_ERROR "CSToolkit: cstoolkit_add_test(${TARGET_NAME}): No FRAMEWORK defined (mandatory)")
        return()
    endif()

    if(NOT FRAMEWORK STREQUAL "QTEST")
        message(SEND_ERROR "CSToolkit: cstoolkit_add_test(${TARGET_NAME}): FRAMEWORK \"${FRAMEWORK}\" is not supported")
        return()
    endif()

    if(NOT DEFINED TEST_FILES_MODE)
        set(TEST_FILES_MODE "${CSTOOLKIT_DEFAULT_FILES_MODE}")
    endif()

    # Sources paths

    # Trick for later call to cmake_path
    set(TEST_PREFIX_DIR "./")

    # Adaption for projects converted from old project file architecture
    if(CMAKE_CURRENT_SOURCE_DIR MATCHES ".*/_project$")
        set(TEST_PREFIX_DIR "./../")
    endif()

    if(NOT DEFINED TEST_SOURCES_DIRS)
        set(TEST_SOURCES_DIRS ${TEST_PREFIX_DIR}${CSTOOLKIT_DEFAULT_TESTS_SOURCES_DIRS})
    endif()
    if(NOT DEFINED TEST_DATA_DIRS)
        set(TEST_DATA_DIRS ${TEST_PREFIX_DIR}${CSTOOLKIT_DEFAULT_TESTS_DATA_DIRS})
    endif()

    # Conversion to absolute path necessary for later call to source_group
    cstoolkit_file_realpath_list(TEST_SOURCES_DIRS)
    cstoolkit_file_realpath_list(TEST_DATA_DIRS)

    if(DEFINED TEST_SOURCES_FILES)
        cstoolkit_file_realpath_list(TEST_SOURCES_FILES)
    endif()
    if(DEFINED TEST_DATA_FILES)
        cstoolkit_file_realpath_list(TEST_DATA_FILES)
    endif()

    if(TEST_FILES_MODE STREQUAL "AUTO" OR TEST_FILES_MODE STREQUAL "APPEND")
        list(TRANSFORM CSTOOLKIT_CXX_SOURCE_FILE_EXTENSIONS PREPEND "*." OUTPUT_VARIABLE CXX_SOURCES_GLOB)
        if(NOT DEFINED TEST_SOURCES_FILES OR TEST_FILES_MODE STREQUAL "APPEND")
            cstoolkit_file_glob_recurse_dirs(TEST_SOURCES_FILES "${CXX_SOURCES_GLOB}" "${TEST_SOURCES_DIRS}" ${TEST_FILES_MODE})
        endif()
        if(NOT DEFINED TEST_DATA_FILES OR TEST_FILES_MODE STREQUAL "APPEND")
            cstoolkit_file_glob_recurse_dirs(TEST_DATA_FILES "*" "${TEST_DATA_DIRS}" ${TEST_FILES_MODE})
        endif()
    endif()

    if(NOT TEST_SOURCES_FILES)
        message(NOTICE ${COLOR_YELLOW_BOLD} "CSToolkit: cstoolkit_add_test(${TARGET_NAME}): No test source file found" ${COLOR_RESET})
        return()
    endif()

    message(VERBOSE "==================================================")
    message(VERBOSE "Tests (${FRAMEWORK}) for target ${TARGET_NAME}")
    message(VERBOSE "  - Tests sources path: ${TEST_SOURCES_DIRS}")
    message(VERBOSE "  - Tests data path: ${TEST_DATA_DIRS}")
    message(VERBOSE "  - Tests sources files: ${TEST_SOURCES_FILES}")
    message(VERBOSE "  - Tests data files: ${TEST_DATA_FILES}")

    # Folders of files in visual studio project
    cstoolkit_source_group(TREE ${TEST_SOURCES_DIRS} PREFIX "Tests Files" FILES ${TEST_SOURCES_FILES})
    cstoolkit_source_group(TREE ${TEST_DATA_DIRS} PREFIX "Data Files" FILES ${TEST_DATA_FILES})

    # Folder of the project in visual studio solution
    # Recalculate because Tested Target and Test Target must be inside a folder to avoid name conflict
    set(TARGET_FOLDER "${CMAKE_CURRENT_SOURCE_DIR}")
    cmake_path(RELATIVE_PATH TARGET_FOLDER BASE_DIRECTORY ${CMAKE_SOURCE_DIR})

    # Add leading and trailing "/" for simpler regex evaluation
    string(PREPEND TARGET_FOLDER "/")
    string(APPEND TARGET_FOLDER "/")
    string(REGEX REPLACE "-[Ss][Rr][Cc]/" "/" TARGET_FOLDER "${TARGET_FOLDER}")
    # DO NOT Remove last occurrence of TARGET_NAME (case-insensitive)
    # Should be the only difference with cstoolkit_add_target()
    string(REGEX REPLACE "/[Ss][Oo][Uu][Rr][Cc][Ee][Ss]?/" "/" TARGET_FOLDER "${TARGET_FOLDER}")
    string(REGEX REPLACE "/[Ss][Rr][Cc]/" "/" TARGET_FOLDER "${TARGET_FOLDER}")
    string(REGEX REPLACE "/[Tt][Ee][Ss][Tt][Ss]?/" "/" TARGET_FOLDER "${TARGET_FOLDER}")
    string(REPLACE "/_project/" "/" TARGET_FOLDER "${TARGET_FOLDER}") # LDTools legacy
    string(REPLACE "/../" "/" TARGET_FOLDER "${TARGET_FOLDER}")
    string(REPLACE "/./" "/" TARGET_FOLDER "${TARGET_FOLDER}")
    string(REGEX REPLACE "/+" "/" TARGET_FOLDER "${TARGET_FOLDER}")

    # Remove leading and trailing "/"
    string(REGEX REPLACE "^/" "" TARGET_FOLDER "${TARGET_FOLDER}")
    string(REGEX REPLACE "/$" "" TARGET_FOLDER "${TARGET_FOLDER}")

    set_target_properties(${TARGET_NAME} PROPERTIES FOLDER "${TARGET_FOLDER}")
    if(TARGET ${TARGET_NAME}_STATIC)
        set_target_properties(${TARGET_NAME}_STATIC PROPERTIES FOLDER "${TARGET_FOLDER}")
    endif()

    # Objects of the tested target reused by every test target. Optionally drop some
    # compilation units (e.g. the one defining main() when testing an executable,
    # which would clash with the main() provided by the test framework).
    set(TEST_TARGET_OBJECTS "$<TARGET_OBJECTS:${TARGET_NAME}>")
    if(TEST_EXCLUDE_OBJECTS)
        # Object files are named after the source stem (Visual Studio: "main.obj")
        # or after the full source name (Ninja/Makefiles: "main.cpp.o[bj]"), so match both forms.
        set(_exclude_names "")
        foreach(_excluded IN LISTS TEST_EXCLUDE_OBJECTS)
            cmake_path(GET _excluded FILENAME _excluded_name) # main
            cmake_path(GET _excluded STEM LAST_ONLY _excluded_stem) # main.cpp
            foreach(_pattern IN ITEMS "${_excluded_stem}" "${_excluded_name}") # escape regex special characters
                string(REPLACE "." "\\." _pattern "${_pattern}")
                string(REPLACE "+" "\\+" _pattern "${_pattern}")
                list(APPEND _exclude_names "${_pattern}")
            endforeach()
        endforeach()
        list(REMOVE_DUPLICATES _exclude_names)
        list(JOIN _exclude_names "|" _exclude_names)
        set(TEST_TARGET_OBJECTS "$<FILTER:$<TARGET_OBJECTS:${TARGET_NAME}>,EXCLUDE,[/\\\\](${_exclude_names})\\.o(bj)?$>")
    endif()

    foreach(_test_file IN LISTS TEST_SOURCES_FILES)
        cmake_path(GET _test_file STEM LAST_ONLY _filename)
        if(NOT _filename MATCHES "tst_.*")
            message(SEND_ERROR "CSToolkit: cstoolkit_add_test(${TARGET_NAME}): Test file ${_test_file} name does not match required qt naming rules (tst_<name>).")
            continue()
        endif()

        string(REGEX REPLACE "^tst_" "" _test_target "${_filename}")
        set(_test_target ${_test_target}Test)

        add_executable(${_test_target})
        target_sources(${_test_target} PRIVATE ${_test_file})
        if(FRAMEWORK STREQUAL "QTEST")
            target_link_libraries(${_test_target} PRIVATE Qt6::Test)
        endif()

        set_target_properties(${_test_target} PROPERTIES FOLDER ${TARGET_FOLDER}/tests)

        # Postfix, necessary for executables
        if(CMAKE_CONFIGURATION_TYPES)
            foreach(_config ${CMAKE_CONFIGURATION_TYPES})
                string(TOUPPER "${_config}" _config)
                set_target_properties(${_test_target} PROPERTIES "${_config}_POSTFIX" "${CMAKE_${_config}_POSTFIX}")
            endforeach()
        else()
            string(TOUPPER "${CMAKE_BUILD_TYPE}" _config)
            set_target_properties(${_test_target} PROPERTIES "${_config}_POSTFIX" "${CMAKE_${_config}_POSTFIX}")
        endif()

        set_target_properties(${_test_target} PROPERTIES
            ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/tests/lib/$<LOWER_CASE:$<CONFIG>>"
            LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/tests/lib/$<LOWER_CASE:$<CONFIG>>"
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/tests/bin/$<LOWER_CASE:$<CONFIG>>"
        )

        # Reuse the object files already compiled for the tested target instead of recompiling all its sources
        target_sources(${_test_target} PRIVATE ${TEST_TARGET_OBJECTS})

        # Reuse the tested target's own build requirements
        target_include_directories(${_test_target} PRIVATE $<TARGET_PROPERTY:${TARGET_NAME},INCLUDE_DIRECTORIES>)
        target_compile_options(${_test_target} PRIVATE $<TARGET_PROPERTY:${TARGET_NAME},COMPILE_OPTIONS>)
        target_compile_definitions(${_test_target} PRIVATE $<TARGET_PROPERTY:${TARGET_NAME},COMPILE_DEFINITIONS>)
        target_compile_definitions(${_test_target} PRIVATE $<TARGET_PROPERTY:${TARGET_NAME},DEFINE_SYMBOL>)
        target_link_options(${_test_target} PRIVATE $<TARGET_PROPERTY:${TARGET_NAME},LINK_OPTIONS>)
        target_link_libraries(${_test_target} PRIVATE $<TARGET_PROPERTY:${TARGET_NAME},LINK_LIBRARIES>)

        # Framework specific generated files and runtime setup
        if(FRAMEWORK STREQUAL "QTEST")
            cstoolkit_qt_wrap_cpp(TEST_MOC_FILES TESTS TARGET ${_test_target} ${_test_file})

            cstoolkit_source_group(TREE ${CMAKE_CURRENT_BINARY_DIR}/tests/generated PREFIX "Generated Files/tests" FILES ${TEST_MOC_FILES})

            target_include_directories(${_test_target} PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/tests/generated/moc/$<LOWER_CASE:$<CONFIG>>)
            target_sources(${_test_target} PRIVATE ${TEST_MOC_FILES})
        endif()

        add_test(NAME ${_test_target} COMMAND ${_test_target})
        if(FRAMEWORK STREQUAL "QTEST")
            set_tests_properties(${_test_target} PROPERTIES ENVIRONMENT "PATH=${Qt6_ROOT}/bin;$ENV{PATH}")
        endif()

        add_custom_command(TARGET ${_test_target} POST_BUILD
            COMMAND ${CSTOOLKIT_COPY} -e
                "$<FILTER:$<TARGET_RUNTIME_DLLS:${_test_target}>,EXCLUDE,^${Qt6_INSTALL_PREFIX}>"
                "$<TARGET_FILE_DIR:${_test_target}>"
                COMMAND_EXPAND_LISTS
        )
    endforeach()

endfunction()
