function(cstoolkit_add_target TARGET_NAME TARGET_TYPE)
    
    # Parse arguments

    set(TARGET_OPTIONS RECURSIVE RECURSIVE_INCLUDE RECURSIVE_INTERFACE_INCLUDE NO_INSTALL PUBLIC_HEADERS_NO_EXTENSION)
    set(TARGET_UNIQUE NAMESPACE ALIAS EXTENSION PLUGINS_DIR)
    set(TARGET_MULTIPLE
        # LIBRARIES
        PUBLIC_LINK_LIBRARIES
        PRIVATE_LINK_LIBRARIES
        COMBINED_LINK_LIBRARIES
        # DIRS
        PUBLIC_HEADERS_DIRS
        PRIVATE_HEADERS_DIRS
        SOURCES_DIRS
        UI_DIRS
        RESOURCES_DIRS
        TRANSLATION_DIRS
        # FILES
        PUBLIC_HEADERS_FILES
        PRIVATE_HEADERS_FILES
        SOURCES_FILES
        UI_FILES
        RESOURCES_FILES
        TRANSLATION_FILES
        DEPLOY_FILES
        # OPTIONS
        LINK_OPTIONS
        COMPILE_DEFINITIONS
        COMPILE_OPTIONS
        #MSVC
        RC_ICONS
    )
    set(TARGET_REPETITIVE PLUGINS)
    cmake_parse_arguments(PARSE_ARGV 2 TARGET "${TARGET_OPTIONS}" "${TARGET_UNIQUE}" "${TARGET_MULTIPLE};${TARGET_REPETITIVE}")

    if(TARGET_NAME STREQUAL "")
        message(SEND_ERROR "CSToolkit: add_target(): No NAME defined for target")
        return()
    endif()

    if(DEFINED TARGET_UNPARSED_ARGUMENTS)
        message(SEND_ERROR "CSToolkit: add_target(${TARGET_NAME}): Unkown arguments \"${TARGET_UNPARSED_ARGUMENTS}\"")
        return()
    endif()

    cstoolkit_parse_repetitive_arguments(TARGET "${TARGET_REPETITIVE}" "${TARGET_OPTIONS};${TARGET_UNIQUE};${TARGET_MULTIPLE}" ${ARGN})

    if(TARGET_TYPE STREQUAL "EXECUTABLE")
        set(TARGET_EXECUTABLE TRUE)
    elseif(TARGET_TYPE STREQUAL "SHARED")
        set(TARGET_SHARED TRUE)
    elseif(TARGET_TYPE STREQUAL "MODULE")
        set(TARGET_SHARED TRUE)
        set(TARGET_MODULE TRUE)
    elseif(TARGET_TYPE STREQUAL "STATIC")
        set(TARGET_STATIC TRUE)
    elseif(TARGET_TYPE STREQUAL "INTERFACE")
        set(TARGET_INTERFACE TRUE)
    elseif(TARGET_TYPE STREQUAL "SHARED_AND_STATIC")
        set(TARGET_SHARED_AND_STATIC TRUE)
        set(TARGET_SHARED TRUE)
        set(TARGET_STATIC TRUE)
    else()
        message(SEND_ERROR "CSToolkit: add_target(): Incorrect TARGET_TYPE \"${TARGET_TYPE}\" for target \"${TARGET_NAME}\". Must be either EXECUTABLE, SHARED, STATIC or INTERFACE.")
        return()
    endif()

    if(NOT DEFINED TARGET_NAMESPACE)
        set(TARGET_NAMESPACE ${PROJECT_NAME}::)
    elseif(NOT TARGET_NAMESPACE MATCHES "^.*::$")
        set(TARGET_NAMESPACE ${TARGET_NAMESPACE}::)
    endif()

    if(TARGET_ALIAS)
        message(WARNING "Obsolete parameter ALIAS, please use NAMESPACE.")
    endif()

    # Library
    if(TARGET_SHARED_AND_STATIC)
        set(TARGET_NAME_STATIC ${TARGET_NAME}_STATIC)
        add_library(${TARGET_NAME} SHARED)
        add_library(${TARGET_NAME_STATIC} STATIC)
        add_library(${TARGET_NAMESPACE}${TARGET_NAME} ALIAS ${TARGET_NAME})
        add_library(${TARGET_NAMESPACE}${TARGET_NAME_STATIC} ALIAS ${TARGET_NAME_STATIC})
        if(TARGET_ALIAS AND NOT TARGET_ALIAS STREQUAL ${TARGET_NAMESPACE}${TARGET_NAME})
            add_library(${TARGET_ALIAS} ALIAS ${TARGET_NAME})
            add_library(${TARGET_ALIAS}_STATIC ALIAS ${TARGET_NAME_STATIC})
        endif()
    elseif(TARGET_SHARED OR TARGET_STATIC OR TARGET_INTERFACE)
        if(TARGET_STATIC)
            set(TARGET_NAME_STATIC ${TARGET_NAME})
        endif()
        add_library(${TARGET_NAME} ${TARGET_TYPE})
        add_library(${TARGET_NAMESPACE}${TARGET_NAME} ALIAS ${TARGET_NAME})
        if(TARGET_ALIAS AND NOT TARGET_ALIAS STREQUAL ${TARGET_NAMESPACE}${TARGET_NAME})
            add_library(${TARGET_ALIAS} ALIAS ${TARGET_NAME})
        endif()
    endif()

    # Executable

    if(TARGET_EXECUTABLE)
        add_executable(${TARGET_NAME})
        add_executable(${TARGET_NAMESPACE}${TARGET_NAME} ALIAS ${TARGET_NAME})
        if(TARGET_ALIAS AND NOT TARGET_ALIAS STREQUAL ${TARGET_NAMESPACE}${TARGET_NAME})
            add_executable(${TARGET_ALIAS} ALIAS ${TARGET_NAME})
        endif()
    endif()

    # Postfix, necessaire pour les executables
    if(CMAKE_CONFIGURATION_TYPES)
        foreach(_config ${CMAKE_CONFIGURATION_TYPES})
            string(TOUPPER "${_config}" _config)
            set_target_properties(${TARGET_NAME} PROPERTIES "${_config}_POSTFIX" "${CMAKE_${_config}_POSTFIX}")
            if(TARGET_SHARED_AND_STATIC)
                set_target_properties(${TARGET_NAME_STATIC} PROPERTIES "${_config}_POSTFIX" "${CMAKE_${_config}_POSTFIX}")
            endif()
        endforeach()
    else()
        string(TOUPPER "${CMAKE_BUILD_TYPE}" _config)
        set_target_properties(${TARGET_NAME} PROPERTIES "${_config}_POSTFIX" "${CMAKE_${_config}_POSTFIX}")
        if(TARGET_SHARED_AND_STATIC)
            set_target_properties(${TARGET_NAME_STATIC} PROPERTIES "${_config}_POSTFIX" "${CMAKE_${_config}_POSTFIX}")
        endif()
    endif()

    # Extension

    if(TARGET_EXTENSION)
        set_target_properties(${TARGET_NAME} PROPERTIES SUFFIX ".${TARGET_EXTENSION}")
    endif()

    if(CSTOOLKIT_PREFIX_OUTPUT_NAME)
        set(OUTPUT_NAME ${PROJECT_NAME}${TARGET_NAME})
        set_target_properties(${TARGET_NAME} PROPERTIES OUTPUT_NAME "${OUTPUT_NAME}")
        if(TARGET_SHARED_AND_STATIC)
            set_target_properties(${TARGET_NAME_STATIC} PROPERTIES OUTPUT_NAME "${OUTPUT_NAME}")
        endif()
    else()
        set(OUTPUT_NAME ${TARGET_NAME})
        set_target_properties(${TARGET_NAME} PROPERTIES OUTPUT_NAME "${OUTPUT_NAME}")
        if(TARGET_SHARED_AND_STATIC)
            set_target_properties(${TARGET_NAME_STATIC} PROPERTIES OUTPUT_NAME "${OUTPUT_NAME}")
        endif()
    endif()

    # Ouput dirs

    set_target_properties(${TARGET_NAME} PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib/$<LOWER_CASE:$<CONFIG>>"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib/$<LOWER_CASE:$<CONFIG>>"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin/$<LOWER_CASE:$<CONFIG>>"
    )
    if(TARGET_SHARED_AND_STATIC)
        set_target_properties(${TARGET_NAME_STATIC} PROPERTIES
            ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib/$<LOWER_CASE:$<CONFIG>>"
            LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib/$<LOWER_CASE:$<CONFIG>>"
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin/$<LOWER_CASE:$<CONFIG>>"
        )
    endif()

    if(TARGET_MODULE) # Module libraries are always treated as library targets by cmake
        set_target_properties(${TARGET_NAME} PROPERTIES
            LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin/$<LOWER_CASE:$<CONFIG>>"
        )
    endif()

    # Version

    if(PROJECT_VERSION AND PROJECT_VERSION_MAJOR)
        set_target_properties(${TARGET_NAME} PROPERTIES VERSION ${PROJECT_VERSION} SOVERSION ${PROJECT_VERSION_MAJOR})
        if(TARGET_SHARED_AND_STATIC)
            set_target_properties(${TARGET_NAME_STATIC} PROPERTIES VERSION ${PROJECT_VERSION} SOVERSION ${PROJECT_VERSION_MAJOR})
        endif()
    endif()

    # Folder of the project in visual studio soluction

    set(_folder "${CMAKE_CURRENT_SOURCE_DIR}/")
    cmake_path(RELATIVE_PATH _folder BASE_DIRECTORY ${CMAKE_SOURCE_DIR})
    string(REPLACE "${TARGET_NAME}/" "" _folder "${_folder}")
    string(REPLACE "Sources/" "" _folder "${_folder}")
    string(REPLACE "src/" "" _folder "${_folder}")
    string(REPLACE "test/" "" _folder "${_folder}")
    string(REPLACE "_project/" "" _folder "${_folder}")
    string(REPLACE "../" "" _folder "${_folder}")
    string(REGEX REPLACE "/$" "" _folder "${_folder}")
    string(REGEX REPLACE "^\\.\\.?$" "" _folder "${_folder}")

    set_target_properties(${TARGET_NAME} PROPERTIES FOLDER "${_folder}")
    if(TARGET_SHARED_AND_STATIC)
        set_target_properties(${TARGET_NAME_STATIC} PROPERTIES FOLDER "${_folder}")
    endif()
    

    # Sources Paths

    # Trick for later call to cmake_path
    set(TARGET_PREFIX_DIR "./")

    # Adaption for projects converted from old project file architecture
    if(CMAKE_CURRENT_SOURCE_DIR MATCHES ".*/_project$")
        set(TARGET_PREFIX_DIR "./../")
    endif()

    if(NOT DEFINED TARGET_PUBLIC_HEADERS_DIRS)
        set(TARGET_PUBLIC_HEADERS_DIRS ${TARGET_PREFIX_DIR}${CSTOOLKIT_DEFAULT_PUBLIC_HEADERS_DIRS})
    endif()
    if(NOT DEFINED TARGET_PRIVATE_HEADERS_DIRS)
        set(TARGET_PRIVATE_HEADERS_DIRS ${TARGET_PREFIX_DIR}${CSTOOLKIT_DEFAULT_PRIVATE_HEADERS_DIRS})
    endif()
    if(NOT DEFINED TARGET_SOURCES_DIRS)
        set(TARGET_SOURCES_DIRS ${TARGET_PREFIX_DIR}${CSTOOLKIT_DEFAULT_SOURCES_DIRS})
    endif()
    if(NOT DEFINED TARGET_UI_DIRS)
        set(TARGET_UI_DIRS ${TARGET_PREFIX_DIR}${CSTOOLKIT_DEFAULT_UI_DIRS})
    endif()
    if(NOT DEFINED TARGET_RESOURCES_DIRS)
        set(TARGET_RESOURCES_DIRS ${TARGET_PREFIX_DIR}${CSTOOLKIT_DEFAULT_RESOURCES_DIRS})
    endif()
    if(NOT DEFINED TARGET_TRANSLATION_DIRS)
        set(TARGET_TRANSLATION_DIRS ${TARGET_PREFIX_DIR}${CSTOOLKIT_DEFAULT_TRANSLATION_DIRS})
    endif()

    # Conversion to absolute path necessary for later call to source_group
    cstoolkit_file_realpath_list(TARGET_PUBLIC_HEADERS_DIRS)
    cstoolkit_file_realpath_list(TARGET_PRIVATE_HEADERS_DIRS)
    cstoolkit_file_realpath_list(TARGET_SOURCES_DIRS)
    cstoolkit_file_realpath_list(TARGET_UI_DIRS)
    cstoolkit_file_realpath_list(TARGET_RESOURCES_DIRS)
    cstoolkit_file_realpath_list(TARGET_TRANSLATION_DIRS)

    # Sources
    if(DEFINED TARGET_PUBLIC_HEADERS_FILES)
        cstoolkit_file_realpath_list(TARGET_PUBLIC_HEADERS_FILES)
    endif()
    if(DEFINED TARGET_PRIVATE_HEADERS_FILES)
        cstoolkit_file_realpath_list(TARGET_PRIVATE_HEADERS_FILES)
    endif()
    if(DEFINED TARGET_SOURCES_FILES)
        cstoolkit_file_realpath_list(TARGET_SOURCES_FILES)
    endif()
    if(DEFINED TARGET_UI_FILES)
        cstoolkit_file_realpath_list(TARGET_UI_FILES)
    endif()
    if(DEFINED TARGET_RESOURCES_FILES)
        cstoolkit_file_realpath_list(TARGET_RESOURCES_FILES)
    endif()
    if(DEFINED TARGET_TRANSLATION_FILES)
        cstoolkit_file_realpath_list(TARGET_TRANSLATION_FILES)
    endif()

    if(TARGET_RECURSIVE)
        if(NOT DEFINED TARGET_PUBLIC_HEADERS_FILES)
            if(TARGET_PUBLIC_HEADERS_NO_EXTENSION)
                cstoolkit_file_glob_recurse_dirs(TARGET_PUBLIC_HEADERS_FILES "*" "${TARGET_PUBLIC_HEADERS_DIRS}")
            else()
                cstoolkit_file_glob_recurse_dirs(TARGET_PUBLIC_HEADERS_FILES "*.h;*.hpp;*.hxx" "${TARGET_PUBLIC_HEADERS_DIRS}")
            endif()
        endif()
        if(NOT DEFINED TARGET_PRIVATE_HEADERS_FILES)
            cstoolkit_file_glob_recurse_dirs(TARGET_PRIVATE_HEADERS_FILES "*.h;*.hpp;*.hxx" "${TARGET_PRIVATE_HEADERS_DIRS}")
        endif()
        if(NOT DEFINED TARGET_SOURCES_FILES)
            cstoolkit_file_glob_recurse_dirs(TARGET_SOURCES_FILES "*.c;*.cpp;*.cxx" "${TARGET_SOURCES_DIRS}")
        endif()
        if(NOT DEFINED TARGET_UI_FILES)
            cstoolkit_file_glob_recurse_dirs(TARGET_UI_FILES "*.ui" "${TARGET_UI_DIRS}")
        endif()
        if(NOT DEFINED TARGET_RESOURCES_FILES)
            cstoolkit_file_glob_recurse_dirs(TARGET_RESOURCES_FILES "*.qrc" "${TARGET_RESOURCES_DIRS}")
        endif()
        if(NOT DEFINED TARGET_TRANSLATION_FILES)
            cstoolkit_file_glob_recurse_dirs(TARGET_TRANSLATION_FILES "*.ts" "${TARGET_TRANSLATION_DIRS}")
        endif()
    endif()

    # Folders of files in visual studio project

    cstoolkit_source_group(TREE ${TARGET_PRIVATE_HEADERS_DIRS} PREFIX "Header Files (Private)" FILES ${TARGET_PRIVATE_HEADERS_FILES})
    cstoolkit_source_group(TREE ${TARGET_PUBLIC_HEADERS_DIRS} PREFIX "Header Files (Public)" FILES ${TARGET_PUBLIC_HEADERS_FILES})
    cstoolkit_source_group(TREE ${TARGET_SOURCES_DIRS} PREFIX "Source Files" FILES ${TARGET_SOURCES_FILES})
    cstoolkit_source_group(TREE ${TARGET_UI_DIRS} PREFIX "Form Files" FILES ${TARGET_UI_FILES})
    cstoolkit_source_group(TREE ${TARGET_RESOURCES_DIRS} PREFIX "Resource Files" FILES ${TARGET_RESOURCES_FILES})
    cstoolkit_source_group(TREE ${TARGET_TRANSLATION_DIRS} PREFIX "Translation Files" FILES ${TARGET_TRANSLATION_FILES})

    # Sources

    target_sources(${TARGET_NAME} PRIVATE
        ${TARGET_SOURCES_FILES}
        ${TARGET_UI_FILES}
        ${TARGET_RESOURCES_FILES}
        ${TARGET_TRANSLATION_FILES}
    )
    if(TARGET_SHARED_AND_STATIC)
        target_sources(${TARGET_NAME_STATIC} PRIVATE
            ${TARGET_SOURCES_FILES}
            ${TARGET_UI_FILES}
            ${TARGET_RESOURCES_FILES}
            ${TARGET_TRANSLATION_FILES}
        )
    endif()

    if(TARGET_PUBLIC_HEADERS_FILES)
        target_sources(${TARGET_NAME}
            PUBLIC FILE_SET HEADERS BASE_DIRS ${TARGET_PUBLIC_HEADERS_DIRS} FILES ${TARGET_PUBLIC_HEADERS_FILES})
        if(TARGET_SHARED_AND_STATIC)
            target_sources(${TARGET_NAME_STATIC}
                PUBLIC FILE_SET HEADERS BASE_DIRS ${TARGET_PUBLIC_HEADERS_DIRS} FILES ${TARGET_PUBLIC_HEADERS_FILES})
        endif()
    endif()
    target_sources(${TARGET_NAME}
        PRIVATE FILE_SET "private" TYPE HEADERS BASE_DIRS ${TARGET_PRIVATE_HEADERS_DIRS} FILES ${TARGET_PRIVATE_HEADERS_FILES})
    if(TARGET_SHARED_AND_STATIC)
        target_sources(${TARGET_NAME_STATIC}
            PRIVATE FILE_SET "private" TYPE HEADERS BASE_DIRS ${TARGET_PRIVATE_HEADERS_DIRS} FILES ${TARGET_PRIVATE_HEADERS_FILES})
    endif()

    # RECURSIVE_INCLUDE options
    # this option allows to add all include subdirectories to private include directories
    # RECURSIVE_INTERFACE_INCLUDE also adds to interface include directories
    if(TARGET_INTERFACE AND TARGET_RECURSIVE_INCLUDE)
        message(SEND_ERROR "CSToolkit: add_target(${TARGET_NAME}): RECURSIVE_INCLUDE is not available for INTERFACE target. Use RECURSIVE_INTERFACE_INCLUDE instead.")
    elseif(TARGET_RECURSIVE_INCLUDE OR TARGET_RECURSIVE_INTERFACE_INCLUDE)
        foreach(_public_header_file ${TARGET_PUBLIC_HEADERS_FILES})
            get_filename_component(_public_header_dir "${_public_header_file}" PATH)
            list(APPEND TARGET_RECURSIVE_PUBLIC_HEADERS_DIRS ${_public_header_dir})
        endforeach()
        list(REMOVE_DUPLICATES TARGET_RECURSIVE_PUBLIC_HEADERS_DIRS)
        list(REMOVE_ITEM TARGET_RECURSIVE_PUBLIC_HEADERS_DIRS ${TARGET_PUBLIC_HEADERS_DIRS})

        foreach(_private_header_file ${TARGET_PRIVATE_HEADERS_FILES})
            get_filename_component(_private_header_dir "${_private_header_file}" PATH)
            list(APPEND TARGET_RECURSIVE_PRIVATE_HEADERS_DIRS ${_private_header_dir})
        endforeach()
        list(REMOVE_DUPLICATES TARGET_RECURSIVE_PRIVATE_HEADERS_DIRS)
        list(REMOVE_ITEM TARGET_RECURSIVE_PRIVATE_HEADERS_DIRS ${TARGET_PRIVATE_HEADERS_DIRS})

        if(NOT TARGET_INTERFACE)
            target_include_directories(${TARGET_NAME} PRIVATE ${TARGET_RECURSIVE_PUBLIC_HEADERS_DIRS} ${TARGET_RECURSIVE_PRIVATE_HEADERS_DIRS})
            if(TARGET_SHARED_AND_STATIC)
                target_include_directories(${TARGET_NAME_STATIC} PRIVATE ${TARGET_RECURSIVE_PUBLIC_HEADERS_DIRS} ${TARGET_RECURSIVE_PRIVATE_HEADERS_DIRS})
            endif()
        endif()

        if(TARGET_RECURSIVE_INTERFACE_INCLUDE AND TARGET_RECURSIVE_PUBLIC_HEADERS_DIRS)
            set(TARGET_BUILD_INTERFACE_RECURSIVE_PUBLIC_HEADERS_DIRS ${TARGET_RECURSIVE_PUBLIC_HEADERS_DIRS})
            list(TRANSFORM TARGET_BUILD_INTERFACE_RECURSIVE_PUBLIC_HEADERS_DIRS PREPEND "$<BUILD_INTERFACE:")
            list(TRANSFORM TARGET_BUILD_INTERFACE_RECURSIVE_PUBLIC_HEADERS_DIRS APPEND ">")
            target_include_directories(${TARGET_NAME} INTERFACE ${TARGET_BUILD_INTERFACE_RECURSIVE_PUBLIC_HEADERS_DIRS})
            if(TARGET_SHARED_AND_STATIC)
                target_include_directories(${TARGET_NAME_STATIC} INTERFACE ${TARGET_BUILD_INTERFACE_RECURSIVE_PUBLIC_HEADERS_DIRS})
            endif()
        endif()
    endif()

    # Compile Options

    target_compile_options(${TARGET_NAME} PRIVATE ${TARGET_COMPILE_OPTIONS})
    if(TARGET_SHARED_AND_STATIC)
        target_compile_options(${TARGET_NAME_STATIC} PRIVATE ${TARGET_COMPILE_OPTIONS})
    endif()

    # Compile Definitions

    target_compile_definitions(${TARGET_NAME} PRIVATE ${TARGET_COMPILE_DEFINITIONS})
    if(TARGET_SHARED_AND_STATIC)
        target_compile_definitions(${TARGET_NAME_STATIC} PRIVATE ${TARGET_COMPILE_DEFINITIONS})
    endif()

    # Default Definition for Export symbols
    string(TOUPPER ${OUTPUT_NAME} OUTPUT_NAME_UPPER)
    set_target_properties(${TARGET_NAME} PROPERTIES DEFINE_SYMBOL ${OUTPUT_NAME_UPPER}_LIB)
    if(TARGET_SHARED_AND_STATIC)
        set_target_properties(${TARGET_NAME_STATIC} PROPERTIES DEFINE_SYMBOL ${OUTPUT_NAME_UPPER}_LIB)
        target_compile_definitions(${TARGET_NAME_STATIC} PUBLIC ${OUTPUT_NAME_UPPER}_STATIC)
    endif()

    # Link Options

    if(TARGET_EXECUTABLE OR TARGET_SHARED)
	    target_link_options(${TARGET_NAME} PRIVATE ${TARGET_LINK_OPTIONS})
    elseif(TARGET_LINK_OPTIONS)
        message(SEND_ERROR "CSToolkit: add_target(${TARGET_NAME}): LINK_OPTIONS defined for target with no link step.")
    endif()

    # Links

    if(TARGET_INTERFACE)
        target_link_libraries(${TARGET_NAME}
            INTERFACE ${TARGET_PUBLIC_LINK_LIBRARIES})
        if(TARGET_PRIVATE_LINK_LIBRARIES)
            message(SEND_ERROR "CSToolkit: add_target(${TARGET_NAME}): PRIVATE_LINK_LIBRARIES is not available for INTERFACE target.")
        endif()
    else()
        target_link_libraries(${TARGET_NAME}
            PUBLIC ${TARGET_PUBLIC_LINK_LIBRARIES}
            PRIVATE ${TARGET_PRIVATE_LINK_LIBRARIES})
        if(TARGET_SHARED_AND_STATIC)
            target_link_libraries(${TARGET_NAME} PRIVATE ${TARGET_COMBINED_LINK_LIBRARIES})
            target_link_libraries(${TARGET_NAME_STATIC}
                PUBLIC ${TARGET_PUBLIC_LINK_LIBRARIES}
                PRIVATE ${TARGET_PRIVATE_LINK_LIBRARIES})
        endif()
    endif()

    # Specific STATIC
    # Combined Libraries
    if(TARGET_STATIC)
        set_target_properties(${TARGET_NAME_STATIC} PROPERTIES PREFIX "lib")

        # Borland FIX when using "-" in target name
        if(CMAKE_CXX_COMPILER_ID STREQUAL "Borland")
            set_target_properties(${TARGET_NAME_STATIC} PROPERTIES STATIC_LIBRARY_OPTIONS "/N")
        endif()

        # Mandatory to be able to include static library inside a shared library
        set_target_properties(${TARGET_NAME_STATIC} PROPERTIES POSITION_INDEPENDENT_CODE ON)

        # Default pdb output is not next to .lib
        set_target_properties(${TARGET_NAME_STATIC} PROPERTIES COMPILE_PDB_OUTPUT_DIRECTORY $<TARGET_FILE_DIR:${TARGET_NAME_STATIC}>)
        
        # Necessary to redefine default name for msvc 2015
        # and COMPILE_PDB_NAME does not support generator expression
        if(CMAKE_CONFIGURATION_TYPES)
            foreach(_config ${CMAKE_CONFIGURATION_TYPES})
                string(TOUPPER "${_config}" _config)
                set_target_properties(${TARGET_NAME_STATIC} PROPERTIES "COMPILE_PDB_NAME_${_config}" "${OUTPUT_NAME}${CMAKE_${_config}_POSTFIX}")
            endforeach()
        else()
            string(TOUPPER "${CMAKE_BUILD_TYPE}" _config)
            set_target_properties(${TARGET_NAME_STATIC} PROPERTIES "COMPILE_PDB_NAME_${_config}" "${OUTPUT_NAME}${CMAKE_${_config}_POSTFIX}")
        endif()

        if(TARGET_COMBINED_LINK_LIBRARIES)
            target_link_libraries(${TARGET_NAME_STATIC} PRIVATE ${TARGET_COMBINED_LINK_LIBRARIES})

            # If X is a static library or object library, and links Y privately:
            # "target_link_libraries(X PRIVATE Y)"
            # then $<LINK_ONLY:Y> is placed in X's INTERFACE_LINK_LIBRARIES.
            # Because we combined the libs we don't need this
            # We still use target_link_libraries for the other functionnalities it provides
            get_target_property(_interface_link_libraries ${TARGET_NAME_STATIC} INTERFACE_LINK_LIBRARIES)
            foreach(_lib ${TARGET_COMBINED_LINK_LIBRARIES})
                list(REMOVE_ITEM _interface_link_libraries "$<LINK_ONLY:${_lib}>")
            endforeach()
            set_target_properties(${TARGET_NAME_STATIC} PROPERTIES INTERFACE_LINK_LIBRARIES "${_interface_link_libraries}")

            if(WIN32)
                set(_static_options)
                foreach(_lib ${TARGET_COMBINED_LINK_LIBRARIES})
                    list(APPEND _static_options "$<TARGET_FILE:${_lib}>")
                endforeach()

                list(REVERSE _static_options)

                add_custom_command(TARGET ${TARGET_NAME_STATIC} PRE_LINK
                    COMMAND ${CMAKE_COMMAND} -E rm -f $<TARGET_FILE:${TARGET_NAME_STATIC}>
                )
                add_custom_command(TARGET ${TARGET_NAME_STATIC} POST_BUILD
                    COMMAND ${CMAKE_AR} /OUT:$<TARGET_FILE:${TARGET_NAME_STATIC}> $<TARGET_FILE:${TARGET_NAME_STATIC}> ${_static_options}
                )

                # PDBs
                if(MSVC)
                    set(TARGET_COMBINED_SYMBOLS "$<LIST:TRANSFORM,${_static_options},REPLACE,\(.*\)\\.[^.]+,\\1.pdb>")
                elseif(BORLAND)
                    set(TARGET_COMBINED_SYMBOLS "$<LIST:TRANSFORM,${_static_options},REPLACE,\(.*\)\\.[^.]+,\\1.tds>")
                endif()
            else() #LINUX
                set(_ar_script "CREATE $<TARGET_FILE:${TARGET_NAME_STATIC}>")
                foreach(_lib ${TARGET_COMBINED_LINK_LIBRARIES})
                    set(_ar_script "${_ar_script}\nADDLIB $<TARGET_FILE:${_lib}>")
                endforeach()
                set(_ar_script "${_ar_script}\nADDLIB $<TARGET_FILE:${TARGET_NAME_STATIC}>")
                set(_ar_script "${_ar_script}\nSAVE")
                set(_ar_script "${_ar_script}\nEND")

                file(GENERATE OUTPUT ${TARGET_NAME}_combine_$<LOWER_CASE:$<CONFIG>>.ar CONTENT "${_ar_script}")

                add_custom_command(TARGET ${TARGET_NAME_STATIC} POST_BUILD
                    COMMAND ${CMAKE_AR} -M < ${TARGET_NAME}_combine_$<LOWER_CASE:$<CONFIG>>.ar
                    COMMAND ${CMAKE_RANLIB} $<TARGET_FILE:${TARGET_NAME_STATIC}>  # Update index for the archive
                )
            endif()
        endif()
    elseif(TARGET_COMBINED_LINK_LIBRARIES)
        message(SEND_ERROR "CSToolkit: add_target(${TARGET_NAME}): TARGET_COMBINED_LINK_LIBRARIES defined for non-static target.")
    endif()

    # Qt
    set(_qt_modules ${TARGET_PUBLIC_LINK_LIBRARIES})
    list(APPEND _qt_modules ${TARGET_PRIVATE_LINK_LIBRARIES})
    list(FILTER _qt_modules INCLUDE REGEX "^Qt5::")
    if(_qt_modules AND TARGET_SHARED_AND_STATIC)
        message(SEND_ERROR "CSToolkit: add_target(${TARGET_NAME}): Unsuported Qt dependency with SHARED_AND_STATIC option.")
    elseif(_qt_modules AND NOT TARGET_INTERFACE )

        # MOC
        if(NOT CMAKE_AUTOMOC)
            cstoolkit_qt_wrap_cpp(MOC_FILES TARGET ${TARGET_NAME}
                ${TARGET_PRIVATE_HEADERS_FILES} ${TARGET_PUBLIC_HEADERS_FILES}
                ${TARGET_SOURCES_FILES}
            )

            target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/generated/moc)

            target_sources(${TARGET_NAME} PRIVATE ${MOC_FILES})
            cstoolkit_source_group(TREE ${CMAKE_CURRENT_BINARY_DIR}/generated PREFIX "Generated Files" FILES ${MOC_FILES})
        endif()

        # UI
        if(NOT CMAKE_AUTOUIC)
            cstoolkit_qt_wrap_ui(UIC_FILES ${TARGET_UI_FILES})

            target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/generated/uic)

            target_sources(${TARGET_NAME} PRIVATE ${UIC_FILES})
            cstoolkit_source_group(TREE ${CMAKE_CURRENT_BINARY_DIR}/generated PREFIX "Generated Files" FILES ${UIC_FILES})
        endif()

        # QRC
        if(NOT CMAKE_AUTORCC)
            cstoolkit_qt_add_resources(RCC_FILES TARGET_QRC_RESOURCES_FILES ${TARGET_RESOURCES_FILES})

            target_sources(${TARGET_NAME} PRIVATE ${RCC_FILES})
            cstoolkit_source_group(TREE ${CMAKE_CURRENT_BINARY_DIR}/generated PREFIX "Generated Files" FILES ${RCC_FILES})

            target_sources(${TARGET_NAME} PRIVATE ${TARGET_QRC_RESOURCES_FILES})
            cstoolkit_source_group(TREE ${TARGET_RESOURCES_DIRS} PREFIX "Resource Files" FILES ${TARGET_QRC_RESOURCES_FILES})
        endif()

    endif()

    # Generation des fichiers target_info
    if(TARGET_SHARED OR TARGET_MODULE OR TARGET_EXECUTABLE)
        string(TIMESTAMP _year "%Y" UTC)

        if(TARGET_EXECUTABLE)
            set(_generate_executable_info "EXECUTABLE")
        endif()
        if(_qt_modules)
            set(_generate_qt_info "QT")
        endif()

        cstoolkit_generate_target_info(
            TARGET "${TARGET_NAME}"
            VERSION "${PROJECT_VERSION}"
            PRODUCT "${PROJECT_NAME}"
            ORGANIZATION "CSGroup"
            DOMAIN "https://www.csgroup.eu"
            COPYRIGHT "Copyright \\251 ${_year} CSGroup"
            ${_generate_executable_info}
            ${_generate_qt_info}
        )

        if(MSVC)
            cstoolkit_generate_rc_file(
                TARGET "${TARGET_NAME}"
                VERSION "${PROJECT_VERSION}"
                COMPANY "CSGroup"
                COPYRIGHT "Copyright \\251 ${_year} CSGroup"
                ICONS ${TARGET_RC_ICONS}
            )
        endif()
    endif()

    # Deployement des DLLs

    if(TARGET_EXECUTABLE)
        # Copy of runtime dlls and pdbs for the target
        set_target_properties(${TARGET_NAME} PROPERTIES RUNTIME_DEPENDENCIES "")

        set(TARGET_RUNTIME_DLLS "$<TARGET_PROPERTY:${TARGET_NAME},RUNTIME_DEPENDENCIES>")
        set(TARGET_RUNTIME_DLLS "$<TARGET_GENEX_EVAL:${TARGET_NAME},${TARGET_RUNTIME_DLLS}>")
        set(TARGET_RUNTIME_DLLS "$<JOIN:${TARGET_RUNTIME_DLLS},;>") # remove empty elements
        set(TARGET_RUNTIME_DLLS "$<LIST:TRANSFORM,${TARGET_RUNTIME_DLLS},REPLACE,(.+),$<1:$><$<1:$><TARGET_EXISTS:\\0$<ANGLE-R>:$<1:$><TARGET_FILE:\\0$<ANGLE-R>$<ANGLE-R>>")
        set(TARGET_RUNTIME_DLLS "$<TARGET_GENEX_EVAL:${TARGET_NAME},${TARGET_RUNTIME_DLLS}>")
        set(TARGET_RUNTIME_DLLS "$<LIST:REMOVE_DUPLICATES,${TARGET_RUNTIME_DLLS}>")
        set(TARGET_RUNTIME_DLLS "$<JOIN:${TARGET_RUNTIME_DLLS},;>") # remove empty elements

        if(Qt5_INSTALL_PREFIX) #Filtering of Qt's dlls, necessary for development
            set(TARGET_RUNTIME_DLLS "$<FILTER:${TARGET_RUNTIME_DLLS},EXCLUDE,^${Qt5_INSTALL_PREFIX}>")
        endif()

        add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
            COMMAND ${CSTOOLKIT_COPY} -e
                "${TARGET_RUNTIME_DLLS}"
                "$<TARGET_FILE_DIR:${TARGET_NAME}>"
                COMMAND_EXPAND_LISTS
        )
        
        if(MSVC)
            set(TARGET_RUNTIME_SYMBOLS "$<LIST:TRANSFORM,${TARGET_RUNTIME_DLLS},REPLACE,\(.*\)\\.[^.]+,\\1.pdb>")
        elseif(BORLAND)
            set(TARGET_RUNTIME_SYMBOLS "$<LIST:TRANSFORM,${TARGET_RUNTIME_DLLS},REPLACE,\(.*\)\\.[^.]+,\\1.tds>")
        endif()
        if(TARGET_RUNTIME_SYMBOLS)
            add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
                COMMAND ${CSTOOLKIT_COPY}
                    "${TARGET_RUNTIME_SYMBOLS}"
                    "$<TARGET_FILE_DIR:${TARGET_NAME}>"
                    COMMAND_EXPAND_LISTS
            )
        endif()

        if(TARGET_PLUGINS)
            if(TARGET_PLUGINS_DIR)
                message(WARNING "Obsolete parameter PLUGINS_DIR, please use new form PLUGINS <plugins> DESTINATION <plugin_dir>")
                if(TARGET_PLUGINSC GREATER 1 OR "DESTINATION" IN_LIST TARGET_PLUGINS)
                    message(SEND_ERROR "CSToolkit: add_target(${TARGET_NAME}): Mixed usage of obsolete parameter PLUGINS_DIR with new PLUGINS syntax.")
                    return()
                endif()
                list(APPEND TARGET_PLUGINSV0 DESTINATION "${TARGET_PLUGINS_DIR}")
            endif()

            # We manipulate the list to always have root destination "/" in index 0
            # necessary for intersection of plugins dependencies later

            set(TARGET_PLUGINS)
            set(TARGET_PLUGINS_DESTINATIONS "/")
            set(TARGET_PLUGINS_DESTINATION0 "/")
            set(TARGET_PLUGINS_DESTINATION0_LIST "")
            set(DESTINATION_INDEX 1)

            foreach(target_plugins_args ${TARGET_PLUGINSV})
                cmake_parse_arguments(${target_plugins_args} "" "DESTINATION" "PLUGINS" ${${target_plugins_args}})
                if(DEFINED ${target_plugins_args}_UNPARSED_ARGUMENTS)
                    message(SEND_ERROR "CSToolkit: add_target(${TARGET_NAME}): Unkown arguments \"${${target_plugins_args}_UNPARSED_ARGUMENTS}\"")
                    return()
                endif()

                if(NOT ${target_plugins_args}_PLUGINS)
                    continue()
                endif()

                if("${${target_plugins_args}_DESTINATION}" STREQUAL "" OR "${${target_plugins_args}_DESTINATION}" STREQUAL ".")
                    set(${target_plugins_args}_DESTINATION "/")
                endif()
                
                list(APPEND TARGET_PLUGINS ${${target_plugins_args}_PLUGINS})
                add_dependencies(${TARGET_NAME} ${${target_plugins_args}_PLUGINS})
                
                set(current_destination_index ${DESTINATION_INDEX})

                list(FIND TARGET_PLUGINS_DESTINATIONS "${${target_plugins_args}_DESTINATION}" found_destination_index)

                if(found_destination_index EQUAL -1)
                    list(APPEND TARGET_PLUGINS_DESTINATIONS "${${target_plugins_args}_DESTINATION}")
                    set(TARGET_PLUGINS_DESTINATION${DESTINATION_INDEX} ${${target_plugins_args}_DESTINATION})
                    set(TARGET_PLUGINS_DESTINATION${DESTINATION_INDEX}_LIST)
                    math(EXPR DESTINATION_INDEX "${DESTINATION_INDEX}+1")
                else()
                    set(current_destination_index ${found_destination_index})
                endif()

                list(APPEND TARGET_PLUGINS_DESTINATION${current_destination_index}_LIST ${${target_plugins_args}_PLUGINS})
            endforeach()

            set(TARGET_PLUGINS_DESTINATION_SIZE ${DESTINATION_INDEX})

            set_target_properties(${TARGET_NAME} PROPERTIES PLUGINS "${TARGET_PLUGINS}")
            set_target_properties(${TARGET_NAME} PROPERTIES PLUGINS_DESTINATIONS "${TARGET_PLUGINS_DESTINATIONS}")
            set_target_properties(${TARGET_NAME} PROPERTIES PLUGINS_DESTINATION_SIZE "${TARGET_PLUGINS_DESTINATION_SIZE}")

            # Création de la genex de base, moins les TARGET_RUNTIME_DLLS 
            set(DESTINATION_INDEX 0)
            while(DESTINATION_INDEX LESS TARGET_PLUGINS_DESTINATION_SIZE)
                set_target_properties(${TARGET_NAME} PROPERTIES PLUGINS_DESTINATION${DESTINATION_INDEX} "${TARGET_PLUGINS_DESTINATION${DESTINATION_INDEX}}")
                set_target_properties(${TARGET_NAME} PROPERTIES PLUGINS_DESTINATION${DESTINATION_INDEX}_LIST "${TARGET_PLUGINS_DESTINATION${DESTINATION_INDEX}_LIST}")

                set(PLUGINS_TARGET_FILES${DESTINATION_INDEX} "$<TARGET_PROPERTY:${TARGET_NAME},PLUGINS_DESTINATION${DESTINATION_INDEX}_LIST>")
                set(PLUGINS_TARGET_FILES${DESTINATION_INDEX} "$<TARGET_GENEX_EVAL:${TARGET_NAME},${PLUGINS_TARGET_FILES${DESTINATION_INDEX}}>")
                set(PLUGINS_TARGET_FILES${DESTINATION_INDEX} "$<JOIN:${PLUGINS_TARGET_FILES${DESTINATION_INDEX}},;>") # remove empty elements
                set(PLUGINS_TARGET_FILES${DESTINATION_INDEX} "$<LIST:TRANSFORM,${PLUGINS_TARGET_FILES${DESTINATION_INDEX}},REPLACE,(.+),$<1:$><$<1:$><TARGET_EXISTS:\\0$<ANGLE-R>:$<1:$><TARGET_FILE:\\0$<ANGLE-R>$<ANGLE-R>>")
                set(PLUGINS_TARGET_FILES${DESTINATION_INDEX} "$<TARGET_GENEX_EVAL:${TARGET_NAME},${PLUGINS_TARGET_FILES${DESTINATION_INDEX}}>")
                set(PLUGINS_TARGET_FILES${DESTINATION_INDEX} "$<LIST:REMOVE_DUPLICATES,${PLUGINS_TARGET_FILES${DESTINATION_INDEX}}>")
                set(PLUGINS_TARGET_FILES${DESTINATION_INDEX} "$<JOIN:${PLUGINS_TARGET_FILES${DESTINATION_INDEX}},;>") # remove empty elements

                # Removes TARGET_RUNTIME_DLLS that are already deployed
                set(PLUGINS_TARGET_FILES${DESTINATION_INDEX} "$<LIST:REMOVE_ITEM,${PLUGINS_TARGET_FILES${DESTINATION_INDEX}},${TARGET_RUNTIME_DLLS}>")

                if(Qt5_INSTALL_PREFIX) #Filtering of Qt's dlls, necessary for development
                    set(PLUGINS_TARGET_FILES${DESTINATION_INDEX} "$<FILTER:${PLUGINS_TARGET_FILES${DESTINATION_INDEX}},EXCLUDE,^${Qt5_INSTALL_PREFIX}>")
                endif()

                math(EXPR DESTINATION_INDEX "${DESTINATION_INDEX}+1")
            endwhile()

            # Deplacement des dependances communes vers /
            
            set(DESTINATION_INDEX 1)
            while(DESTINATION_INDEX LESS TARGET_PLUGINS_DESTINATION_SIZE)
                # Removes PLUGINS_TARGET_FILES0 that are already deployed next to exe
                set(PLUGINS_TARGET_FILES${DESTINATION_INDEX} "$<LIST:REMOVE_ITEM,${PLUGINS_TARGET_FILES${DESTINATION_INDEX}},${PLUGINS_TARGET_FILES0}>")
                math(EXPR DESTINATION_INDEX "${DESTINATION_INDEX}+1")
            endwhile()
            
            # intersects every plugin folder and moves common dependencies next to exe
            set(MOVE_TO_ROOT_PLUGINS_TARGET_FILES)
            set(DESTINATION_INDEX_A 1)
            while(DESTINATION_INDEX_A LESS TARGET_PLUGINS_DESTINATION_SIZE)
                math(EXPR DESTINATION_INDEX_B "${DESTINATION_INDEX_A}+1")
                while(DESTINATION_INDEX_B LESS TARGET_PLUGINS_DESTINATION_SIZE)
                    cstoolkit_genex_list_intersection("${PLUGINS_TARGET_FILES${DESTINATION_INDEX_A}}" "${PLUGINS_TARGET_FILES${DESTINATION_INDEX_B}}" PLUGINS_TARGET_FILES_INTER)

                    set(MOVE_TO_ROOT_PLUGINS "${MOVE_TO_ROOT_PLUGINS};${PLUGINS_TARGET_FILES_INTER}")
                    set(PLUGINS_TARGET_FILES${DESTINATION_INDEX_A} "$<LIST:REMOVE_ITEM,${PLUGINS_TARGET_FILES${DESTINATION_INDEX_A}},${PLUGINS_TARGET_FILES_INTER}>")
                    set(PLUGINS_TARGET_FILES${DESTINATION_INDEX_B} "$<LIST:REMOVE_ITEM,${PLUGINS_TARGET_FILES${DESTINATION_INDEX_B}},${PLUGINS_TARGET_FILES_INTER}>")
        
                    math(EXPR DESTINATION_INDEX_B "${DESTINATION_INDEX_B}+1")
                endwhile()
        
                math(EXPR DESTINATION_INDEX_A "${DESTINATION_INDEX_A}+1")
            endwhile()

            set(PLUGINS_TARGET_FILES0 "${PLUGINS_TARGET_FILES0};${MOVE_TO_ROOT_PLUGINS}")
            set(PLUGINS_TARGET_FILES0 "$<JOIN:${PLUGINS_TARGET_FILES0},;>")
            
            #création des commandes de copy
            set(DESTINATION_INDEX 0)
            while(DESTINATION_INDEX LESS TARGET_PLUGINS_DESTINATION_SIZE)
                # Copy of the plugins files
                add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
                    COMMAND ${CSTOOLKIT_COPY} -e
                        "${PLUGINS_TARGET_FILES${DESTINATION_INDEX}}"
                        "$<TARGET_FILE_DIR:${TARGET_NAME}>/${TARGET_PLUGINS_DESTINATION${DESTINATION_INDEX}}"
                        COMMAND_EXPAND_LISTS
                )

                if(MSVC)
                    set(PLUGINS_TARGET_SYMBOLS${DESTINATION_INDEX} "$<LIST:TRANSFORM,${PLUGINS_TARGET_FILES${DESTINATION_INDEX}},REPLACE,\(.*\)\\.[^.]+,\\1.pdb>")
                elseif(BORLAND)
                    set(PLUGINS_TARGET_SYMBOLS${DESTINATION_INDEX} "$<LIST:TRANSFORM,${PLUGINS_TARGET_FILES${DESTINATION_INDEX}},REPLACE,\(.*\)\\.[^.]+,\\1.tds>")
                endif()
                if(TARGET_RUNTIME_SYMBOLS)
                    # Copy of the plugins pdbs
                    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
                        COMMAND ${CSTOOLKIT_COPY}
                            "${PLUGINS_TARGET_SYMBOLS${DESTINATION_INDEX}}"
                            "$<TARGET_FILE_DIR:${TARGET_NAME}>/${TARGET_PLUGINS_DESTINATION${DESTINATION_INDEX}}"
                            COMMAND_EXPAND_LISTS
                    )
                endif()

                math(EXPR DESTINATION_INDEX "${DESTINATION_INDEX}+1")
            endwhile()
        endif()
    elseif(TARGET_PLUGINS)
        message(SEND_ERROR "CSToolkit: add_target(${TARGET_NAME}): Unsuported argument PLUGINS for non-executable target.")
    endif()

    # Deploy of .deploy files

    foreach(_deploy_file ${TARGET_DEPLOY_FILES})
        cstoolkit_deploy(${_deploy_file})
    endforeach()

    # Installation

    if(NOT TARGET_NO_INSTALL)
        # cette partie complexe est necessaire pour eviter des messages d'erreur cmake sur le fait que des référence d'install ne soit pas exportés

        # Les targets qui ne sont pas du top level project sont exclu du install all et placé dans un dossier à part
        if(NOT PROJECT_IS_TOP_LEVEL OR NOT TARGET_NAMESPACE STREQUAL "${PROJECT_NAME}::")
            string(REGEX REPLACE "::$" "" TARGET_INSTALL_INTER_DIR "${TARGET_NAMESPACE}")
            set(TARGET_INSTALL_EXCLUDE_FROM_ALL "EXCLUDE_FROM_ALL")
            set(TARGET_INSTALL_LIBDIR externals/${TARGET_INSTALL_INTER_DIR}/${TARGET_NAME}/lib/$<LOWER_CASE:$<CONFIG>>)
            set(TARGET_INSTALL_BINDIR externals/${TARGET_INSTALL_INTER_DIR}/${TARGET_NAME}/bin/$<LOWER_CASE:$<CONFIG>>)
            set(TARGET_INSTALL_SYMBOLSDIR externals/${TARGET_INSTALL_INTER_DIR}/${TARGET_NAME}/symbols/$<LOWER_CASE:$<CONFIG>>)
            set(TARGET_INSTALL_INCLUDEDIR externals/${TARGET_INSTALL_INTER_DIR}/${TARGET_NAME}/include)
            set(TARGET_INSTALL_CMAKEDIR externals/${TARGET_INSTALL_INTER_DIR}/${TARGET_NAME}/cmake)
            set(TARGET_INSTALL_CONFIG_NAME ${TARGET_INSTALL_INTER_DIR}_${TARGET_NAME})
            set(TARGET_INSTALL_TARGETS_NAME ${TARGET_INSTALL_CONFIG_NAME}Targets)
            set(TARGET_INSTALL_COMPONENT ${TARGET_INSTALL_INTER_DIR}_${TARGET_NAME})
            set(TARGET_INSTALL_DESTINATION externals/${TARGET_INSTALL_INTER_DIR}/${TARGET_NAME})
        else()
            get_property(CSTOOLKIT_INSTALL_TARGETS_ALL GLOBAL PROPERTY CSTOOLKIT_INSTALL_TARGETS_ALL)

            # Detection de la methode d'install
            list(LENGTH CSTOOLKIT_INSTALL_TARGETS CSTOOLKIT_INSTALL_TARGETS_NB)

            if(CSTOOLKIT_INSTALL_TARGETS_NB EQUAL 1 AND TARGET_NAME STREQUAL CSTOOLKIT_INSTALL_TARGETS) # mode single component
                set(TARGET_INSTALL_LIBDIR lib/$<LOWER_CASE:$<CONFIG>>)
                set(TARGET_INSTALL_BINDIR bin/$<LOWER_CASE:$<CONFIG>>)
                set(TARGET_INSTALL_SYMBOLSDIR symbols/$<LOWER_CASE:$<CONFIG>>)
                set(TARGET_INSTALL_INCLUDEDIR include)
                set(TARGET_INSTALL_CMAKEDIR cmake)
                set(TARGET_INSTALL_CONFIG_NAME ${PROJECT_NAME})
                set(TARGET_INSTALL_TARGETS_NAME ${TARGET_INSTALL_CONFIG_NAME}Targets)
                set(TARGET_INSTALL_COMPONENT ${TARGET_NAME})
                set(TARGET_INSTALL_DESTINATION ".")
                set_property(GLOBAL APPEND PROPERTY CSTOOLKIT_INSTALL_TARGETS_ALL "${TARGET_NAME}")
            elseif((CSTOOLKIT_INSTALL_TARGETS_NB GREATER 1 AND TARGET_NAME IN_LIST CSTOOLKIT_INSTALL_TARGETS) # mode multi component
                    OR CSTOOLKIT_INSTALL_TARGETS_NB EQUAL 0) # all targets
                set(TARGET_INSTALL_LIBDIR ${TARGET_NAME}/lib/$<LOWER_CASE:$<CONFIG>>)
                set(TARGET_INSTALL_BINDIR ${TARGET_NAME}/bin/$<LOWER_CASE:$<CONFIG>>)
                set(TARGET_INSTALL_SYMBOLSDIR ${TARGET_NAME}/symbols/$<LOWER_CASE:$<CONFIG>>)
                set(TARGET_INSTALL_INCLUDEDIR ${TARGET_NAME}/include)
                set(TARGET_INSTALL_CMAKEDIR ${TARGET_NAME}/cmake)
                set(TARGET_INSTALL_CONFIG_NAME ${PROJECT_NAME}_${TARGET_NAME})
                set(TARGET_INSTALL_TARGETS_NAME ${TARGET_INSTALL_CONFIG_NAME}Targets)
                set(TARGET_INSTALL_COMPONENT ${TARGET_NAME})
                set(TARGET_INSTALL_DESTINATION ${TARGET_NAME})
                set_property(GLOBAL APPEND PROPERTY CSTOOLKIT_INSTALL_TARGETS_ALL "${TARGET_NAME}")
            else() #autres components à exclure
                set(TARGET_INSTALL_EXCLUDE_FROM_ALL "EXCLUDE_FROM_ALL")
                set(TARGET_INSTALL_LIBDIR internals/${TARGET_NAME}/lib/$<LOWER_CASE:$<CONFIG>>)
                set(TARGET_INSTALL_BINDIR internals/${TARGET_NAME}/bin/$<LOWER_CASE:$<CONFIG>>)
                set(TARGET_INSTALL_SYMBOLSDIR internals/${TARGET_NAME}/symbols/$<LOWER_CASE:$<CONFIG>>)
                set(TARGET_INSTALL_INCLUDEDIR internals/${TARGET_NAME}/include)
                set(TARGET_INSTALL_CMAKEDIR internals/${TARGET_NAME}/cmake)
                set(TARGET_INSTALL_CONFIG_NAME ${PROJECT_NAME}_${TARGET_NAME})
                set(TARGET_INSTALL_TARGETS_NAME ${TARGET_INSTALL_CONFIG_NAME}Targets)
                set(TARGET_INSTALL_COMPONENT ${TARGET_NAME})
                set(TARGET_INSTALL_DESTINATION internals/${TARGET_NAME})
            endif()
        endif()

        set_target_properties(${TARGET_NAME} PROPERTIES INSTALL_LIBDIR "${TARGET_INSTALL_LIBDIR}")
        set_target_properties(${TARGET_NAME} PROPERTIES INSTALL_BINDIR "${TARGET_INSTALL_BINDIR}")
        set_target_properties(${TARGET_NAME} PROPERTIES INSTALL_SYMBOLSDIR "${TARGET_INSTALL_SYMBOLSDIR}")
        set_target_properties(${TARGET_NAME} PROPERTIES INSTALL_INCLUDEDIR "${TARGET_INSTALL_INCLUDEDIR}")
        set_target_properties(${TARGET_NAME} PROPERTIES INSTALL_CMAKEDIR "${TARGET_INSTALL_CMAKEDIR}")
        set_target_properties(${TARGET_NAME} PROPERTIES INSTALL_COMPONENT "${TARGET_INSTALL_COMPONENT}")
        set_target_properties(${TARGET_NAME} PROPERTIES INSTALL_EXCLUDE_FROM_ALL "${TARGET_INSTALL_EXCLUDE_FROM_ALL}")
        if(TARGET_SHARED_AND_STATIC)
            set_target_properties(${TARGET_NAME_STATIC} PROPERTIES INSTALL_LIBDIR "${TARGET_INSTALL_LIBDIR}")
            set_target_properties(${TARGET_NAME_STATIC} PROPERTIES INSTALL_BINDIR "${TARGET_INSTALL_BINDIR}")
            set_target_properties(${TARGET_NAME_STATIC} PROPERTIES INSTALL_SYMBOLSDIR "${TARGET_INSTALL_SYMBOLSDIR}")
            set_target_properties(${TARGET_NAME_STATIC} PROPERTIES INSTALL_INCLUDEDIR "${TARGET_INSTALL_INCLUDEDIR}")
            set_target_properties(${TARGET_NAME_STATIC} PROPERTIES INSTALL_CMAKEDIR "${TARGET_INSTALL_CMAKEDIR}")
            set_target_properties(${TARGET_NAME_STATIC} PROPERTIES INSTALL_COMPONENT "${TARGET_INSTALL_COMPONENT}")
            set_target_properties(${TARGET_NAME_STATIC} PROPERTIES INSTALL_EXCLUDE_FROM_ALL "${TARGET_INSTALL_EXCLUDE_FROM_ALL}")
        endif()

        if(TARGET_RECURSIVE_INTERFACE_INCLUDE AND TARGET_RECURSIVE_PUBLIC_HEADERS_DIRS)
            foreach(_public_header_dir ${TARGET_RECURSIVE_PUBLIC_HEADERS_DIRS})
                cstoolkit_relative_path_dirs(_public_header_dir BASE_DIRECTORIES ${TARGET_PUBLIC_HEADERS_DIRS} OUTPUT_VARIABLE _relative_public_header_dir)
                set(_relative_public_header_dir ${TARGET_INSTALL_INCLUDEDIR}/${_relative_public_header_dir})
                list(APPEND TARGET_RECURSIVE_RELATIVE_PUBLIC_HEADERS_DIRS ${_relative_public_header_dir})
            endforeach()

            set(TARGET_INSTALL_INCLUDES_DESTINATION INCLUDES DESTINATION ${TARGET_RECURSIVE_RELATIVE_PUBLIC_HEADERS_DIRS})
        endif()

        if(TARGET_MODULE)
            set(TARGET_INSTALL_LIBRARY_DESTINATION ${TARGET_INSTALL_BINDIR})
        else()
            set(TARGET_INSTALL_LIBRARY_DESTINATION ${TARGET_INSTALL_LIBDIR})
        endif()

        if(TARGET_SHARED_AND_STATIC)
            set(INSTALLED_TARGETS ${TARGET_NAME} ${TARGET_NAME_STATIC})
        else()
            set(INSTALLED_TARGETS ${TARGET_NAME})
        endif()

        install(TARGETS ${INSTALLED_TARGETS} EXPORT ${TARGET_INSTALL_TARGETS_NAME}
            DESTINATION ${TARGET_INSTALL_DESTINATION}
            COMPONENT ${TARGET_INSTALL_COMPONENT}
            ${TARGET_INSTALL_EXCLUDE_FROM_ALL}
            ARCHIVE DESTINATION ${TARGET_INSTALL_LIBDIR}
            LIBRARY DESTINATION ${TARGET_INSTALL_LIBRARY_DESTINATION}
            RUNTIME DESTINATION ${TARGET_INSTALL_BINDIR}
            FILE_SET HEADERS DESTINATION ${TARGET_INSTALL_INCLUDEDIR}
            ${TARGET_INSTALL_INCLUDES_DESTINATION}
        )

        if(TARGET_EXECUTABLE)
            # Runtime dependencies
            install(FILES "${TARGET_RUNTIME_DLLS}" DESTINATION ${TARGET_INSTALL_BINDIR} COMPONENT ${TARGET_INSTALL_COMPONENT} ${TARGET_INSTALL_EXCLUDE_FROM_ALL})

            set(PLUGINS_RUNTIME_DLLS "")
            if(TARGET_PLUGINS)
                set(DESTINATION_INDEX 0)
                while(DESTINATION_INDEX LESS TARGET_PLUGINS_DESTINATION_SIZE)
                    set(PLUGINS_RUNTIME_DLLS "${PLUGINS_RUNTIME_DLLS};${PLUGINS_TARGET_FILES${DESTINATION_INDEX}}")
                    install(FILES "${PLUGINS_TARGET_FILES${DESTINATION_INDEX}}" DESTINATION ${TARGET_INSTALL_BINDIR}/${TARGET_PLUGINS_DESTINATION${DESTINATION_INDEX}} COMPONENT ${TARGET_INSTALL_COMPONENT} OPTIONAL ${TARGET_INSTALL_EXCLUDE_FROM_ALL})

                    if(PLUGINS_TARGET_SYMBOLS${DESTINATION_INDEX})
                        install(FILES "${PLUGINS_TARGET_SYMBOLS${DESTINATION_INDEX}}" DESTINATION ${TARGET_INSTALL_SYMBOLSDIR} COMPONENT ${TARGET_INSTALL_COMPONENT} OPTIONAL ${TARGET_INSTALL_EXCLUDE_FROM_ALL})
                    endif()

                    math(EXPR DESTINATION_INDEX "${DESTINATION_INDEX}+1")
                endwhile()
            endif()

            # Qt
            if(_qt_modules AND CSTOOLKIT_AUTO_DEPLOY_QT)
                set(ALL_RUNTIME_DLLS "${TARGET_RUNTIME_DLLS};${PLUGINS_RUNTIME_DLLS}")
                set(ALL_RUNTIME_DLLS "$<LIST:REMOVE_DUPLICATES,${ALL_RUNTIME_DLLS}>")
                set(ALL_RUNTIME_DLLS "$<JOIN:${ALL_RUNTIME_DLLS},;>") # remove empty elements
                cstoolkit_qt_generate_deploy_app_script(
                    TARGET ${TARGET_NAME}
                    INSTALL_DIR ${TARGET_INSTALL_BINDIR}
                    RUNTIME_DEPENDENCIES ${ALL_RUNTIME_DLLS}
                    OUTPUT_SCRIPT qt_deploy_script
                    NO_UNSUPPORTED_PLATFORM_ERROR
                )
                install(SCRIPT ${qt_deploy_script} COMPONENT ${TARGET_INSTALL_COMPONENT} ${TARGET_INSTALL_EXCLUDE_FROM_ALL})
            endif()
        endif()

        # PDBS
        if(MSVC)
            if(TARGET_SHARED)
                install(FILES $<TARGET_PDB_FILE:${TARGET_NAME}> DESTINATION ${TARGET_INSTALL_BINDIR} COMPONENT ${TARGET_INSTALL_COMPONENT} OPTIONAL ${TARGET_INSTALL_EXCLUDE_FROM_ALL})
            elseif(TARGET_EXECUTABLE)
                install(FILES $<TARGET_PDB_FILE:${TARGET_NAME}> DESTINATION ${TARGET_INSTALL_SYMBOLSDIR} COMPONENT ${TARGET_INSTALL_COMPONENT} OPTIONAL ${TARGET_INSTALL_EXCLUDE_FROM_ALL})
                install(FILES "${TARGET_RUNTIME_SYMBOLS}" DESTINATION ${TARGET_INSTALL_SYMBOLSDIR} COMPONENT ${TARGET_INSTALL_COMPONENT} OPTIONAL ${TARGET_INSTALL_EXCLUDE_FROM_ALL})
            endif()
            if(TARGET_STATIC) # Not else because of SHARED_AND_STATIC case
                install(FILES $<TARGET_FILE_DIR:${TARGET_NAME_STATIC}>/$<TARGET_FILE_PREFIX:${TARGET_NAME_STATIC}>$<TARGET_FILE_BASE_NAME:${TARGET_NAME_STATIC}>.pdb
                    DESTINATION ${TARGET_INSTALL_LIBDIR} COMPONENT ${TARGET_INSTALL_COMPONENT} OPTIONAL ${TARGET_INSTALL_EXCLUDE_FROM_ALL})
                if(TARGET_COMBINED_LINK_LIBRARIES)
                    install(FILES "${TARGET_COMBINED_SYMBOLS}" DESTINATION ${TARGET_INSTALL_LIBDIR} COMPONENT ${TARGET_INSTALL_COMPONENT} OPTIONAL ${TARGET_INSTALL_EXCLUDE_FROM_ALL})
                endif()
            endif()
        elseif(BORLAND)
            if(TARGET_SHARED)
                install(FILES $<TARGET_FILE_DIR:${TARGET_NAME}>/$<TARGET_FILE_PREFIX:${TARGET_NAME}>$<TARGET_FILE_BASE_NAME:${TARGET_NAME}>.tds DESTINATION ${TARGET_INSTALL_BINDIR} COMPONENT ${TARGET_INSTALL_COMPONENT} OPTIONAL ${TARGET_INSTALL_EXCLUDE_FROM_ALL})
            elseif(TARGET_EXECUTABLE)
                install(FILES $<TARGET_FILE_DIR:${TARGET_NAME}>/$<TARGET_FILE_PREFIX:${TARGET_NAME}>$<TARGET_FILE_BASE_NAME:${TARGET_NAME}>.tds DESTINATION ${TARGET_INSTALL_SYMBOLSDIR} COMPONENT ${TARGET_INSTALL_COMPONENT} OPTIONAL ${TARGET_INSTALL_EXCLUDE_FROM_ALL})
                install(FILES "${TARGET_RUNTIME_SYMBOLS}" DESTINATION ${TARGET_INSTALL_SYMBOLSDIR} COMPONENT ${TARGET_INSTALL_COMPONENT} OPTIONAL ${TARGET_INSTALL_EXCLUDE_FROM_ALL})
            endif()
            if(TARGET_STATIC) # Not else because of SHARED_AND_STATIC case
                install(FILES $<TARGET_FILE_DIR:${TARGET_NAME_STATIC}>/$<TARGET_FILE_PREFIX:${TARGET_NAME_STATIC}>$<TARGET_FILE_BASE_NAME:${TARGET_NAME_STATIC}>.tds
                    DESTINATION ${TARGET_INSTALL_LIBDIR} COMPONENT ${TARGET_INSTALL_COMPONENT} OPTIONAL ${TARGET_INSTALL_EXCLUDE_FROM_ALL})
                if(TARGET_COMBINED_LINK_LIBRARIES)
                    install(FILES "${TARGET_COMBINED_SYMBOLS}" DESTINATION ${TARGET_INSTALL_LIBDIR} COMPONENT ${TARGET_INSTALL_COMPONENT} OPTIONAL ${TARGET_INSTALL_EXCLUDE_FROM_ALL})
                endif()
            endif()
        endif()

        # Config
        install(EXPORT ${TARGET_INSTALL_TARGETS_NAME}
            NAMESPACE ${TARGET_NAMESPACE}
            DESTINATION ${TARGET_INSTALL_CMAKEDIR}
            COMPONENT ${TARGET_INSTALL_COMPONENT}
            ${TARGET_INSTALL_EXCLUDE_FROM_ALL}
        )
        generate_target_config()
    endif()

    # Debug Messages
    message(VERBOSE "==================================================")
    message(VERBOSE "Component ${TARGET_NAME} is ${TARGET_TYPE}")
    message(VERBOSE "  - Dependencies: ${ALL_DEPS}")
    message(VERBOSE "  - Public headers path: ${TARGET_PUBLIC_HEADERS_DIRS}")
    message(VERBOSE "  - Private headers path: ${TARGET_PRIVATE_HEADERS_DIR}")
    message(VERBOSE "  - Sources path: ${TARGET_SOURCES_DIR}")
    message(VERBOSE "  - Recursive: ${TARGET_RECURSIVE}")
    message(VERBOSE  "  - Public headers files: ${TARGET_PUBLIC_HEADERS_FILES}")
    message(VERBOSE  "  - Private headers files: ${TARGET_PRIVATE_HEADERS_FILES}")
    message(VERBOSE  "  - Sources files: ${TARGET_SOURCES_FILES}")
    message(VERBOSE  "  - Public link libraries: ${TARGET_PUBLIC_LINK_LIBRARIES}")
    message(VERBOSE  "  - Private link libraries: ${TARGET_PRIVATE_LINK_LIBRARIES}")
    message(VERBOSE  "  - Runtime dependencies: ${TARGET_RUNTIME_DEPS}")
    if(NOT TARGET_NO_INSTALL)
        message(VERBOSE "  - Installation: Default")
    endif()
endfunction()

macro(generate_target_config)
    # Target Config File
    set(FIND_DEPENDENCY_GENEX "$<TARGET_PROPERTY:INTERFACE_LINK_LIBRARIES>")
    if(TARGET_STATIC)
        set(FIND_DEPENDENCY_GENEX "${FIND_DEPENDENCY_GENEX};$<TARGET_PROPERTY:LINK_LIBRARIES>")
    endif()
    set(FIND_DEPENDENCY_GENEX "$<TARGET_GENEX_EVAL:${TARGET_NAME},${FIND_DEPENDENCY_GENEX}>")
    set(FIND_DEPENDENCY_GENEX "$<LIST:REMOVE_DUPLICATES,${FIND_DEPENDENCY_GENEX}>")
    set(FIND_DEPENDENCY_GENEX "$<JOIN:${FIND_DEPENDENCY_GENEX},;>") # remove empty elements
    set(FIND_DEPENDENCY_GENEX "$<LIST:FILTER,${FIND_DEPENDENCY_GENEX},INCLUDE,.+::.+>")
    set(FIND_DEPENDENCY_GENEX "$<LIST:TRANSFORM,${FIND_DEPENDENCY_GENEX},REPLACE,(.+)::(.+),find_dependency(\\1 COMPONENTS \\2)>")

    file(READ "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/../templates/TargetConfig.cmake.in" _TARGET_CONFIG_CONTENT)
    string(CONFIGURE "${_TARGET_CONFIG_CONTENT}" _CONTENT @ONLY)
    file(GENERATE
        OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${TARGET_INSTALL_CONFIG_NAME}Config.cmake"
        CONTENT "${_CONTENT}"
        TARGET ${TARGET_NAME}
    )

    install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_INSTALL_CONFIG_NAME}Config.cmake
        DESTINATION ${TARGET_INSTALL_CMAKEDIR}
        COMPONENT ${TARGET_INSTALL_COMPONENT}
        ${TARGET_INSTALL_EXCLUDE_FROM_ALL}
    )
endmacro()
