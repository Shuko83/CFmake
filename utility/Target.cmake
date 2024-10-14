function(cstoolkit_add_target TARGET_NAME TARGET_TYPE)

    # Parse arguments

    set(TARGET_OPTIONS RECURSIVE RECURSIVE_INCLUDE RECURSIVE_INTERFACE_INCLUDE NO_INSTALL PUBLIC_HEADERS_NO_EXTENSION)
    set(TARGET_UNIQUE NAMESPACE ALIAS EXTENSION PLUGINS_DIR)
    set(TARGET_MULTIPLE
        # LIBRARIES
        PUBLIC_LINK_LIBRARIES
        PRIVATE_LINK_LIBRARIES
        COMBINED_LINK_LIBRARIES
        PLUGINS
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
    cmake_parse_arguments(TARGET "${TARGET_OPTIONS}" "${TARGET_UNIQUE}" "${TARGET_MULTIPLE}" ${ARGN})

    if(TARGET_NAME STREQUAL "")
        message(SEND_ERROR "CSToolkit: add_target(): No NAME defined for target")
        return()
    endif()

    if(DEFINED TARGET_UNPARSED_ARGUMENTS)
        message(SEND_ERROR "CSToolkit: add_target(${TARGET_NAME}): Unkown arguments \"${TARGET_UNPARSED_ARGUMENTS}\"")
    endif()

    if(TARGET_TYPE STREQUAL "EXECUTABLE")
        set(TARGET_EXECUTABLE TRUE)
    elseif(TARGET_TYPE STREQUAL "SHARED")
        set(TARGET_SHARED TRUE)
    elseif(TARGET_TYPE STREQUAL "STATIC")
        set(TARGET_STATIC TRUE)
    elseif(TARGET_TYPE STREQUAL "INTERFACE")
        set(TARGET_INTERFACE TRUE)
    else()
        message(SEND_ERROR "CSToolkit: add_target(): Incorrect TARGET_TYPE \"${TARGET_TYPE}\" for target \"${TARGET_NAME}\". Must be either EXECUTABLE, SHARED, STATIC or INTERFACE.")
        return()
    endif()

    if(NOT DEFINED TARGET_NAMESPACE)
        set(TARGET_NAMESPACE ${PROJECT_NAME}::)
    elseif(NOT TARGET_NAMESPACE MATCHES "^.*::$")
        set(TARGET_NAMESPACE ${TARGET_NAMESPACE}::)
    endif()

    # Library

    if(TARGET_SHARED OR TARGET_STATIC OR TARGET_INTERFACE)
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
        endforeach()
    else()
        string(TOUPPER "${CMAKE_BUILD_TYPE}" _config)
        set_target_properties(${TARGET_NAME} PROPERTIES "${_config}_POSTFIX" "${CMAKE_${_config}_POSTFIX}")
    endif()

    # Extension

    if(TARGET_EXTENSION)
        set_target_properties(${TARGET_NAME} PROPERTIES SUFFIX ".${TARGET_EXTENSION}")
    endif()

    if(CSTOOLKIT_PREFIX_OUTPUT_NAME)
        set(OUTPUT_NAME ${PROJECT_NAME}${TARGET_NAME})
        set_target_properties(${TARGET_NAME} PROPERTIES OUTPUT_NAME "${OUTPUT_NAME}")
    else()
        set(OUTPUT_NAME ${TARGET_NAME})
    endif()

    # Ouput dirs

    set_target_properties(${TARGET_NAME} PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib/$<LOWER_CASE:$<CONFIG>>"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib/$<LOWER_CASE:$<CONFIG>>"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin/$<LOWER_CASE:$<CONFIG>>"
    )

    # Version

    if(PROJECT_VERSION AND PROJECT_VERSION_MAJOR)
        set_target_properties(${TARGET_NAME} PROPERTIES VERSION ${PROJECT_VERSION} SOVERSION ${PROJECT_VERSION_MAJOR})
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

    # Sources Paths

    # Trick for later call to REAL_PATH
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
    file(REAL_PATH ${TARGET_PUBLIC_HEADERS_DIRS} TARGET_PUBLIC_HEADERS_DIRS EXPAND_TILDE)
    file(REAL_PATH ${TARGET_PRIVATE_HEADERS_DIRS} TARGET_PRIVATE_HEADERS_DIRS EXPAND_TILDE)
    file(REAL_PATH ${TARGET_SOURCES_DIRS} TARGET_SOURCES_DIRS EXPAND_TILDE)
    file(REAL_PATH ${TARGET_UI_DIRS} TARGET_UI_DIRS EXPAND_TILDE)
    file(REAL_PATH ${TARGET_RESOURCES_DIRS} TARGET_RESOURCES_DIRS EXPAND_TILDE)
    file(REAL_PATH ${TARGET_TRANSLATION_DIRS} TARGET_TRANSLATION_DIRS EXPAND_TILDE)

    # Sources
    if(DEFINED TARGET_PUBLIC_HEADERS_FILES)
        set(_public_headers_files ${TARGET_PUBLIC_HEADERS_FILES})
        set(TARGET_PUBLIC_HEADERS_FILES "")
        foreach(_public_header_file ${_public_headers_files})
            file(REAL_PATH ${_public_header_file} _public_header_file EXPAND_TILDE)
            list(APPEND TARGET_PUBLIC_HEADERS_FILES ${_public_header_file})
        endforeach()
    endif()
    if(DEFINED TARGET_PRIVATE_HEADERS_FILES)
        set(_private_headers_files ${TARGET_PRIVATE_HEADERS_FILES})
        set(TARGET_PRIVATE_HEADERS_FILES "")
        foreach(_private_header_file ${_private_headers_files})
            file(REAL_PATH ${_private_header_file} _private_header_file EXPAND_TILDE)
            list(APPEND TARGET_PRIVATE_HEADERS_FILES ${_private_header_file})
        endforeach()
    endif()
    if(DEFINED TARGET_SOURCES_FILES)
        set(_sources_files ${TARGET_SOURCES_FILES})
        set(TARGET_SOURCES_FILES "")
        foreach(_source_file ${_sources_files})
            file(REAL_PATH ${_source_file} _source_file EXPAND_TILDE)
            list(APPEND TARGET_SOURCES_FILES ${_source_file})
        endforeach()
    endif()
    if(DEFINED TARGET_UI_FILES)
        set(_ui_files ${TARGET_UI_FILES})
        set(TARGET_UI_FILES "")
        foreach(_ui_file ${_ui_files})
            file(REAL_PATH ${_ui_file} _ui_file EXPAND_TILDE)
            list(APPEND TARGET_UI_FILES ${_ui_file})
        endforeach()
    endif()
    if(DEFINED TARGET_RESOURCES_FILES)
        set(_resources_files ${TARGET_RESOURCES_FILES})
        set(TARGET_RESOURCES_FILES "")
        foreach(_resource_file ${_resources_files})
            file(REAL_PATH ${_resource_file} _resource_file EXPAND_TILDE)
            list(APPEND TARGET_RESOURCES_FILES ${_resource_file})
        endforeach()
    endif()
    if(DEFINED TARGET_TRANSLATION_FILES)
        set(_translation_files ${TARGET_TRANSLATION_FILES})
        set(TARGET_TRANSLATION_FILES "")
        foreach(_translation_file ${_translation_files})
            file(REAL_PATH ${_translation_file} _translation_file EXPAND_TILDE)
            list(APPEND TARGET_TRANSLATION_FILES ${_translation_file})
        endforeach()
    endif()

    if(TARGET_RECURSIVE)
        if(NOT DEFINED TARGET_PUBLIC_HEADERS_FILES)
            if(TARGET_PUBLIC_HEADERS_NO_EXTENSION)
                file(GLOB_RECURSE TARGET_PUBLIC_HEADERS_FILES ${TARGET_PUBLIC_HEADERS_DIRS}/*)
            else()
                file(GLOB_RECURSE TARGET_PUBLIC_HEADERS_FILES ${TARGET_PUBLIC_HEADERS_DIRS}/*.h ${TARGET_PUBLIC_HEADERS_DIRS}/*.hpp ${TARGET_PUBLIC_HEADERS_DIRS}/*.hxx)
            endif()
        endif()
        if(NOT DEFINED TARGET_PRIVATE_HEADERS_FILES)
            file(GLOB_RECURSE TARGET_PRIVATE_HEADERS_FILES ${TARGET_PRIVATE_HEADERS_DIRS}/*.h ${TARGET_PRIVATE_HEADERS_DIRS}/*.hpp ${TARGET_PRIVATE_HEADERS_DIRS}/*.hxx)
        endif()
        if(NOT DEFINED TARGET_SOURCES_FILES)
            file(GLOB_RECURSE TARGET_SOURCES_FILES ${TARGET_SOURCES_DIRS}/*.c ${TARGET_SOURCES_DIRS}/*.cpp ${TARGET_SOURCES_DIRS}/*.cxx)
        endif()
        if(NOT DEFINED TARGET_UI_FILES)
            file(GLOB_RECURSE TARGET_UI_FILES ${TARGET_UI_DIRS}/*.ui)
        endif()
        if(NOT DEFINED TARGET_RESOURCES_FILES)
            file(GLOB_RECURSE TARGET_RESOURCES_FILES ${TARGET_RESOURCES_DIRS}/*.qrc)
        endif()
        if(NOT DEFINED TARGET_TRANSLATION_FILES)
            file(GLOB_RECURSE TARGET_TRANSLATION_FILES ${TARGET_TRANSLATION_DIRS}/*.ts)
        endif()
    endif()

    # Folders of files in visual studio project

    source_group(TREE ${TARGET_PRIVATE_HEADERS_DIRS} PREFIX "Header Files (Private)" FILES ${TARGET_PRIVATE_HEADERS_FILES})
    source_group(TREE ${TARGET_PUBLIC_HEADERS_DIRS} PREFIX "Header Files (Public)" FILES ${TARGET_PUBLIC_HEADERS_FILES})
    source_group(TREE ${TARGET_SOURCES_DIRS} PREFIX "Source Files" FILES ${TARGET_SOURCES_FILES})
    source_group(TREE ${TARGET_UI_DIRS} PREFIX "Form Files" FILES ${TARGET_UI_FILES})
    source_group(TREE ${TARGET_RESOURCES_DIRS} PREFIX "Resource Files" FILES ${TARGET_RESOURCES_FILES})
    source_group(TREE ${TARGET_TRANSLATION_DIRS} PREFIX "Translation Files" FILES ${TARGET_TRANSLATION_FILES})

    # Sources

    target_sources(${TARGET_NAME} PRIVATE
        ${TARGET_SOURCES_FILES}
        ${TARGET_UI_FILES}
        ${TARGET_RESOURCES_FILES}
        ${TARGET_TRANSLATION_FILES}
    )

    target_sources(${TARGET_NAME}
        PUBLIC FILE_SET HEADERS BASE_DIRS ${TARGET_PUBLIC_HEADERS_DIRS} FILES ${TARGET_PUBLIC_HEADERS_FILES}
        PRIVATE FILE_SET "private" TYPE HEADERS BASE_DIRS ${TARGET_PRIVATE_HEADERS_DIRS} FILES ${TARGET_PRIVATE_HEADERS_FILES}
    )

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
        endif()

        if(TARGET_RECURSIVE_INTERFACE_INCLUDE AND TARGET_RECURSIVE_PUBLIC_HEADERS_DIRS)
            set(TARGET_BUILD_INTERFACE_RECURSIVE_PUBLIC_HEADERS_DIRS ${TARGET_RECURSIVE_PUBLIC_HEADERS_DIRS})
            list(TRANSFORM TARGET_BUILD_INTERFACE_RECURSIVE_PUBLIC_HEADERS_DIRS PREPEND "$<BUILD_INTERFACE:")
            list(TRANSFORM TARGET_BUILD_INTERFACE_RECURSIVE_PUBLIC_HEADERS_DIRS APPEND ">")
            target_include_directories(${TARGET_NAME} INTERFACE ${TARGET_BUILD_INTERFACE_RECURSIVE_PUBLIC_HEADERS_DIRS})
        endif()
    endif()

    # Compile Options

    target_compile_options(${TARGET_NAME} PRIVATE ${TARGET_COMPILE_OPTIONS})

    # Compile Definitions

    target_compile_definitions(${TARGET_NAME} PRIVATE ${TARGET_COMPILE_DEFINITIONS})

    # Default Definition for Export symbols
    string(TOUPPER ${OUTPUT_NAME} OUTPUT_NAME_UPPER)
    set_target_properties(${TARGET_NAME} PROPERTIES DEFINE_SYMBOL ${OUTPUT_NAME_UPPER}_LIB)

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
    endif()

    # Specific STATIC
    # Combined Libraries
    if(TARGET_STATIC)
        # Mandatory to be able to include static library inside a shared library
        set_target_properties(${TARGET_NAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)

        # Default pdb output is not next to .lib
        set_target_properties(${TARGET_NAME} PROPERTIES COMPILE_PDB_OUTPUT_DIRECTORY $<TARGET_FILE_DIR:${TARGET_NAME}>)
        # Necessary to redefine name for msvc 2015
        # COMPILE_PDB_NAME does not support generator expression
        set_target_properties(${TARGET_NAME} PROPERTIES COMPILE_PDB_NAME_DEBUG ${OUTPUT_NAME}${CMAKE_DEBUG_POSTFIX})

        if(TARGET_COMBINED_LINK_LIBRARIES)
            target_link_libraries(${TARGET_NAME} PRIVATE ${TARGET_COMBINED_LINK_LIBRARIES})

            # If X is a static library or object library, and links Y privately:
            # "target_link_libraries(X PRIVATE Y)"
            # then $<LINK_ONLY:Y> is placed in X's INTERFACE_LINK_LIBRARIES.
            # Because we combined the libs we don't need this
            # We still use target_link_libraries for the other functionnalities it provides
            get_target_property(_interface_link_libraries ${TARGET_NAME} INTERFACE_LINK_LIBRARIES)
            foreach(_lib ${TARGET_COMBINED_LINK_LIBRARIES})
                list(REMOVE_ITEM _interface_link_libraries "$<LINK_ONLY:${_lib}>")
            endforeach()
            set_target_properties(${TARGET_NAME} PROPERTIES INTERFACE_LINK_LIBRARIES "${_interface_link_libraries}")

            if(WIN32)
                set(_static_options)
                foreach(_lib ${TARGET_COMBINED_LINK_LIBRARIES})
                    list(APPEND _static_options "$<TARGET_FILE:${_lib}>")
                endforeach()

                set_target_properties(${TARGET_NAME} PROPERTIES STATIC_LIBRARY_OPTIONS "${_static_options}")

                # PDBs
                if(MSVC)
                    set(TARGET_COMBINED_PDBS "$<LIST:TRANSFORM,${_static_options},REPLACE,\(.*\)\\.[^.]+,\\1.pdb>")
                endif()
            else() #LINUX
                set(_ar_script "CREATE $<TARGET_FILE:${TARGET_NAME}>")
                foreach(_lib ${TARGET_COMBINED_LINK_LIBRARIES})
                    set(_ar_script "${_ar_script}\nADDLIB $<TARGET_FILE:${_lib}>")
                endforeach()
                set(_ar_script "${_ar_script}\nSAVE")
                set(_ar_script "${_ar_script}\nEND")

                file(GENERATE OUTPUT ${TARGET_NAME}_combine.ar CONTENT "${_ar_script}")

                add_custom_command(TARGET ${TARGET_NAME} PRE_LINK
                    COMMAND ${CMAKE_AR} -M < ${TARGET_NAME}_combine.ar
                )
            endif()
        endif()
    elseif(TARGET_COMBINED_LINK_LIBRARIES)
        message(SEND_ERROR "CSToolkit: add_target(${TARGET_NAME}): TARGET_COMBINED_LINK_LIBRARIES defined for non-static target")
    endif()

    # Qt
    set(_qt_modules ${TARGET_PUBLIC_LINK_LIBRARIES})
    list(APPEND _qt_modules ${TARGET_PRIVATE_LINK_LIBRARIES})
    list(FILTER _qt_modules INCLUDE REGEX "^Qt5::")
    if(_qt_modules AND NOT TARGET_INTERFACE )

        # MOC
        if(NOT CMAKE_AUTOMOC)
            cstoolkit_qt_wrap_cpp(MOC_FILES TARGET ${TARGET_NAME}
                ${TARGET_PRIVATE_HEADERS_FILES} ${TARGET_PUBLIC_HEADERS_FILES}
                ${TARGET_SOURCES_FILES}
            )

            target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/generated/moc)

            target_sources(${TARGET_NAME} PRIVATE ${MOC_FILES})
            source_group(TREE ${CMAKE_CURRENT_BINARY_DIR}/generated PREFIX "Generated Files" FILES ${MOC_FILES})
        endif()

        # UI
        if(NOT CMAKE_AUTOUIC)
            cstoolkit_qt_wrap_ui(UIC_FILES ${TARGET_UI_FILES})

            target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/generated/uic)

            target_sources(${TARGET_NAME} PRIVATE ${UIC_FILES})
            source_group(TREE ${CMAKE_CURRENT_BINARY_DIR}/generated PREFIX "Generated Files" FILES ${UIC_FILES})
        endif()

        # QRC
        if(NOT CMAKE_AUTORCC)
            cstoolkit_qt_add_resources(RCC_FILES TARGET_QRC_RESOURCES_FILES ${TARGET_RESOURCES_FILES})

            target_sources(${TARGET_NAME} PRIVATE ${RCC_FILES})
            source_group(TREE ${CMAKE_CURRENT_BINARY_DIR}/generated PREFIX "Generated Files" FILES ${RCC_FILES})

            target_sources(${TARGET_NAME} PRIVATE ${TARGET_QRC_RESOURCES_FILES})
            source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "Resource Files" FILES ${TARGET_QRC_RESOURCES_FILES})
        endif()

    endif()

    # Generation des fichiers target_info
    if(NOT TARGET_INTERFACE AND NOT TARGET_STATIC)
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
        if(Qt5_INSTALL_PREFIX) #Filtering of Qt's dlls necessary for development
            set(TARGET_RUNTIME_DLLS "$<FILTER:$<TARGET_RUNTIME_DLLS:${TARGET_NAME}>,EXCLUDE,^${Qt5_INSTALL_PREFIX}>")
        else()
            set(TARGET_RUNTIME_DLLS "$<TARGET_RUNTIME_DLLS:${TARGET_NAME}>")
        endif()

        add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
            COMMAND ${CSTOOLKIT_COPY} -e
                "${TARGET_RUNTIME_DLLS}"
                "$<TARGET_FILE_DIR:${TARGET_NAME}>"
                COMMAND_EXPAND_LISTS
        )

        if(MSVC)
            set(TARGET_RUNTIME_PDBS "$<LIST:TRANSFORM,${TARGET_RUNTIME_DLLS},REPLACE,\(.*\)\\.[^.]+,\\1.pdb>")
            add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
                COMMAND ${CSTOOLKIT_COPY}
                    "${TARGET_RUNTIME_PDBS}"
                    "$<TARGET_FILE_DIR:${TARGET_NAME}>"
                    COMMAND_EXPAND_LISTS
            )
        endif()

        if(TARGET_PLUGINS)
            # Adding plusgins as dependencies
            add_dependencies(${TARGET_NAME} ${TARGET_PLUGINS})
            set_target_properties(${TARGET_NAME} PROPERTIES PLUGINS "${TARGET_PLUGINS}")

            if(WIN32)
                set(PLUGINS_TARGET ${TARGET_NAME}_plugins)

                add_executable(${PLUGINS_TARGET} EXCLUDE_FROM_ALL CMakeLists.txt)
                set_target_properties(${PLUGINS_TARGET} PROPERTIES EXCLUDE_FROM_DEFAULT_BUILD True)
                set_target_properties(${PLUGINS_TARGET} PROPERTIES LINKER_LANGUAGE CXX)
                target_link_libraries(${PLUGINS_TARGET} PUBLIC ${TARGET_PLUGINS})

                if(Qt5_INSTALL_PREFIX) #Filtering of Qt's dlls necessary for development
                    set(PLUGINS_RUNTIME_DLLS "$<FILTER:$<TARGET_RUNTIME_DLLS:${PLUGINS_TARGET}>,EXCLUDE,^${Qt5_INSTALL_PREFIX}>")
                else()
                    set(PLUGINS_RUNTIME_DLLS "$<TARGET_RUNTIME_DLLS:${PLUGINS_TARGET}>")
                endif()

                # Copy of the plugin dll and its dependencies
                add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
                    COMMAND ${CSTOOLKIT_COPY} -e
                        "${PLUGINS_RUNTIME_DLLS}"
                        "$<TARGET_FILE_DIR:${TARGET_NAME}>/${TARGET_PLUGINS_DIR}"
                        COMMAND_EXPAND_LISTS
                )

                if(MSVC)
                    set(PLUGINS_RUNTIME_PDBS "$<LIST:TRANSFORM,${PLUGINS_RUNTIME_DLLS},REPLACE,\(.*\)\\.[^.]+,\\1.pdb>")
                    # Copy of the plugin dll and its dependencies
                    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
                        COMMAND ${CSTOOLKIT_COPY}
                            "${PLUGINS_RUNTIME_PDBS}"
                            "$<TARGET_FILE_DIR:${TARGET_NAME}>/${TARGET_PLUGINS_DIR}"
                            COMMAND_EXPAND_LISTS
                    )
                endif()
            else()
                set(PLUGINS_TARGET_FILES ${TARGET_PLUGINS})
                list(TRANSFORM PLUGINS_TARGET_FILES PREPEND "$<TARGET_FILE:")
                list(TRANSFORM PLUGINS_TARGET_FILES APPEND ">")

                # Copy of the plugin dll
                add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
                    COMMAND ${CSTOOLKIT_COPY} -e
                        "${PLUGINS_TARGET_FILES}"
                        "$<TARGET_FILE_DIR:${TARGET_NAME}>/${TARGET_PLUGINS_DIR}"
                        COMMAND_EXPAND_LISTS
                )
            endif()
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
            set(TARGET_INSTALL_LIB_DIR externals/${TARGET_INSTALL_INTER_DIR}/${TARGET_NAME}/lib/$<LOWER_CASE:$<CONFIG>>)
            set(TARGET_INSTALL_BIN_DIR externals/${TARGET_INSTALL_INTER_DIR}/${TARGET_NAME}/bin/$<LOWER_CASE:$<CONFIG>>)
            set(TARGET_INSTALL_SYMBOLS_DIR externals/${TARGET_INSTALL_INTER_DIR}/${TARGET_NAME}/symbols/$<LOWER_CASE:$<CONFIG>>)
            set(TARGET_INSTALL_INCLUDE_DIR externals/${TARGET_INSTALL_INTER_DIR}/${TARGET_NAME}/include)
            set(TARGET_INSTALL_CMAKE_DIR externals/${TARGET_INSTALL_INTER_DIR}/${TARGET_NAME}/cmake)
            set(TARGET_INSTALL_CONFIG_NAME ${TARGET_INSTALL_INTER_DIR}_${TARGET_NAME})
            set(TARGET_INSTALL_COMPONENT ${TARGET_INSTALL_INTER_DIR}_${TARGET_NAME})
            set(TARGET_INSTALL_DESTINATION externals/${TARGET_INSTALL_INTER_DIR}/${TARGET_NAME})
        else()
            if(NOT CSTOOLKIT_INSTALL_TARGETS)
                get_property(CSTOOLKIT_INSTALL_TARGETS GLOBAL PROPERTY CSTOOLKIT_INSTALL_TARGETS_AUTO)
            endif()
            get_property(CSTOOLKIT_INSTALL_TARGETS_ALL GLOBAL PROPERTY CSTOOLKIT_INSTALL_TARGETS_ALL)

            if(NOT CSTOOLKIT_INSTALL_TARGETS AND NOT CSTOOLKIT_INSTALL_TARGETS_ALL AND TARGET_NAME STREQUAL PROJECT_NAME)
                # Si aucune target précisé et que la target a le meme nom que le projet, mode single component
                set_property(GLOBAL PROPERTY CSTOOLKIT_INSTALL_TARGETS_AUTO "${TARGET_NAME}")
                set(CSTOOLKIT_INSTALL_TARGETS "${TARGET_NAME}")
                message(STATUS "CSToolkit: Automatic install mode detection: Single Component")
            endif()

            # Detection de la methode d'install
            list(LENGTH CSTOOLKIT_INSTALL_TARGETS CSTOOLKIT_INSTALL_TARGETS_NB)

            if(CSTOOLKIT_INSTALL_TARGETS_NB EQUAL 1 AND TARGET_NAME STREQUAL CSTOOLKIT_INSTALL_TARGETS) # mode single component
                set(TARGET_INSTALL_LIB_DIR lib/$<LOWER_CASE:$<CONFIG>>)
                set(TARGET_INSTALL_BIN_DIR bin/$<LOWER_CASE:$<CONFIG>>)
                set(TARGET_INSTALL_SYMBOLS_DIR symbols/$<LOWER_CASE:$<CONFIG>>)
                set(TARGET_INSTALL_INCLUDE_DIR include)
                set(TARGET_INSTALL_CMAKE_DIR cmake)
                set(TARGET_INSTALL_CONFIG_NAME ${TARGET_NAME})
                set(TARGET_INSTALL_COMPONENT ${TARGET_NAME})
                set(TARGET_INSTALL_DESTINATION ".")
                set_property(GLOBAL APPEND PROPERTY CSTOOLKIT_INSTALL_TARGETS_ALL "${TARGET_NAME}")
            elseif((CSTOOLKIT_INSTALL_TARGETS_NB GREATER 1 AND TARGET_NAME IN_LIST CSTOOLKIT_INSTALL_TARGETS)
                    OR CSTOOLKIT_INSTALL_TARGETS_NB EQUAL 0) # mode multi component
                set(TARGET_INSTALL_LIB_DIR ${TARGET_NAME}/lib/$<LOWER_CASE:$<CONFIG>>)
                set(TARGET_INSTALL_BIN_DIR ${TARGET_NAME}/bin/$<LOWER_CASE:$<CONFIG>>)
                set(TARGET_INSTALL_SYMBOLS_DIR ${TARGET_NAME}/symbols/$<LOWER_CASE:$<CONFIG>>)
                set(TARGET_INSTALL_INCLUDE_DIR ${TARGET_NAME}/include)
                set(TARGET_INSTALL_CMAKE_DIR ${TARGET_NAME}/cmake)
                set(TARGET_INSTALL_CONFIG_NAME ${PROJECT_NAME}_${TARGET_NAME})
                set(TARGET_INSTALL_COMPONENT ${TARGET_NAME})
                set(TARGET_INSTALL_DESTINATION ${TARGET_NAME})
                set_property(GLOBAL APPEND PROPERTY CSTOOLKIT_INSTALL_TARGETS_ALL "${TARGET_NAME}")
            else() #autres components à exclure
                set(TARGET_INSTALL_EXCLUDE_FROM_ALL "EXCLUDE_FROM_ALL")
                set(TARGET_INSTALL_LIB_DIR internals/${TARGET_NAME}/lib/$<LOWER_CASE:$<CONFIG>>)
                set(TARGET_INSTALL_BIN_DIR internals/${TARGET_NAME}/bin/$<LOWER_CASE:$<CONFIG>>)
                set(TARGET_INSTALL_SYMBOLS_DIR internals/${TARGET_NAME}/symbols/$<LOWER_CASE:$<CONFIG>>)
                set(TARGET_INSTALL_INCLUDE_DIR internals/${TARGET_NAME}/include)
                set(TARGET_INSTALL_CMAKE_DIR internals/${TARGET_NAME}/cmake)
                set(TARGET_INSTALL_CONFIG_NAME ${PROJECT_NAME}_${TARGET_NAME})
                set(TARGET_INSTALL_COMPONENT ${TARGET_NAME})
                set(TARGET_INSTALL_DESTINATION internals/${TARGET_NAME})
            endif()
        endif()

        if(TARGET_RECURSIVE_INTERFACE_INCLUDE AND TARGET_RECURSIVE_PUBLIC_HEADERS_DIRS)
            foreach(_public_header_dir ${TARGET_RECURSIVE_PUBLIC_HEADERS_DIRS})
                cmake_path(RELATIVE_PATH _public_header_dir BASE_DIRECTORY ${TARGET_PUBLIC_HEADERS_DIRS} OUTPUT_VARIABLE _relative_public_header_dir)
                set(_relative_public_header_dir ${TARGET_INSTALL_INCLUDE_DIR}/${_relative_public_header_dir})
                list(APPEND TARGET_RECURSIVE_RELATIVE_PUBLIC_HEADERS_DIRS ${_relative_public_header_dir})
            endforeach()

            set(TARGET_INSTALL_INCLUDES_DESTINATION INCLUDES DESTINATION ${TARGET_RECURSIVE_RELATIVE_PUBLIC_HEADERS_DIRS})
        endif()

        install(TARGETS ${TARGET_NAME} EXPORT ${TARGET_INSTALL_CONFIG_NAME}Targets
            DESTINATION ${TARGET_INSTALL_DESTINATION}
            COMPONENT ${TARGET_INSTALL_COMPONENT}
            ${TARGET_INSTALL_EXCLUDE_FROM_ALL}
            ARCHIVE DESTINATION ${TARGET_INSTALL_LIB_DIR}
            LIBRARY DESTINATION ${TARGET_INSTALL_LIB_DIR}
            RUNTIME DESTINATION ${TARGET_INSTALL_BIN_DIR}
            FILE_SET HEADERS DESTINATION ${TARGET_INSTALL_INCLUDE_DIR}
            ${TARGET_INSTALL_INCLUDES_DESTINATION}
        )

        if(TARGET_EXECUTABLE)
            # Runtime dependencies
            install(FILES ${TARGET_RUNTIME_DLLS} DESTINATION ${TARGET_INSTALL_BIN_DIR} COMPONENT ${TARGET_INSTALL_COMPONENT} ${TARGET_INSTALL_EXCLUDE_FROM_ALL})
            if(TARGET_PLUGINS)
                install(FILES ${PLUGINS_RUNTIME_DLLS} DESTINATION ${TARGET_INSTALL_BIN_DIR}/${TARGET_PLUGINS_DIR} COMPONENT ${TARGET_INSTALL_COMPONENT} OPTIONAL ${TARGET_INSTALL_EXCLUDE_FROM_ALL})
            endif()

            # Qt
            if(_qt_modules AND CSTOOLKIT_AUTO_DEPLOY_QT)
                cstoolkit_qt_generate_deploy_app_script(
                    TARGET ${TARGET_NAME}
                    INSTALL_DIR ${TARGET_INSTALL_BIN_DIR}
                    RUNTIME_DEPENDENCIES ${TARGET_RUNTIME_DLLS} ${PLUGINS_RUNTIME_DLLS}
                    OUTPUT_SCRIPT qt_deploy_script
                    NO_UNSUPPORTED_PLATFORM_ERROR
                )
                install(SCRIPT ${qt_deploy_script} COMPONENT ${TARGET_INSTALL_COMPONENT} ${TARGET_INSTALL_EXCLUDE_FROM_ALL})
            endif()
        endif()

        # PDBS
        if(MSVC)
            if(TARGET_SHARED)
                install(FILES $<TARGET_PDB_FILE:${TARGET_NAME}> DESTINATION ${TARGET_INSTALL_BIN_DIR} COMPONENT ${TARGET_INSTALL_COMPONENT} OPTIONAL ${TARGET_INSTALL_EXCLUDE_FROM_ALL})
            elseif(TARGET_EXECUTABLE)
                install(FILES $<TARGET_PDB_FILE:${TARGET_NAME}> DESTINATION ${TARGET_INSTALL_SYMBOLS_DIR} COMPONENT ${TARGET_INSTALL_COMPONENT} OPTIONAL ${TARGET_INSTALL_EXCLUDE_FROM_ALL})
                install(FILES ${TARGET_RUNTIME_PDBS} DESTINATION ${TARGET_INSTALL_SYMBOLS_DIR} COMPONENT ${TARGET_INSTALL_COMPONENT} OPTIONAL ${TARGET_INSTALL_EXCLUDE_FROM_ALL})
                if(TARGET_PLUGINS)
                    install(FILES ${PLUGINS_RUNTIME_PDBS} DESTINATION ${TARGET_INSTALL_SYMBOLS_DIR} COMPONENT ${TARGET_INSTALL_COMPONENT} OPTIONAL ${TARGET_INSTALL_EXCLUDE_FROM_ALL})
                endif()
            elseif(TARGET_STATIC)
                install(FILES $<TARGET_FILE_DIR:${TARGET_NAME}>/$<TARGET_FILE_BASE_NAME:${TARGET_NAME}>.pdb
                    DESTINATION ${TARGET_INSTALL_LIB_DIR} COMPONENT ${TARGET_INSTALL_COMPONENT} OPTIONAL ${TARGET_INSTALL_EXCLUDE_FROM_ALL})
                if(TARGET_COMBINED_LINK_LIBRARIES)
                    install(FILES "${TARGET_COMBINED_PDBS}" DESTINATION ${TARGET_INSTALL_LIB_DIR} COMPONENT ${TARGET_INSTALL_COMPONENT} OPTIONAL ${TARGET_INSTALL_EXCLUDE_FROM_ALL})
                endif()
            endif()
        endif()

        # Config
        install(EXPORT ${TARGET_INSTALL_CONFIG_NAME}Targets
            NAMESPACE ${TARGET_NAMESPACE}
            DESTINATION ${TARGET_INSTALL_CMAKE_DIR}
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

macro (generate_target_config)

    list(APPEND INTERFACE_DEPENDENCIES ${TARGET_PUBLIC_LINK_LIBRARIES})
    if(TARGET_STATIC)
        list(APPEND INTERFACE_DEPENDENCIES ${TARGET_PRIVATE_LINK_LIBRARIES})
    endif()
    foreach(dep ${INTERFACE_DEPENDENCIES})
        string(REGEX MATCH "(.+)::(.+)" IS_PACKAGE ${dep})
        if(IS_PACKAGE)
            set(PACKAGE_NAME ${CMAKE_MATCH_1})
            set(COMPONENT_NAME ${CMAKE_MATCH_2})
            set(FIND_DEPENDENCY_COMMANDS "${FIND_DEPENDENCY_COMMANDS}find_dependency(${PACKAGE_NAME} COMPONENTS ${COMPONENT_NAME})\n")
        endif()
    endforeach()

    include(CMakePackageConfigHelpers)

    # Target Config File
    configure_package_config_file(
        ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/../templates/TargetConfig.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_INSTALL_CONFIG_NAME}Config.cmake
        INSTALL_DESTINATION ${TARGET_INSTALL_CMAKE_DIR}
    )

    install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_INSTALL_CONFIG_NAME}Config.cmake
        DESTINATION ${TARGET_INSTALL_CMAKE_DIR}
        COMPONENT ${TARGET_INSTALL_COMPONENT}
        ${TARGET_INSTALL_EXCLUDE_FROM_ALL}
    )
endmacro()
