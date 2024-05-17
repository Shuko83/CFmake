function(cstoolkit_add_target TARGET_NAME TARGET_TYPE)
    
    # Parse arguments

    set(TARGET_OPTIONS RECURSIVE RECURSIVE_INCLUDE NOINSTALL)
    set(TARGET_UNIQUE ALIAS EXTENSION PLUGINS_DIR)
    set(TARGET_MULTIPLE PUBLIC_LINK_LIBRARIES PRIVATE_LINK_LIBRARIES COMBINED_LINK_LIBRARIES PUBLIC_HEADERS_FILES PUBLIC_HEADERS_BASE_DIRS LINK_OPTIONS COMPILE_DEFINITIONS COMPILE_OPTIONS PLUGINS DEPLOY_FILES)
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

    # Paths

    if(NOT DEFINED TARGET_PUBLIC_HEADERS_BASE_DIRS)
        string(REPLACE "/_project" "" TARGET_PUBLIC_HEADERS_BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include")
    endif()
    string(REPLACE "/_project" "" TARGET_PRIVATE_HEADERS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
    string(REPLACE "/_project" "" TARGET_SOURCES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
    string(REPLACE "/_project" "" TARGET_UI_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
    string(REPLACE "/_project" "" TARGET_RESOURCES_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
    string(REPLACE "/_project" "" TARGET_TRANSLATION_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

    if(CMAKE_PROJECT_NAME STREQUAL TARGET_NAME)
        set(TARGET_INSTALL_LIB_DIR lib/$<LOWER_CASE:$<CONFIG>>)
        set(TARGET_INSTALL_BIN_DIR bin/$<LOWER_CASE:$<CONFIG>>)
        set(TARGET_INSTALL_INCLUDE_DIR include)
        set(TARGET_INSTALL_CMAKE_DIR cmake)
        set(TARGET_INSTALL_CONFIG_NAME ${PROJECT_NAME})
    else()
        set(TARGET_INSTALL_LIB_DIR ${TARGET_NAME}/lib/$<LOWER_CASE:$<CONFIG>>)
        set(TARGET_INSTALL_BIN_DIR ${TARGET_NAME}/bin/$<LOWER_CASE:$<CONFIG>>)
        set(TARGET_INSTALL_INCLUDE_DIR ${TARGET_NAME}/include)
        set(TARGET_INSTALL_CMAKE_DIR ${TARGET_NAME}/cmake)
        set(TARGET_INSTALL_CONFIG_NAME ${PROJECT_NAME}_${TARGET_NAME})
    endif()

    # Sources

    if(TARGET_RECURSIVE)
        if(NOT DEFINED TARGET_PUBLIC_HEADERS_FILES)
            file(GLOB_RECURSE TARGET_PUBLIC_HEADERS_FILES ${TARGET_PUBLIC_HEADERS_BASE_DIRS}/*)
        endif()
        file(GLOB_RECURSE TARGET_PRIVATE_HEADERS_FILES ${TARGET_PRIVATE_HEADERS_DIR}/*.h ${TARGET_PRIVATE_HEADERS_DIR}/*.hpp)
        file(GLOB_RECURSE TARGET_SOURCES_FILES ${TARGET_SOURCES_DIR}/*.c ${TARGET_SOURCES_DIR}/*.cpp)
        file(GLOB_RECURSE TARGET_UI_FILES ${TARGET_UI_DIR}/*.ui)
        file(GLOB_RECURSE TARGET_RESOURCES_FILES ${TARGET_RESOURCES_DIR}/*.qrc)
        file(GLOB_RECURSE TARGET_TRANSLATION_FILES ${TARGET_TRANSLATION_DIR}/*.ts)
    endif()

    # Library
    
    if(TARGET_SHARED OR TARGET_STATIC OR TARGET_INTERFACE)
        add_library(${TARGET_NAME} ${TARGET_TYPE} ${TARGET_SOURCES_FILES} ${TARGET_UI_FILES} ${TARGET_RESOURCES_FILES})
        if(TARGET_ALIAS)
            add_library(${TARGET_ALIAS} ALIAS ${TARGET_NAME})
        endif()
    endif()

    # Executable

    if(TARGET_EXECUTABLE)
        add_executable(${TARGET_NAME} ${TARGET_SOURCES_FILES} ${TARGET_UI_FILES} ${TARGET_RESOURCES_FILES})
        if(TARGET_ALIAS)
            add_executable(${TARGET_ALIAS} ALIAS ${TARGET_NAME})
        endif()
    endif()

    # Extension

    if (TARGET_EXTENSION)
        set_target_properties(${TARGET_NAME} PROPERTIES SUFFIX ".${TARGET_EXTENSION}")
    endif()

    # Dossier des projets

    source_group(TREE ${TARGET_PRIVATE_HEADERS_DIR} PREFIX "Header Files (Private)" FILES ${TARGET_PRIVATE_HEADERS_FILES})
    source_group(TREE ${TARGET_PUBLIC_HEADERS_BASE_DIRS} PREFIX "Header Files (Public)" FILES ${TARGET_PUBLIC_HEADERS_FILES})
    source_group(TREE ${TARGET_SOURCES_DIR} PREFIX "Source Files" FILES ${TARGET_SOURCES_FILES})
    source_group(TREE ${TARGET_UI_DIR} PREFIX "Form Files" FILES ${TARGET_UI_FILES})
    source_group(TREE ${TARGET_RESOURCES_DIR} PREFIX "Resource Files" FILES ${TARGET_RESOURCES_FILES})
    source_group(TREE ${TARGET_TRANSLATION_DIR} PREFIX "Translation Files" FILES ${TARGET_TRANSLATION_FILES})

    set( _folder "${CMAKE_CURRENT_SOURCE_DIR}/")
    cmake_path(RELATIVE_PATH _folder BASE_DIRECTORY ${CMAKE_SOURCE_DIR})
    string(REPLACE "${TARGET_NAME}/" "" _folder "${_folder}")
    string(REPLACE "Sources/" "" _folder "${_folder}")
    string(REPLACE "src/" "" _folder "${_folder}")
    string(REPLACE "test/" "" _folder "${_folder}")
    string(REPLACE "_project/" "" _folder "${_folder}")
    string(REPLACE "../" "" _folder "${_folder}")
    string(REPLACE "../" "" _folder "${_folder}")
    string(REGEX REPLACE "/$" "" _folder "${_folder}")

    set_target_properties(${TARGET_NAME} PROPERTIES FOLDER "${_folder}")

    set_target_properties(${TARGET_NAME} PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib/$<LOWER_CASE:$<CONFIG>>"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib/$<LOWER_CASE:$<CONFIG>>"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin/$<LOWER_CASE:$<CONFIG>>"
    )

    # Compile Options

    target_compile_options(${TARGET_NAME} PRIVATE ${TARGET_COMPILE_OPTIONS})

    # Compile Definitions

    string(TOUPPER ${TARGET_NAME} TARGET_NAME_UPPER)
    set_target_properties(${TARGET_NAME} PROPERTIES DEFINE_SYMBOL ${TARGET_NAME_UPPER}_LIB)

    target_compile_definitions(${TARGET_NAME} PRIVATE ${TARGET_COMPILE_DEFINITIONS})

    # Headers

    target_sources(${TARGET_NAME}
        PUBLIC FILE_SET HEADERS BASE_DIRS ${TARGET_PUBLIC_HEADERS_BASE_DIRS} FILES ${TARGET_PUBLIC_HEADERS_FILES}
        PRIVATE FILE_SET "private" TYPE HEADERS BASE_DIRS ${TARGET_PRIVATE_HEADERS_DIR} FILES ${TARGET_PRIVATE_HEADERS_FILES}
    )

    # Links

    target_link_libraries(${TARGET_NAME}
        PUBLIC ${TARGET_PUBLIC_LINK_LIBRARIES}
        PRIVATE ${TARGET_PRIVATE_LINK_LIBRARIES})
    
    # Link Options
    if(TARGET_EXECUTABLE OR TARGET_SHARED)
	    target_link_options(${TARGET_NAME} PRIVATE ${TARGET_LINK_OPTIONS})
    elseif(TARGET_LINK_OPTIONS)
        message(SEND_ERROR "CSToolkit: add_target(${TARGET_NAME}): LINK_OPTIONS defined for target with no link step.")
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

    # Generation des fichiers info_${TARGET_NAME}
    if(TARGET_SHARED OR TARGET_EXECUTABLE)
        cstoolkit_generate_target_info(TARGET ${TARGET_NAME} COPYRIGHT CSGroup COMPANY CSGroup PRODUCT ${TARGET_NAME})
    endif()

    # Qt

    # MOC
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

    # UI
    cstoolkit_qt_wrap_ui(UIC_FILES ${TARGET_UI_FILES})
    target_sources(${TARGET_NAME}
        PRIVATE FILE_SET "uic" TYPE HEADERS BASE_DIRS ${CMAKE_CURRENT_BINARY_DIR}/generated/uic FILES ${UIC_FILES}
    )
    source_group(TREE ${CMAKE_CURRENT_BINARY_DIR}/generated PREFIX "Generated Files" FILES ${UIC_FILES})

    # QRC
    cstoolkit_qt_add_resources(RCC_FILES TARGET_QRC_RESOURCES_FILES ${TARGET_RESOURCES_FILES})

    target_sources(${TARGET_NAME}
        PRIVATE ${RCC_FILES}
    )
    source_group(TREE ${CMAKE_CURRENT_BINARY_DIR}/generated PREFIX "Generated Files" FILES ${RCC_FILES})

    target_sources(${TARGET_NAME}
        PRIVATE ${TARGET_QRC_RESOURCES_FILES}
    )
    source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "Resource Files" FILES ${TARGET_QRC_RESOURCES_FILES})

    # Deployement des DLLs

    if(TARGET_EXECUTABLE)
        # Copy of runtime dlls for the target

        add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} 
            -E copy -t $<TARGET_FILE_DIR:${TARGET_NAME}> $<TARGET_RUNTIME_DLLS:${TARGET_NAME}> COMMAND_EXPAND_LISTS
        )
        
        # Copy of pdbs for the target
        
        add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
            COMMAND ${CSTOOLKIT_COPY} -e 
                "$<LIST:TRANSFORM,$<TARGET_RUNTIME_DLLS:${TARGET_NAME}>,REPLACE,\(.*\)\\.[^.]+,\\1.pdb>"
                "$<TARGET_FILE_DIR:${TARGET_NAME}>"
                COMMAND_EXPAND_LISTS 
        )

        if (TARGET_PLUGINS) 
            # Adding plusgins as dependencies
            add_dependencies(${TARGET_NAME} ${TARGET_PLUGINS})
            set_target_properties(${TARGET_NAME} PROPERTIES PLUGINS ${TARGET_PLUGINS})

            set(PLUGINS_TARGET ${TARGET_NAME}_plugins)

            add_executable(${PLUGINS_TARGET} EXCLUDE_FROM_ALL CMakeLists.txt)
            set_target_properties(${PLUGINS_TARGET} PROPERTIES EXCLUDE_FROM_DEFAULT_BUILD True)
            set_target_properties(${PLUGINS_TARGET} PROPERTIES LINKER_LANGUAGE CXX)
            target_link_libraries(${PLUGINS_TARGET} PUBLIC ${TARGET_PLUGINS})
            
            # Copy of the plugin dll and its dependencies
            add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
            COMMAND ${CSTOOLKIT_COPY} -e 
                "$<TARGET_RUNTIME_DLLS:${PLUGINS_TARGET}>"
                "$<TARGET_FILE_DIR:${TARGET_NAME}>/${TARGET_PLUGINS_DIR}" COMMAND_EXPAND_LISTS
                )
        endif()
    elseif(TARGET_PLUGINS)
        message(SEND_ERROR "CSToolkit: add_target(${TARGET_NAME}): Unsuported argument PLUGINS for non-executable target.")
    endif()

    # Deploy of .deploy files

    foreach(_deploy_file ${TARGET_DEPLOY_FILES})
        cstoolkit_deploy(${_deploy_file})
    endforeach()

    # Installation

    if(NOT TARGET_NOINSTALL)

        # Only if recursive includes is activated
        if (RELATIVE_PUBLIC_HEADER_DIRECTORIES)
            set(INCLUDES_PARAMS INCLUDES DESTINATION ${RELATIVE_PUBLIC_HEADER_DIRECTORIES})
        endif()

        install(TARGETS ${TARGET_NAME} EXPORT ${TARGET_INSTALL_CONFIG_NAME}Targets DESTINATION ${TARGET_NAME}
            ARCHIVE DESTINATION ${TARGET_INSTALL_LIB_DIR}
            RUNTIME DESTINATION ${TARGET_INSTALL_BIN_DIR}
            FILE_SET HEADERS DESTINATION ${TARGET_INSTALL_INCLUDE_DIR}
            ${INCLUDES_PARAMS}              
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
                install(FILES $<TARGET_PDB_FILE:${TARGET_NAME}> DESTINATION ${TARGET_INSTALL_BIN_DIR} OPTIONAL)
            elseif(TARGET_STATIC)
                # Default pdb output is not next to .lib
                set_target_properties(${TARGET_NAME} PROPERTIES COMPILE_PDB_OUTPUT_DIRECTORY $<TARGET_FILE_DIR:${TARGET_NAME}>)
                # Necessary to redefine name for msvc 2015
                # COMPILE_PDB_NAME does not support generator expression
                set_target_properties(${TARGET_NAME} PROPERTIES COMPILE_PDB_NAME_DEBUG ${TARGET_NAME}${CMAKE_DEBUG_POSTFIX})
                install(FILES $<TARGET_FILE_DIR:${TARGET_NAME}>/$<TARGET_FILE_BASE_NAME:${TARGET_NAME}>.pdb
                    DESTINATION ${TARGET_INSTALL_LIB_DIR} CONFIGURATIONS Debug)
            endif()
        endif()

        # Config
        install(EXPORT ${TARGET_INSTALL_CONFIG_NAME}Targets
            NAMESPACE ${PROJECT_NAME}::
            DESTINATION ${TARGET_INSTALL_CMAKE_DIR}
        )
        generate_target_config()
    endif()

    # Debug Messages

    #message(STATUS "==================================================")
    #message(STATUS "Component ${TARGET_NAME} is ${TARGET_TYPE}")
    #message(STATUS "  - Dependencies: ${ALL_DEPS}")
    #message(STATUS "  - Public headers path: ${TARGET_PUBLIC_HEADERS_BASE_DIRS}")
    #message(STATUS "  - Private headers path: ${TARGET_PRIVATE_HEADERS_DIR}")
    #message(STATUS "  - Sources path: ${TARGET_SOURCES_DIR}")
    #message(STATUS "  - Recursive: ${TARGET_RECURSIVE}")
    #message(DEBUG  "  - Public headers files: ${TARGET_PUBLIC_HEADERS_FILES}")
    #message(DEBUG  "  - Private headers files: ${TARGET_PRIVATE_HEADERS_FILES}")
    #message(DEBUG  "  - Sources files: ${TARGET_SOURCES_FILES}")
    #message(DEBUG  "  - Public link libraries: ${TARGET_PUBLIC_LINK_LIBRARIES}")
    #message(DEBUG  "  - Private link libraries: ${TARGET_PRIVATE_LINK_LIBRARIES}")
    #message(DEBUG  "  - Runtime dependencies: ${TARGET_RUNTIME_DEPS}")
    #if(NOT TARGET_NOINSTALL)
    #message(STATUS "  - Installation: Default")
    #endif()

    #find_package(Qt5 ${Qt5_VERSION} COMPONENTS "Core")
endfunction()

macro (generate_target_config)

    list(APPEND INTERFACE_DEPENDENCIES ${TARGET_PUBLIC_LINK_LIBRARIES})
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
    )
endmacro()
