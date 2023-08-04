set_os_variable(CodxL22ED${L22_EDITION}Pattern "CodxL22ED${L22_EDITION}" "CodxL22")
set_os_variable(CodxL22ED${L22_EDITION}Elements "CodxL22ED${L22_EDITION}Elements;CodxL22ED${L22_EDITION}Elements1;CodxL22ED${L22_EDITION}Elements2;CodxL22ED${L22_EDITION}Elements3" "CodxL22Elements")
set_os_variable(CodxL22ED${L22_EDITION}Messages "CodxL22ED${L22_EDITION}Messages" "CodxL22Messages")

find_codx_package_libraries(
  "CodxL22"
  "${CodxL22ED${L22_EDITION}Pattern}"
  "${CodxL22ED${L22_EDITION}Elements};${CodxL22ED${L22_EDITION}Messages}"
  "L22ED${L22_EDITION}/common/L22Common.h"
  "${DEP_TARGET_NAME}/debug"
  "${DEP_TARGET_NAME}/release")

if(NOT WIN32)
  add_library(CodxL22::Elements1 ALIAS CodxL22::Elements)
  add_library(CodxL22::Elements2 ALIAS CodxL22::Elements)
  add_library(CodxL22::Elements3 ALIAS CodxL22::Elements)
endif(NOT WIN32)
