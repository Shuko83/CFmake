function(cstoolkit_add_target TARGET_NAME TARGET_TYPE)
    
    # Parse arguments

    set(TARGET_OPTIONS RECURSIVE NOINSTALL)
    set(TARGET_UNIQUE ALIAS)
    set(TARGET_MULTIPLE PUBLIC_LINK_LIBRARIES PRIVATE_LINK_LIBRARIES PUBLIC_HEADERS_FILES PUBLIC_HEADERS_BASE_DIR INTERFACE_INCLUDE_DIRECTORIES LINK_OPTIONS COMPILE_DEFINITIONS COMPILE_OPTIONS RUNTIME_DEPS)
    cmake_parse_arguments(TARGET "${TARGET_OPTIONS}" "${TARGET_UNIQUE}" "${TARGET_MULTIPLE}" ${ARGN})

    #message("\nadd_target(${TARGET_NAME})")

    if(DEFINED TARGET_UNPARSED_ARGUMENTS)
        message(SEND_ERROR "CSToolkit: add_target(): Unkown arguments \"${TARGET_UNPARSED_ARGUMENTS}\"")
    endif()

    if(TARGET_NAME STREQUAL "")
        message(SEND_ERROR "CSToolkit: add_target(): No NAME defined for target")
        return()
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

    # Paths

    if(NOT DEFINED TARGET_PUBLIC_HEADERS_BASE_DIR)
        string(REPLACE "/_project" "" TARGET_PUBLIC_HEADERS_BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
    endif()
    string(REPLACE "/_project" "" TARGET_PRIVATE_HEADERS_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
    string(REPLACE "/_project" "" TARGET_SOURCES_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
    string(REPLACE "/_project" "" TARGET_UI_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
    string(REPLACE "/_project" "" TARGET_RESOURCES_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
    string(REPLACE "/_project" "" TARGET_TRANSLATION_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

    if(CMAKE_PROJECT_NAME STREQUAL TARGET_NAME)
        set(TARGET_INSTALL_LIB_DIR lib/$<LOWER_CASE:$<CONFIG>>)
        set(TARGET_INSTALL_BIN_DIR bin/$<LOWER_CASE:$<CONFIG>>)
        set(TARGET_INSTALL_INCLUDE_DIR include)
        set(TARGET_INSTALL_CMAKE_DIR cmake)
    else()
        set(TARGET_INSTALL_LIB_DIR ${TARGET_NAME}/lib/$<LOWER_CASE:$<CONFIG>>)
        set(TARGET_INSTALL_BIN_DIR ${TARGET_NAME}/bin/$<LOWER_CASE:$<CONFIG>>)
        set(TARGET_INSTALL_INCLUDE_DIR ${TARGET_NAME}/include)
        set(TARGET_INSTALL_CMAKE_DIR ${TARGET_NAME}/cmake)
    endif()

    # Sources

    if(TARGET_RECURSIVE)
        if(NOT DEFINED TARGET_PUBLIC_HEADERS_FILES)
            file(GLOB_RECURSE TARGET_PUBLIC_HEADERS_FILES ${TARGET_PUBLIC_HEADERS_BASE_DIR}/*.h ${TARGET_PUBLIC_HEADERS_BASE_DIR}/*.hpp)
        endif()
        file(GLOB_RECURSE TARGET_PRIVATE_HEADERS_FILES ${TARGET_PRIVATE_HEADERS_DIR}/*.h ${TARGET_PRIVATE_HEADERS_DIR}/*.hpp)
        file(GLOB_RECURSE TARGET_SOURCES_FILES ${TARGET_SOURCES_DIR}/*.c ${TARGET_SOURCES_DIR}/*.cpp)
        file(GLOB_RECURSE TARGET_UI_FILES ${TARGET_UI_DIR}/*.ui)
        file(GLOB_RECURSE TARGET_RESOURCES_FILES ${TARGET_RESOURCES_DIR}/*.qrc)
        file(GLOB_RECURSE TARGET_TRANSLATION_FILES ${TARGET_TRANSLATION_DIR}/*.ts)
    endif()

    list(REMOVE_ITEM TARGET_PRIVATE_HEADERS_FILES ${TARGET_PUBLIC_HEADERS_FILES})

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

    # Dossier des projets

    get_filename_component(ABSOLUTE_HEADERS_BASE_DIR_PATH ${TARGET_PUBLIC_HEADERS_BASE_DIR} ABSOLUTE)
    source_group(TREE ${TARGET_PRIVATE_HEADERS_DIR} PREFIX "Header Files (Private)" FILES ${TARGET_PRIVATE_HEADERS_FILES})
    source_group(TREE ${ABSOLUTE_HEADERS_BASE_DIR_PATH} PREFIX "Header Files (Public)" FILES ${TARGET_PUBLIC_HEADERS_FILES})
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
        PUBLIC FILE_SET HEADERS BASE_DIRS ${TARGET_PUBLIC_HEADERS_BASE_DIR} FILES ${TARGET_PUBLIC_HEADERS_FILES}
        PRIVATE FILE_SET "private" TYPE HEADERS BASE_DIRS ${TARGET_PRIVATE_HEADERS_DIR} FILES ${TARGET_PRIVATE_HEADERS_FILES}
    )

    # Links

    cstoolkit_target_link_libraries(${TARGET_NAME}
        PUBLIC ${TARGET_PUBLIC_LINK_LIBRARIES}
        PRIVATE ${TARGET_PRIVATE_LINK_LIBRARIES})
    
    # Link Options
	target_link_options(${TARGET_NAME} PRIVATE ${TARGET_LINK_OPTIONS})
    # Get all directories of headers file
    set(HEADERS_INCLUDE_DIR ${TARGET_PRIVATE_HEADERS_FILES})
    list(TRANSFORM HEADERS_INCLUDE_DIR REPLACE "(.*)/[^/]+" "\\1") 
    list(REMOVE_DUPLICATES HEADERS_INCLUDE_DIR)
    
    # Get absolute path
    foreach(interface_dir_path ${TARGET_INTERFACE_INCLUDE_DIRECTORIES})
        message("${TARGET_NAME} DIR_PATH : ${interface_dir_path}")
        cmake_path(ABSOLUTE_PATH interface_dir_path BASE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} OUTPUT_VARIABLE absolute_interface_dir_path)
        list(APPEND ABSOLUTE_INTERFACE_INCLUDE_DIRECTORIES ${absolute_interface_dir_path})
    endforeach()
    
    # Adding generator expression to all directories
    list(TRANSFORM ABSOLUTE_INTERFACE_INCLUDE_DIRECTORIES PREPEND "$<BUILD_INTERFACE:")
    list(TRANSFORM ABSOLUTE_INTERFACE_INCLUDE_DIRECTORIES APPEND ">")

    if (NOT TARGET_INTERFACE)
        target_include_directories(${TARGET_NAME} PRIVATE ${HEADERS_INCLUDE_DIR})
    endif()

    target_include_directories(${TARGET_NAME} INTERFACE ${ABSOLUTE_INTERFACE_INCLUDE_DIRECTORIES})
    
    
    target_sources(${TARGET_NAME}
    PUBLIC FILE_SET HEADERS BASE_DIRS ${TARGET_PUBLIC_HEADERS_BASE_DIR} FILES ${TARGET_PUBLIC_HEADERS_FILES}
    PRIVATE FILE_SET "private" TYPE HEADERS BASE_DIRS ${TARGET_PRIVATE_HEADERS_DIR} FILES ${TARGET_PRIVATE_HEADERS_FILES}
    )
    
    
    list(APPEND DEPENDENCIES ${TARGET_PUBLIC_LINK_LIBRARIES} ${TARGET_PRIVATE_LINK_LIBRARIES} ${TARGET_RUNTIME_DEPS})
    set_target_properties(${TARGET_NAME} PROPERTIES "DEPENDENCIES" "${DEPENDENCIES}")

    cstoolkit_target_link_libraries(${TARGET_NAME}
        PUBLIC ${TARGET_PUBLIC_LINK_LIBRARIES}
        PRIVATE ${TARGET_PRIVATE_LINK_LIBRARIES})

    if(TARGET_SHARED OR TARGET_EXECUTABLE)
        cstoolkit_generate_target_info(TARGET ${TARGET_NAME} COPYRIGHT CSGroup COMPANY CSGroup PRODUCT ${TARGET_NAME})
    endif()

    # Qt

    cstoolkit_qt5_wrap_cpp(MOC_FILES TARGET ${TARGET_NAME}
        ${TARGET_PRIVATE_HEADERS_FILES} ${TARGET_PUBLIC_HEADERS_FILES}
    )
    target_sources(${TARGET_NAME}
        PRIVATE ${MOC_FILES}
    )
    source_group(TREE ${CMAKE_CURRENT_BINARY_DIR}/generated PREFIX "Generated Files" FILES ${MOC_FILES})

    cstoolkit_qt5_wrap_ui(UIC_FILES ${TARGET_UI_FILES})
    target_sources(${TARGET_NAME}
        PRIVATE FILE_SET "uic" TYPE HEADERS BASE_DIRS ${CMAKE_CURRENT_BINARY_DIR}/generated/uic FILES ${UIC_FILES}
    )
    source_group(TREE ${CMAKE_CURRENT_BINARY_DIR}/generated PREFIX "Generated Files" FILES ${UIC_FILES})

    # Deployement des DLLs

    if(TARGET_EXECUTABLE)


        # add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
        #    COMMAND ${CMAKE_COMMAND} -E copy_if_different 
        #    $<LIST:TRANSFORM,$<TARGET_RUNTIME_DLLS:${TARGET_NAME}>,REPLACE,\(.*\)\\.[^.]+,\\1.pdb> 
        #    $<TARGET_FILE_DIR:${TARGET_NAME}> COMMAND_EXPAND_LISTS
        # )
        
        # Copy of runtime dlls for the target

        add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} 
            -E copy -t $<TARGET_FILE_DIR:${TARGET_NAME}> $<TARGET_RUNTIME_DLLS:${TARGET_NAME}> COMMAND_EXPAND_LISTS
        )

        # Copy of pdbs for the target
        
        add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND}
                -DFILE_LIST="$<LIST:TRANSFORM,$<TARGET_RUNTIME_DLLS:${TARGET_NAME}>,REPLACE,\(.*\)\\.[^.]+,\\1.pdb>"
                -DDESTINATION="$<TARGET_FILE_DIR:${TARGET_NAME}>"
                -P C:/CSToolkit/CopyIfExist.cmake COMMAND_EXPAND_LISTS
        )

    endif()

    # Installation

    # Adding include to all path because headers are installed in include directory 
    set(INCLUDE_DESTINATION_DIRECTORIES ${TARGET_INTERFACE_INCLUDE_DIRECTORIES})
    list(TRANSFORM INCLUDE_DESTINATION_DIRECTORIES PREPEND "${TARGET_INSTALL_INCLUDE_DIR}/")

    if(NOT TARGET_NOINSTALL)
        install(TARGETS ${TARGET_NAME} EXPORT ${PROJECT_NAME}_${TARGET_NAME}Targets DESTINATION ${TARGET_NAME}
            ARCHIVE DESTINATION ${TARGET_INSTALL_LIB_DIR}
            RUNTIME DESTINATION ${TARGET_INSTALL_BIN_DIR}
            FILE_SET HEADERS DESTINATION ${TARGET_INSTALL_INCLUDE_DIR}
            INCLUDES DESTINATION ${INCLUDE_DESTINATION_DIRECTORIES}
        #      LIBRARY DESTINATION ${TARGET_NAME}/LIBRARY
        #      OBJECTS DESTINATION ${TARGET_NAME}/OBJECTS
        #      FRAMEWORK DESTINATION ${TARGET_NAME}/FRAMEWORK
        #      BUNDLE DESTINATION ${TARGET_NAME}/BUNDLE
        #      PUBLIC_HEADER DESTINATION ${TARGET_NAME}/PUBLIC_HEADER
        #      PRIVATE_HEADER DESTINATION ${TARGET_NAME}/PRIVATE_HEADER
        #      RESOURCE DESTINATION ${TARGET_NAME}/RESOURCE
        )
        
        # PDBS
        if(TARGET_SHARED OR TARGET_EXECUTABLE)
            install(FILES $<TARGET_PDB_FILE:${TARGET_NAME}> DESTINATION ${TARGET_INSTALL_BIN_DIR} OPTIONAL)
        endif()
        #if(TARGET_SHARED OR TARGET_EXECUTABLE)
        # install(FILES $<PROPERTY:${TARGET_NAME},COMPILE_PDB_NAME> DESTINATION ${TARGET_INSTALL_BIN_DIR} OPTIONAL)
        #endif()

        # Config
        install(EXPORT ${PROJECT_NAME}_${TARGET_NAME}Targets
            NAMESPACE ${PROJECT_NAME}::
            DESTINATION ${TARGET_INSTALL_CMAKE_DIR}
        )
        generate_target_config()
    endif()

    # Debug Messages

    #message(STATUS "==================================================")
    #message(STATUS "Component ${TARGET_NAME} is ${TARGET_TYPE}")
    #message(STATUS "  - Dependencies: ${ALL_DEPS}")
    #message(STATUS "  - Public headers path: ${TARGET_PUBLIC_HEADERS_BASE_DIR}")
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
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}_${TARGET_NAME}Config.cmake
        INSTALL_DESTINATION ${TARGET_INSTALL_CMAKE_DIR}
    )

    install(FILES 
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}_${TARGET_NAME}Config.cmake
        DESTINATION ${TARGET_INSTALL_CMAKE_DIR}
    )
endmacro()

function(cstoolkit_target_link_libraries target)
    cmake_parse_arguments(link_libraries_custom "" "" "PRIVATE;PUBLIC;INTERFACE" ${ARGN})
    #message("target_link_libraries_custom " ${target})
    #message("PRIVATE : " ${link_libraries_custom_PRIVATE})
    #message("PUBLIC : " ${link_libraries_custom_PUBLIC})
    #message("INTERFACE : " ${link_libraries_custom_INTERFACE})



    cmake_language(EVAL CODE
        "cmake_language(DEFER DIRECTORY \${CMAKE_SOURCE_DIR} CALL target_link_libraries_post_configure [[${target}]]
        PRIVATE [[${link_libraries_custom_PRIVATE}]] PUBLIC [[${link_libraries_custom_PUBLIC}]] INTERFACE [[${link_libraries_custom_INTERFACE}]])"
    )
endfunction()

function(target_link_libraries_post_configure target)
    cmake_parse_arguments(link_libraries_post "" "" "PRIVATE;PUBLIC;INTERFACE" ${ARGN})
    #message("target_link_libraries_post_configure " ${target})
    #message("PRIVATE : " ${link_libraries_post_PRIVATE})
    #message("PUBLIC : " ${link_libraries_post_PUBLIC})
    #message("INTERFACE : " ${link_libraries_post_INTERFACE})
    
    get_target_property(DEPENDENCIES ${target} DEPENDENCIES)

    foreach(lib ${DEPENDENCIES})
        #message("Library: ${lib}")
        if(TARGET ${lib})
            continue()
        endif()
        string(REGEX MATCH "(.+)::(.+)" IS_PACKAGE ${lib})
        if(IS_PACKAGE)
            set(PACKAGE_NAME ${CMAKE_MATCH_1})
            set(COMPONENT_NAME ${CMAKE_MATCH_2})
            #message("Namespace: ${PACKAGE_NAME}")
            #message("LibName: ${COMPONENT_NAME}")
            #message(${lib} " target not found")
           
            if(${PACKAGE_NAME} STREQUAL "Qt5") # Cas special Qt
                # Récupérer ce qui se trouve après "Qt5::"
                list(APPEND QT5_MODULES ${COMPONENT_NAME})
                continue()
            endif()

            find_package(${PACKAGE_NAME} QUIET COMPONENTS ${COMPONENT_NAME})

            if(${PACKAGE_NAME}_FOUND)
                if(NOT TARGET ${lib})
                    message(WARNING "CSToolkit: ${target}: find_package success for dependency ${lib} but target is missing")
                endif()
                continue()
            elseif(NOT ${PACKAGE_NAME}_NOT_FOUND_MESSAGE)
                message(WARNING "CSToolkit: ${target}: find_package failed for dependency ${lib}\n"
                                "${${PACKAGE_NAME}_NOT_FOUND_MESSAGE}")
                continue()
            endif()

            message(WARNING "CSToolkit: ${target}: find_package failed for dependency ${lib}")

        else() # NOT A PACKAGE
            #message("${target}: ${lib} is not a Package")
            find_library(${lib}_LIB ${lib} NO_CACHE PATHS ${_WINDOWS_KITS_LIB_DIR})
            #message("${target}: ${lib}_LIB = ${${lib}_LIB}")
            if(NOT ${${lib}_LIB} STREQUAL "${lib}_LIB-NOTFOUND")
                #message("${target}: find_library success for dependency ${lib}")
                continue()
            endif()
            string(REGEX MATCH "(.+)-(.+)" HAS_SUFFIX ${lib})
            if(HAS_SUFFIX)
                set(PACKAGE_NAME ${CMAKE_MATCH_1})
                set(SUFFIX_NAME ${CMAKE_MATCH_2})
                find_package(${PACKAGE_NAME} QUIET)
                if(${PACKAGE_NAME}_FOUND)
                    if(NOT TARGET ${lib})
                        message(WARNING "CSToolkit: ${target}: find_package success for dependency ${lib} but target is missing")
                    endif()
                    continue()
                elseif(NOT ${PACKAGE_NAME}_NOT_FOUND_MESSAGE)
                    message(WARNING "CSToolkit: ${target}: find_package failed for dependency ${lib}\n"
                                    "${${PACKAGE_NAME}_NOT_FOUND_MESSAGE}")
                    continue()
                endif()
            endif()

            find_package(${lib} QUIET)

            if(${lib}_FOUND)
                if(NOT TARGET ${lib})
                    message(WARNING "CSToolkit: ${target}: find_package success for dependency ${lib} but target is missing")
                endif()
                continue()
            elseif(NOT ${lib}_NOT_FOUND_MESSAGE)
                message(WARNING "CSToolkit: ${target}: find_package failed for dependency ${lib}\n"
                                "${${lib}_NOT_FOUND_MESSAGE}")
                continue()
            endif()

            message(WARNING "CSToolkit: ${target}: unknown dependency ${lib}")
        endif()
    endforeach()

    # Find package des modules de Qt
    if (QT5_MODULES)
        #message("find_package(Qt5 ${Qt5_VERSION} COMPONENTS ${QT5_MODULES} REQUIRED)")
        find_package(Qt5 REQUIRED COMPONENTS ${QT5_MODULES})
    endif()

    target_link_libraries(${target}
        PRIVATE ${link_libraries_post_PRIVATE} PUBLIC ${link_libraries_post_PUBLIC} INTERFACE ${link_libraries_post_INTERFACE}
    )
    #get_target_property(interfaces_deps ${target} INTERFACE_LINK_LIBRARIES)
    #message("${target} needs ${interfaces_deps}")
    
endfunction()

# qt5_wrap_ui(outfiles inputfile ... )
# partially copied from Qt5WidgetsMacros.cmake

