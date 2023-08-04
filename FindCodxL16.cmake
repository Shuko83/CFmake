set_os_variable(CodxL16ED${L16_EDITION}Pattern "CodxL16ED${L16_EDITION}" "CodxL16")
set_os_variable(CodxL16ED${L16_EDITION}Elements "CodxL16ED${L16_EDITION}Elements;CodxL16ED${L16_EDITION}Elements1;CodxL16ED${L16_EDITION}Elements2;CodxL16ED${L16_EDITION}Elements3;CodxL16ED${L16_EDITION}Elements4;CodxL16ED${L16_EDITION}Elements5" "CodxL16Elements")
set_os_variable(CodxL16ED${L16_EDITION}Messages "CodxL16ED${L16_EDITION}Messages" "CodxL16Messages")

find_codx_package_libraries(
  "CodxL16"
  "${CodxL16ED${L16_EDITION}Pattern}"
  "${CodxL16ED${L16_EDITION}Elements};${CodxL16ED${L16_EDITION}Messages}"
  "L16ED${L16_EDITION}/common/L16Common.h"
  "${DEP_TARGET_NAME}/debug"
  "${DEP_TARGET_NAME}/release")

if(NOT WIN32)
  add_library(CodxL16::Elements1 ALIAS CodxL16::Elements)
  add_library(CodxL16::Elements2 ALIAS CodxL16::Elements)
  add_library(CodxL16::Elements3 ALIAS CodxL16::Elements)
  add_library(CodxL16::Elements4 ALIAS CodxL16::Elements)
  add_library(CodxL16::Elements5 ALIAS CodxL16::Elements)
endif(NOT WIN32)
