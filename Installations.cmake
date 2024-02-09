################################################################################
# Installation definition
################################################################################

function(define_installation)

  # Check sub-project

  if(NOT PROJECT_NAME STREQUAL CMAKE_PROJECT_NAME)
    return()
  endif(NOT PROJECT_NAME STREQUAL CMAKE_PROJECT_NAME)

  # Install headers

  #install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include DESTINATION ".")

  # Install targets

  install(EXPORT ${CMAKE_PROJECT_NAME}Targets
    FILE ${CMAKE_PROJECT_NAME}Targets.cmake
    DESTINATION lib/${TARGET_NAME}/cmake)

  # Install configurations

  include(CMakePackageConfigHelpers)

  set(CMAKE_PROJECT_NAME_LIBRARIES ${${CMAKE_PROJECT_NAME}_LIBRARIES})

  configure_package_config_file(
    ${CMAKE_MODULE_TEMPLATES_PATH}/ProjectConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}Config.cmake
    INSTALL_DESTINATION lib/${TARGET_NAME}/cmake
    NO_SET_AND_CHECK_MACRO
    NO_CHECK_REQUIRED_COMPONENTS_MACRO)

  write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}ConfigVersion.cmake
    VERSION ${CMAKE_PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion)

  install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}Config.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}ConfigVersion.cmake
    DESTINATION lib/${TARGET_NAME}/cmake)

  export(EXPORT ${CMAKE_PROJECT_NAME}Targets
    FILE ${CMAKE_PROJECT_NAME}Targets.cmake)

  # Install license

  install(FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.md
    ${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.txt
    DESTINATION "." OPTIONAL)

endfunction(define_installation)

################################################################################
# Runtime dependencies installation
################################################################################

function(install_runtime_dependencies COMPONENT_NAME COMPONENT_INSTALLNAME COMPONENT_INSTALLDIR)

  foreach(RUNTIME_DEP IN LISTS ${COMPONENT_NAME}_RUNTIME_DEPS)
    if(TARGET ${RUNTIME_DEP})
      get_target_property(RUNTIME_DEP_CONFIGS ${RUNTIME_DEP} IMPORTED_CONFIGURATIONS)
      foreach(RUNTIME_DEP_CONFIG IN LISTS RUNTIME_DEP_CONFIGS)
        get_target_property(RUNTIME_DEP_LOCATION ${RUNTIME_DEP} IMPORTED_LOCATION_${RUNTIME_DEP_CONFIG})
        if(EXISTS ${RUNTIME_DEP_LOCATION})
          relocate_runtime_dependency(${RUNTIME_DEP} ${RUNTIME_DEP_CONFIG} ${RUNTIME_DEP_LOCATION})
          get_filename_component(RUNTIME_DEP_REALPATH ${RUNTIME_DEP_LOCATION} REALPATH)
          get_filename_component(RUNTIME_DEP_NAME ${RUNTIME_DEP_LOCATION} NAME)
          install(FILES ${RUNTIME_DEP_REALPATH}
            DESTINATION ${COMPONENT_INSTALLDIR}
            RENAME ${RUNTIME_DEP_NAME}
            CONFIGURATIONS ${RUNTIME_DEP_CONFIG}
            COMPONENT ${COMPONENT_INSTALLNAME})
        endif(EXISTS ${RUNTIME_DEP_LOCATION})
      endforeach(RUNTIME_DEP_CONFIG IN LISTS RUNTIME_DEP_CONFIGS)
    endif(TARGET ${RUNTIME_DEP})
  endforeach(RUNTIME_DEP IN LISTS ${COMPONENT_NAME}_RUNTIME_DEPS)

endfunction(install_runtime_dependencies)
