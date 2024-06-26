function(cstoolkit_add_target TARGET_NAME TARGET_TYPE)
    
    # Parse arguments

    set(TARGET_OPTIONS RECURSIVE RECURSIVE_INCLUDE NO_INSTALL PUBLIC_HEADERS_NO_EXTENSION)
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
        message("on set a true ${TARGET_NAME}")
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

    # Extension

    if (TARGET_EXTENSION)
        set_target_properties(${TARGET_NAME} PROPERTIES SUFFIX ".${TARGET_EXTENSION}")
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

    if(TARGET_RECURSIVE)
        if(NOT DEFINED TARGET_PUBLIC_HEADERS_FILES)
            if(TARGET_PUBLIC_HEADERS_NO_EXTENSION)
                file(GLOB_RECURSE TARGET_PUBLIC_HEADERS_FILES ${TARGET_PUBLIC_HEADERS_DIRS}/*)
            else()
                file(GLOB_RECURSE TARGET_PUBLIC_HEADERS_FILES ${TARGET_PUBLIC_HEADERS_DIRS}/*.h ${TARGET_PUBLIC_HEADERS_DIRS}/*.hpp)
            endif()
        endif()
        if(NOT DEFINED TARGET_PRIVATE_HEADERS_FILES)
            file(GLOB_RECURSE TARGET_PRIVATE_HEADERS_FILES ${TARGET_PRIVATE_HEADERS_DIRS}/*.h ${TARGET_PRIVATE_HEADERS_DIRS}/*.hpp)
        endif()
        if(NOT DEFINED TARGET_SOURCES_FILES)
            file(GLOB_RECURSE TARGET_SOURCES_FILES ${TARGET_SOURCES_DIRS}/*.c ${TARGET_SOURCES_DIRS}/*.cpp)
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

    # Compile Options

    target_compile_options(${TARGET_NAME} PRIVATE ${TARGET_COMPILE_OPTIONS})

    # Compile Definitions

    target_compile_definitions(${TARGET_NAME} PRIVATE ${TARGET_COMPILE_DEFINITIONS})

    # Default Definition for Export symbols
    string(TOUPPER ${TARGET_NAME} TARGET_NAME_UPPER)
    set_target_properties(${TARGET_NAME} PROPERTIES DEFINE_SYMBOL ${TARGET_NAME_UPPER}_LIB)

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

    # Recursive include, this option allows to include without specifying all paths to the header file 
    # The code bellow fetches all parent directories of header files and adds them to the include directory of the target (PRIVATE and INTERFACE)
    if (TARGET_RECURSIVE_INCLUDE)
        # Get all directories of private headers file
        list(APPEND ALL_HEADER_FILES "${TARGET_PRIVATE_HEADERS_FILES};${TARGET_PUBLIC_HEADERS_FILES}")
        foreach(headerFile ${ALL_HEADER_FILES})
            cmake_path(GET headerFile PARENT_PATH header_dir_path)
            list(APPEND ALL_HEADER_DIRECTORIES ${header_dir_path})
        endforeach()
        list(REMOVE_DUPLICATES ALL_HEADER_DIRECTORIES)

        # Get all directories of public headers file
        list(APPEND PUBLIC_HEADER_FILES ${TARGET_PUBLIC_HEADERS_FILES})
        foreach(publicHeaderFile ${PUBLIC_HEADER_FILES})
            cmake_path(GET publicHeaderFile PARENT_PATH public_header_dir_path)
            list(APPEND PUBLIC_HEADER_DIRECTORIES ${public_header_dir_path})
        endforeach()
        list(REMOVE_DUPLICATES PUBLIC_HEADER_DIRECTORIES)

        # Get relative directory path of PUBLIC_HEADER_DIRECTORIES for the install()
        foreach(directory ${PUBLIC_HEADER_DIRECTORIES})
            cmake_path(RELATIVE_PATH directory BASE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR})/.." OUTPUT_VARIABLE relative_dir_path)
            list(APPEND RELATIVE_PUBLIC_HEADER_DIRECTORIES ${relative_dir_path} )
        endforeach()

        # Adding generator expression to all directories
        set(GENERATOR_PUBLIC_HEADER_DIRECTORIES ${PUBLIC_HEADER_DIRECTORIES})
        list(TRANSFORM GENERATOR_PUBLIC_HEADER_DIRECTORIES PREPEND "$<BUILD_INTERFACE:")
        list(TRANSFORM GENERATOR_PUBLIC_HEADER_DIRECTORIES APPEND ">")

        if (NOT TARGET_INTERFACE)
            target_include_directories(${TARGET_NAME} PRIVATE ${ALL_HEADER_DIRECTORIES})
        endif()
        target_include_directories(${TARGET_NAME} INTERFACE ${GENERATOR_PUBLIC_HEADER_DIRECTORIES})
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
            )

            # Filtering MOC
            
            cstoolkit_filter_moc(${TARGET_SOURCES_FILES} ${TARGET_PRIVATE_HEADERS_FILES} ${TARGET_PUBLIC_HEADERS_FILES})

            target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/generated/moc)

            target_sources(${TARGET_NAME}
                PRIVATE ${MOC_FILES}
            )
            source_group(TREE ${CMAKE_CURRENT_BINARY_DIR}/generated PREFIX "Generated Files" FILES ${MOC_FILES})
        endif()

        # UI
        if(NOT CMAKE_AUTOUIC)
            cstoolkit_qt_wrap_ui(UIC_FILES ${TARGET_UI_FILES})
            target_sources(${TARGET_NAME}
                PRIVATE FILE_SET "uic" TYPE HEADERS BASE_DIRS ${CMAKE_CURRENT_BINARY_DIR}/generated/uic FILES ${UIC_FILES}
            )
            source_group(TREE ${CMAKE_CURRENT_BINARY_DIR}/generated PREFIX "Generated Files" FILES ${UIC_FILES})
        endif()

        # QRC
        if(NOT CMAKE_AUTORCC)
            cstoolkit_qt_add_resources(RCC_FILES TARGET_QRC_RESOURCES_FILES ${TARGET_RESOURCES_FILES})

            target_sources(${TARGET_NAME}
                PRIVATE ${RCC_FILES}
            )
            source_group(TREE ${CMAKE_CURRENT_BINARY_DIR}/generated PREFIX "Generated Files" FILES ${RCC_FILES})

            target_sources(${TARGET_NAME}
                PRIVATE ${TARGET_QRC_RESOURCES_FILES}
            )
            source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "Resource Files" FILES ${TARGET_QRC_RESOURCES_FILES})
        endif()
    
    endif()

    # Generation des fichiers info_${TARGET_NAME}
    if(NOT TARGET_INTERFACE)
        string(TIMESTAMP _year "%Y" UTC)

        if(TARGET_EXECUTABLE AND _qt_modules)
            set(_generate_qt_info "QT_EXECUTABLE")
        endif()

        cstoolkit_generate_target_info(
            TARGET "${TARGET_NAME}"
            VERSION "${PROJECT_VERSION}"
            PRODUCT "${PROJECT_NAME}"
            ORGANIZATION "CSGroup"
            DOMAIN "https://www.csgroup.eu"
            COPYRIGHT "Copyright \\251 ${_year} CSGroup"
            ${_generate_qt_info}
        )
    endif()

    # Deployement des DLLs

    if(TARGET_EXECUTABLE)
        # Copy of runtime dlls and pdbs for the target
        if(Qt5_INSTALL_PREFIX) #Filtering of Qt's dlls necessary for development
            add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} 
                -E copy -t "$<TARGET_FILE_DIR:${TARGET_NAME}>" "$<FILTER:$<TARGET_RUNTIME_DLLS:${TARGET_NAME}>,EXCLUDE,^${Qt5_INSTALL_PREFIX}>" COMMAND_EXPAND_LISTS
            )
            
            add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
                COMMAND ${CSTOOLKIT_COPY} -e 
                    "$<LIST:TRANSFORM,$<FILTER:$<TARGET_RUNTIME_DLLS:${TARGET_NAME}>,EXCLUDE,^${Qt5_INSTALL_PREFIX}>,REPLACE,\(.*\)\\.[^.]+,\\1.pdb>"
                    "$<TARGET_FILE_DIR:${TARGET_NAME}>"
                    COMMAND_EXPAND_LISTS 
            )
        else()
            add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} 
                -E copy -t "$<TARGET_FILE_DIR:${TARGET_NAME}>" "$<TARGET_RUNTIME_DLLS:${TARGET_NAME}>" COMMAND_EXPAND_LISTS
            )
            
            add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
                COMMAND ${CSTOOLKIT_COPY} -e 
                    "$<LIST:TRANSFORM,$<TARGET_RUNTIME_DLLS:${TARGET_NAME}>,REPLACE,\(.*\)\\.[^.]+,\\1.pdb>"
                    "$<TARGET_FILE_DIR:${TARGET_NAME}>"
                    COMMAND_EXPAND_LISTS 
            )
        endif()

        if (TARGET_PLUGINS) 
            # Adding plusgins as dependencies
            add_dependencies(${TARGET_NAME} ${TARGET_PLUGINS})
            set_target_properties(${TARGET_NAME} PROPERTIES PLUGINS ${TARGET_PLUGINS})

            set(PLUGINS_TARGET ${TARGET_NAME}_plugins)

            add_executable(${PLUGINS_TARGET} EXCLUDE_FROM_ALL CMakeLists.txt)
            set_target_properties(${PLUGINS_TARGET} PROPERTIES EXCLUDE_FROM_DEFAULT_BUILD True)
            set_target_properties(${PLUGINS_TARGET} PROPERTIES LINKER_LANGUAGE CXX)
            target_link_libraries(${PLUGINS_TARGET} PUBLIC ${TARGET_PLUGINS})
            
            if(Qt5_INSTALL_PREFIX) #Filtering of Qt's dlls necessary for development
                # Copy of the plugin dll and its dependencies
                add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
                COMMAND ${CSTOOLKIT_COPY} -e 
                    "$<FILTER:$<TARGET_RUNTIME_DLLS:${PLUGINS_TARGET}>,EXCLUDE,^${Qt5_INSTALL_PREFIX}>"
                    "$<TARGET_FILE_DIR:${TARGET_NAME}>/${TARGET_PLUGINS_DIR}" COMMAND_EXPAND_LISTS
                )
            else()
                # Copy of the plugin dll and its dependencies
                add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
                COMMAND ${CSTOOLKIT_COPY} -e 
                    "$<TARGET_RUNTIME_DLLS:${PLUGINS_TARGET}>"
                    "$<TARGET_FILE_DIR:${TARGET_NAME}>/${TARGET_PLUGINS_DIR}" COMMAND_EXPAND_LISTS
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

            if(CSTOOLKIT_INSTALL_TARGETS_NB EQUAL 1) # mode single component
                if(TARGET_NAME STREQUAL PROJECT_NAME) # LE component
                    set(TARGET_INSTALL_LIB_DIR lib/$<LOWER_CASE:$<CONFIG>>)
                    set(TARGET_INSTALL_BIN_DIR bin/$<LOWER_CASE:$<CONFIG>>)
                    set(TARGET_INSTALL_INCLUDE_DIR include)
                    set(TARGET_INSTALL_CMAKE_DIR cmake)
                    set(TARGET_INSTALL_CONFIG_NAME ${TARGET_NAME})
                    set(TARGET_INSTALL_COMPONENT ${TARGET_NAME})
                    set(TARGET_INSTALL_DESTINATION ".")
                    set_property(GLOBAL APPEND PROPERTY CSTOOLKIT_INSTALL_TARGETS_ALL "${TARGET_NAME}")
                else() #autres components à exclure
                    set(TARGET_INSTALL_EXCLUDE_FROM_ALL "EXCLUDE_FROM_ALL")
                    set(TARGET_INSTALL_LIB_DIR internals/${TARGET_NAME}/lib/$<LOWER_CASE:$<CONFIG>>)
                    set(TARGET_INSTALL_BIN_DIR internals/${TARGET_NAME}/bin/$<LOWER_CASE:$<CONFIG>>)
                    set(TARGET_INSTALL_INCLUDE_DIR internals/${TARGET_NAME}/include)
                    set(TARGET_INSTALL_CMAKE_DIR internals/${TARGET_NAME}/cmake)
                    set(TARGET_INSTALL_CONFIG_NAME ${PROJECT_NAME}_${TARGET_NAME})
                    set(TARGET_INSTALL_COMPONENT ${TARGET_NAME})
                    set(TARGET_INSTALL_DESTINATION internals/${TARGET_NAME})
                endif()
            else() # mode multi component
                set(TARGET_INSTALL_LIB_DIR ${TARGET_NAME}/lib/$<LOWER_CASE:$<CONFIG>>)
                set(TARGET_INSTALL_BIN_DIR ${TARGET_NAME}/bin/$<LOWER_CASE:$<CONFIG>>)
                set(TARGET_INSTALL_INCLUDE_DIR ${TARGET_NAME}/include)
                set(TARGET_INSTALL_CMAKE_DIR ${TARGET_NAME}/cmake)
                set(TARGET_INSTALL_CONFIG_NAME ${PROJECT_NAME}_${TARGET_NAME})
                set(TARGET_INSTALL_COMPONENT ${TARGET_NAME})
                set(TARGET_INSTALL_DESTINATION ${TARGET_NAME})
                set_property(GLOBAL APPEND PROPERTY CSTOOLKIT_INSTALL_TARGETS_ALL "${TARGET_NAME}")
            endif()
        endif()

        # Only if recursive includes is activated
        if (RELATIVE_PUBLIC_HEADER_DIRECTORIES)
            set(TARGET_INSTALL_INCLUDES_DESTINATION INCLUDES DESTINATION ${RELATIVE_PUBLIC_HEADER_DIRECTORIES})
        endif()

        install(TARGETS ${TARGET_NAME} EXPORT ${TARGET_INSTALL_CONFIG_NAME}Targets
            DESTINATION ${TARGET_INSTALL_DESTINATION}
            COMPONENT ${TARGET_INSTALL_COMPONENT}
            ${TARGET_INSTALL_EXCLUDE_FROM_ALL}
            ARCHIVE DESTINATION ${TARGET_INSTALL_LIB_DIR}
            RUNTIME DESTINATION ${TARGET_INSTALL_BIN_DIR}
            FILE_SET HEADERS DESTINATION ${TARGET_INSTALL_INCLUDE_DIR}
            ${TARGET_INSTALL_INCLUDES_DESTINATION}
            #      LIBRARY DESTINATION ${TARGET_NAME}/LIBRARY
            #      OBJECTS DESTINATION ${TARGET_NAME}/OBJECTS
            #      FRAMEWORK DESTINATION ${TARGET_NAME}/FRAMEWORK
            #      BUNDLE DESTINATION ${TARGET_NAME}/BUNDLE
            #      PUBLIC_HEADER DESTINATION ${TARGET_NAME}/PUBLIC_HEADER
            #      PRIVATE_HEADER DESTINATION ${TARGET_NAME}/PRIVATE_HEADER
            #      RESOURCE DESTINATION ${TARGET_NAME}/RESOURCE
            )

        # PDBS
        if(MSVC)
            if(TARGET_SHARED OR TARGET_EXECUTABLE)
                install(FILES $<TARGET_PDB_FILE:${TARGET_NAME}> DESTINATION ${TARGET_INSTALL_BIN_DIR} COMPONENT ${TARGET_INSTALL_COMPONENT} OPTIONAL ${TARGET_INSTALL_EXCLUDE_FROM_ALL})
            elseif(TARGET_STATIC)
                # Default pdb output is not next to .lib
                set_target_properties(${TARGET_NAME} PROPERTIES COMPILE_PDB_OUTPUT_DIRECTORY $<TARGET_FILE_DIR:${TARGET_NAME}>)
                # Necessary to redefine name for msvc 2015
                # COMPILE_PDB_NAME does not support generator expression
                set_target_properties(${TARGET_NAME} PROPERTIES COMPILE_PDB_NAME_DEBUG ${TARGET_NAME}${CMAKE_DEBUG_POSTFIX})
                install(FILES $<TARGET_FILE_DIR:${TARGET_NAME}>/$<TARGET_FILE_BASE_NAME:${TARGET_NAME}>.pdb
                    DESTINATION ${TARGET_INSTALL_LIB_DIR} CONFIGURATIONS Debug COMPONENT ${TARGET_INSTALL_COMPONENT} ${TARGET_INSTALL_EXCLUDE_FROM_ALL})
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
    message(VERBOSE "  - Public headers path: ${TARGET_PUBLIC_HEADERS_BASE_DIRS}")
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
