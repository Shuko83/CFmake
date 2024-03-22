include(CMakePackageConfigHelpers)

function(cstoolkit_post_configure)
    #message(STATUS "CSTOOLKIT_POST_CONFIGURE")

    cstoolkit_get_all_targets(ALL_TARGETS)
  
    configure_package_config_file(
        ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/../templates/PackageConfig.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
        INSTALL_DESTINATION ${CMAKE_INSTALL_PREFIX}
    )

    write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMinorVersion
    )

    install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_PREFIX}
    )

    return()

    foreach(target ${ALL_TARGETS})
        get_target_property(DEPENDENCIES ${target} DEPENDENCIES)
        list(APPEND ALL_DEPENDENCIES ${DEPENDENCIES})
    endforeach()

    list(REMOVE_DUPLICATES ALL_DEPENDENCIES)

    set(QT5_MODULES)

    foreach(lib ${ALL_DEPENDENCIES})
        string(REGEX MATCH "(.+)::(.+)" IS_PACKAGE ${lib})    
        if(IS_PACKAGE)
            set(PACKAGE_NAME ${CMAKE_MATCH_1})
            set(COMPONENT_NAME ${CMAKE_MATCH_2})

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
                    find_package(${PACKAGE_NAME} COMPONENTS ${COMPONENT_NAME})
                endif()
            endif()
        else()
        #message(${lib} " no namespace")
        endif()
    endforeach()

    # Find package des modules de Qt
    if (QT5_MODULES)
        #message("find_package(Qt5 ${Qt5_VERSION} COMPONENTS ${QT5_MODULES} REQUIRED)")
        find_package(Qt5 ${Qt5_VERSION} COMPONENTS ${QT5_MODULES} REQUIRED)
    endif()    
endfunction()

function(cstoolkit_get_all_targets var)
    set(targets)
    cstoolkit_get_all_targets_recursive(targets ${CMAKE_CURRENT_SOURCE_DIR})
    set(${var} ${targets} PARENT_SCOPE)
endfunction()

macro(cstoolkit_get_all_targets_recursive targets dir)
    get_property(subdirectories DIRECTORY ${dir} PROPERTY SUBDIRECTORIES)
    foreach(subdir ${subdirectories})
        cstoolkit_get_all_targets_recursive(${targets} ${subdir})
    endforeach()
 
    get_property(current_targets DIRECTORY ${dir} PROPERTY BUILDSYSTEM_TARGETS)
    list(APPEND ${targets} ${current_targets})
endmacro()
