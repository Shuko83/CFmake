function(add_target TARGET_TYPE)
  
  # Parse arguments

  set(TARGET_OPTIONS RECURSIVE NOINSTALL)
  set(TARGET_UNIQUE NAME PATH INSTALLNAME INSTALLDIR)
  set(TARGET_MULTIPLE PUBLIC_BUILD_DEPS PRIVATE_BUILD_DEPS INTERFACE_BUILD_DEPS RUNTIME_DEPS)
  cmake_parse_arguments(TARGET "${TARGET_OPTIONS}" "${TARGET_UNIQUE}" "${TARGET_MULTIPLE}" ${ARGN})

  #message("\nadd_target(${TARGET_NAME})")

  if(DEFINED TARGET_UNPARSED_ARGUMENTS)
    message(SEND_ERROR "Unkown arguments : ${TARGET_UNPARSED_ARGUMENTS}")
  endif()

  if(NOT DEFINED TARGET_NAME OR TARGET_NAME STREQUAL "")
    message(SEND_ERROR "No NAME defined for target")
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
    message(SEND_ERROR "Incorrect TARGET_TYPE \"${TARGET_TYPE}\" for target \"${TARGET_NAME}\". Must be either EXECUTABLE, SHARED, STATIC or INTERFACE.")
    return()
  endif()

  # Paths

  string(REPLACE "/_project" "" TARGET_PUBLIC_HEADERS_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
  string(REPLACE "/_project" "" TARGET_PRIVATE_HEADERS_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
  string(REPLACE "/_project" "" TARGET_SOURCES_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
  string(REPLACE "/_project" "" TARGET_UI_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
  string(REPLACE "/_project" "" TARGET_RESOURCES_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
  string(REPLACE "/_project" "" TARGET_TRANSLATION_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

  #set(TARGET_PUBLIC_HEADERS_DIR ${TARGET_PUBLIC_HEADERS_DIR}/include)

  set(TARGET_INSTALL_LIB_DIR ${TARGET_NAME}/lib/$<LOWER_CASE:$<CONFIG>>)
  set(TARGET_INSTALL_BIN_DIR ${TARGET_NAME}/bin/$<LOWER_CASE:$<CONFIG>>)
  set(TARGET_INSTALL_INCLUDE_DIR ${TARGET_NAME}/include)
  set(TARGET_INSTALL_CMAKE_DIR ${TARGET_NAME}/cmake)

  # Sources
  if(TARGET_RECURSIVE)
    file(GLOB_RECURSE TARGET_PUBLIC_HEADERS_FILES ${TARGET_PUBLIC_HEADERS_DIR}/*.h ${TARGET_PUBLIC_HEADERS_DIR}/*.hpp)
    file(GLOB_RECURSE TARGET_PRIVATE_HEADERS_FILES ${TARGET_PRIVATE_HEADERS_DIR}/*.h ${TARGET_PRIVATE_HEADERS_DIR}/*.hpp)
    file(GLOB_RECURSE TARGET_SOURCES_FILES ${TARGET_SOURCES_DIR}/*.c ${TARGET_SOURCES_DIR}/*.cpp)
    file(GLOB_RECURSE TARGET_UI_FILES ${TARGET_UI_DIR}/*.ui)
    file(GLOB_RECURSE TARGET_RESOURCES_FILES ${TARGET_RESOURCES_DIR}/*.qrc)
    file(GLOB_RECURSE TARGET_TRANSLATION_FILES ${TARGET_TRANSLATION_DIR}/*.ts)
  else()
    file(GLOB TARGET_PUBLIC_HEADERS_FILES ${TARGET_PUBLIC_HEADERS_DIR}/*.h ${TARGET_PUBLIC_HEADERS_DIR}/*.hpp)
    file(GLOB TARGET_PRIVATE_HEADERS_FILES ${TARGET_PRIVATE_HEADERS_DIR}/*.h ${TARGET_PRIVATE_HEADERS_DIR}/*.hpp)
    file(GLOB TARGET_SOURCES_FILES ${TARGET_SOURCES_DIR}/*.c ${TARGET_SOURCES_DIR}/*.cpp)
    file(GLOB TARGET_UI_FILES ${TARGET_UI_DIR}/*.ui)
    file(GLOB TARGET_RESOURCES_FILES ${TARGET_RESOURCES_DIR}/*.qrc)
    file(GLOB TARGET_TRANSLATION_FILES ${TARGET_TRANSLATION_DIR}/*.ts)
  endif()

  # Library
  
  if(TARGET_SHARED OR TARGET_STATIC OR TARGET_INTERFACE)
    add_library(${TARGET_NAME} ${TARGET_TYPE} ${TARGET_SOURCES_FILES} ${TARGET_UI_FILES} ${TARGET_RESOURCES_FILES})
    add_library(${PROJECT_NAME}::${TARGET_NAME} ALIAS ${TARGET_NAME})
  endif()

  # Executable

  if(TARGET_EXECUTABLE)
    add_executable(${TARGET_NAME} ${TARGET_SOURCES_FILES} ${TARGET_UI_FILES} ${TARGET_RESOURCES_FILES})
  endif()

  # Dossier des projets

  source_group(TREE ${TARGET_PRIVATE_HEADERS_DIR}  PREFIX "Header Files (Private)" FILES ${TARGET_PRIVATE_HEADERS_FILES})
  source_group(TREE ${TARGET_PUBLIC_HEADERS_DIR}  PREFIX "Header Files (Public)" FILES ${TARGET_PUBLIC_HEADERS_FILES})
  source_group(TREE ${TARGET_SOURCES_DIR} PREFIX "Source Files" FILES ${TARGET_SOURCES_FILES})
  source_group(TREE ${TARGET_UI_DIR} PREFIX "Form Files" FILES ${TARGET_UI_FILES})
  source_group(TREE ${TARGET_RESOURCES_DIR} PREFIX "Resource Files" FILES ${TARGET_RESOURCES_FILES})
  source_group(TREE ${TARGET_TRANSLATION_DIR} PREFIX "Translation Files" FILES ${TARGET_TRANSLATION_FILES})

  string(REPLACE "${PROJECT_SOURCE_DIR}" "" FOLDER "${CMAKE_CURRENT_SOURCE_DIR}")
  string(REPLACE "/${TARGET_NAME}" "" FOLDER "${FOLDER}")
  string(REPLACE "/Sources" "" FOLDER "${FOLDER}")
  string(REPLACE "/src" "" FOLDER "${FOLDER}")
  string(REPLACE "/test" "" FOLDER "${FOLDER}")
  string(REPLACE "/_project" "" FOLDER "${FOLDER}")
  set_target_properties(${TARGET_NAME} PROPERTIES FOLDER "${FOLDER}")

  set_target_properties(${TARGET_NAME} PROPERTIES AUTOGEN_BUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated")

  # Definitions

  if(TARGET_SHARED OR TARGET_STATIC)
    string(TOUPPER ${TARGET_NAME} TARGET_EXPORTS)
    string(REPLACE "." "_" TARGET_EXPORTS ${TARGET_EXPORTS})
    string(APPEND TARGET_EXPORTS "_LIB")
    add_compile_definitions(${TARGET_EXPORTS})
  endif()

  # Dependencies

  list(APPEND DEPENDENCIES ${TARGET_PUBLIC_BUILD_DEPS} ${TARGET_PRIVATE_BUILD_DEPS} ${TARGET_INTERFACE_BUILD_DEPS} ${TARGET_RUNTIME_DEPS})
  set_target_properties(${TARGET_NAME} PROPERTIES "DEPENDENCIES" "${DEPENDENCIES}")

  # Includes

  target_sources(${TARGET_NAME}
    PUBLIC FILE_SET HEADERS BASE_DIRS ${TARGET_PUBLIC_HEADERS_DIR} FILES ${TARGET_PUBLIC_HEADERS_FILES}
    PRIVATE FILE_SET "interditdemettreunemajuscule" TYPE HEADERS BASE_DIRS ${TARGET_PRIVATE_HEADERS_DIR} FILES ${TARGET_PRIVATE_HEADERS_FILES}
  )

  # Links

  target_link_libraries_custom(${TARGET_NAME}
    PUBLIC ${TARGET_PUBLIC_BUILD_DEPS}
    PRIVATE ${TARGET_PRIVATE_BUILD_DEPS}
    INTERFACE ${TARGET_INTERFACE_BUILD_DEPS})

  # Installation

  if(NOT TARGET_NOINSTALL)
    install(TARGETS ${TARGET_NAME} EXPORT ${PROJECT_NAME}_${TARGET_NAME}Targets DESTINATION ${TARGET_NAME}
      ARCHIVE DESTINATION ${TARGET_INSTALL_LIB_DIR}
      RUNTIME DESTINATION ${TARGET_INSTALL_BIN_DIR}
      FILE_SET HEADERS DESTINATION ${TARGET_INSTALL_INCLUDE_DIR}
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
    # install(FILES $<COMPILE_PDB_FILE:${TARGET_NAME}> DESTINATION ${TARGET_INSTALL_BIN_DIR} OPTIONAL)
    #endif()

    # Config
    install(EXPORT ${PROJECT_NAME}_${TARGET_NAME}Targets
      NAMESPACE ${PROJECT_NAME}::
      DESTINATION ${TARGET_INSTALL_CMAKE_DIR}
    )
    generate_target_config()

  endif()

  # Generation des fichiers infos

  if(TARGET_SHARED OR TARGET_EXECUTABLE)
    generate_target_info(TARGET ${TARGET_NAME} COPYRIGHT CSGroup COMPANY CSGroup PRODUCT ${TARGET_NAME})
  endif()

  # Debug Messages

  #message(STATUS "==================================================")
  #message(STATUS "Component ${TARGET_NAME} is ${TARGET_TYPE}")
  #message(STATUS "  - Dependencies: ${ALL_DEPS}")
  #message(STATUS "  - Public headers path: ${TARGET_PUBLIC_HEADERS_DIR}")
  #message(STATUS "  - Private headers path: ${TARGET_PRIVATE_HEADERS_DIR}")
  #message(STATUS "  - Sources path: ${TARGET_SOURCES_DIR}")
  #message(STATUS "  - Recursive: ${TARGET_RECURSIVE}")
  #message(DEBUG  "  - Public headers files: ${TARGET_PUBLIC_HEADERS_FILES}")
  #message(DEBUG  "  - Private headers files: ${TARGET_PRIVATE_HEADERS_FILES}")
  #message(DEBUG  "  - Sources files: ${TARGET_SOURCES_FILES}")
  #message(DEBUG  "  - Public build dependencies: ${TARGET_PUBLIC_BUILD_DEPS}")
  #message(DEBUG  "  - Private build dependencies: ${TARGET_PRIVATE_BUILD_DEPS}")
  #message(DEBUG  "  - Interface build dependencies: ${TARGET_INTERFACE_BUILD_DEPS}")
  #message(DEBUG  "  - Runtime dependencies: ${TARGET_RUNTIME_DEPS}")
  #if(NOT TARGET_NOINSTALL)
  #  message(STATUS "  - Installation: Default")
  #endif()

  #find_package(Qt5 ${Qt5_VERSION} COMPONENTS "Core")
endfunction()

macro (generate_target_config)

  list(APPEND INTERFACE_DEPENDENCIES ${TARGET_PUBLIC_BUILD_DEPS} ${TARGET_INTERFACE_BUILD_DEPS})
  foreach(dep ${INTERFACE_DEPENDENCIES})
    string(REGEX MATCH "(.+)::(.+)" IS_PACKAGE ${dep})    
    if(IS_PACKAGE)
        set(PACKAGE_NAME ${CMAKE_MATCH_1})
        set(COMPONENT_NAME ${CMAKE_MATCH_2})
        set(FIND_DEPENDENCY_COMMANDS "${FIND_DEPENDENCY_COMMANDS}find_dependency(${PACKAGE_NAME} COMPONENTS ${COMPONENT_NAME})\n")
      endif()
  endforeach()
  message("Command find_dependency a ajouter pour ${TARGET_NAME} : ${FIND_DEPENDENCY_COMMANDS}")
  

  include(CMakePackageConfigHelpers)

  # Target Config File
  configure_package_config_file(
    ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/templates/TargetConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}_${TARGET_NAME}Config.cmake
    INSTALL_DESTINATION ${TARGET_INSTALL_CMAKE_DIR}
  )

  install(FILES 
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}_${TARGET_NAME}Config.cmake
    DESTINATION ${TARGET_INSTALL_CMAKE_DIR}
  )
endmacro()


function(target_link_libraries_custom target)
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
    string(REGEX MATCH "(.+)::(.+)" IS_PACKAGE ${lib})    
    if(IS_PACKAGE)
        set(PACKAGE_NAME ${CMAKE_MATCH_1})
        set(COMPONENT_NAME ${CMAKE_MATCH_2})
        #message("Namespace: ${PACKAGE_NAME}")
        #message("LibName: ${COMPONENT_NAME}")
        if(TARGET ${lib})
            #message(${lib} " is a target")
        else()
            #message(${lib} " target not found")
            
            # Vérifier si le motif "Qt5::" existe dans l'élément actuel     
            string(REGEX MATCH "^Qt5::(.|..|...|....|.....|......|.......|.+[^P]......|.+P[^r].....|.+Pr[^i]....|.+Pri[^v]...|.+Priv[^a]..|.+Priva[^t].|.+Privat[^e])(Private$|$)" IS_QT5 ${lib})
            # Si le motif est trouvé     
            if(IS_QT5)         
            # Récupérer ce qui se trouve après "Qt5::"
                list(APPEND QT5_MODULES ${CMAKE_MATCH_1})
            else()
                find_package(${PACKAGE_NAME} QUIET COMPONENTS ${COMPONENT_NAME})

                if(NOT ${PACKAGE_NAME}_FOUND)
                  message(WARNING "${target} : find_package failed for dependency ${lib}\n"
                                  "${${PACKAGE_NAME}_NOT_FOUND_MESSAGE}")
                endif()
                
                #message("find_package(${PACKAGE_NAME} REQUIRED COMPONENTS ${COMPONENT_NAME})")
                #if(${COMPONENT_NAME} STREQUAL QsCore)
                #  message("Target ${target} needs QsCore")
                #  get_target_property(interfaces_deps ${lib} INTERFACE_LINK_LIBRARIES)
                #  message("QsCore needs ${interfaces_deps}")
                #endif()
            endif()
        endif()
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
  get_target_property(interfaces_deps ${target} INTERFACE_LINK_LIBRARIES)
  message("${target} needs ${interfaces_deps}")
  
endfunction()
