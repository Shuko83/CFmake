# TODO: Use unique name when available
set_os_variable(LinkManagerFramework "LinkManagerFramework" "LnkMgrFwk")

find_package_libraries(
  "LnkMgrFwk"
  "${LinkManagerFramework};SpyGrpcUtils"
  "LnkMgrFwk_Constantes.h"
  "${DEP_TARGET_NAME}/debug"
  "${DEP_TARGET_NAME}/release")

if(NOT WIN32)
  add_library(LnkMgrFwk::LinkManagerFramework ALIAS LnkMgrFwk::LnkMgrFwk)
endif(NOT WIN32)
