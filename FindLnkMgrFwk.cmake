# TODO: Use unique name when available
set_os_variable(LinkManagerFramework "LinkManagerFramework" "LinkManagerFrameworkProto;LnkMgrFwk")

find_package_libraries(
  "LnkMgrFwk"
  "${LinkManagerFramework};SpyGrpcUtils"
  "LnkMgrFwk_Constantes.h"
  "${DEP_TARGET_NAME}/debug"
  "${DEP_TARGET_NAME}/release")

if(NOT WIN32)
  add_library(LnkMgrFwk::LinkManagerFramework ALIAS LnkMgrFwk::LnkMgrFwk)
endif(NOT WIN32)

################################################################################
# LinkManagerFramework runtime dependencies copy
################################################################################

function(copy_lnkmgrfwk_dependencies COMPONENT_NAME)

  if(WIN32)

    foreach(LIB_NAME abseil_dll cares libcrypto-1_1-x64 libssl-1_1-x64 re2)
      add_custom_command(TARGET ${COMPONENT_NAME} POST_BUILD COMMAND cmake -E copy_if_different
        ${LnkMgrFwk_DIR}/${TARGET_NAME}/$<IF:$<CONFIG:DEBUG>,debug,release>/${LIB_NAME}.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    endforeach(LIB_NAME abseil_dll cares libcrypto-1_1-x64 libssl-1_1-x64 re2)

    set(LIB_NAME libprotobuf)
    add_custom_command(TARGET ${COMPONENT_NAME} POST_BUILD COMMAND cmake -E copy_if_different
      ${LnkMgrFwk_DIR}/${TARGET_NAME}/$<IF:$<CONFIG:DEBUG>,debug,release>/${LIB_NAME}$<IF:$<CONFIG:DEBUG>,d,>.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

    set(LIB_NAME_P zlib)
    set(LIB_NAME_S 1)
    add_custom_command(TARGET ${COMPONENT_NAME} POST_BUILD COMMAND cmake -E copy_if_different
      ${LnkMgrFwk_DIR}/${TARGET_NAME}/$<IF:$<CONFIG:DEBUG>,debug,release>/${LIB_NAME_P}$<IF:$<CONFIG:DEBUG>,d,>${LIB_NAME_S}.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

  endif(WIN32)

endfunction(copy_lnkmgrfwk_dependencies COMPONENT_NAME)

################################################################################
# LinkManagerFramework runtime dependencies installation
################################################################################

function(install_lnkmgrfwk_dependencies COMPONENT_NAME INSTALL_DIR)

  if(WIN32)

    foreach(LIB_NAME abseil_dll cares libcrypto-1_1-x64 libssl-1_1-x64 re2)
      install(FILES ${LnkMgrFwk_DIR}/${TARGET_NAME}/$<IF:$<CONFIG:DEBUG>,debug,release>/${LIB_NAME}.dll
        DESTINATION ${INSTALL_DIR} COMPONENT ${COMPONENT_NAME})
    endforeach(LIB_NAME abseil_dll cares libcrypto-1_1-x64 libssl-1_1-x64 re2)

    set(LIB_NAME libprotobuf)
    install(FILES ${LnkMgrFwk_DIR}/${TARGET_NAME}/$<IF:$<CONFIG:DEBUG>,debug,release>/${LIB_NAME}$<IF:$<CONFIG:DEBUG>,d,>.dll
      DESTINATION ${INSTALL_DIR} COMPONENT ${COMPONENT_NAME})

    set(LIB_NAME_P zlib)
    set(LIB_NAME_S 1)
    install(FILES ${LnkMgrFwk_DIR}/${TARGET_NAME}/$<IF:$<CONFIG:DEBUG>,debug,release>/${LIB_NAME_P}$<IF:$<CONFIG:DEBUG>,d,>${LIB_NAME_S}.dll
      DESTINATION ${INSTALL_DIR} COMPONENT ${COMPONENT_NAME})

  endif(WIN32)

endfunction(install_lnkmgrfwk_dependencies COMPONENT_NAME INSTALL_DIR)
