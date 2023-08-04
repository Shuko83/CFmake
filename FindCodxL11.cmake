set_os_variable(CodxL11ED${L11_EDITION}Pattern "CodxL11ED${L11_EDITION}" "CodxL11")
set_os_variable(CodxL11ED${L11_EDITION}Elements "CodxL11ED${L11_EDITION}Elements" "CodxL11Elements")
set_os_variable(CodxL11ED${L11_EDITION}Messages "CodxL11ED${L11_EDITION}Messages" "CodxL11Messages")

find_codx_package_libraries(
  "CodxL11"
  "${CodxL11ED${L11_EDITION}Pattern}"
  "${CodxL11ED${L11_EDITION}Elements};${CodxL11ED${L11_EDITION}Messages}"
  "L11ED${L11_EDITION}/common/L11Common.h"
  "${DEP_TARGET_NAME}/debug"
  "${DEP_TARGET_NAME}/release")
