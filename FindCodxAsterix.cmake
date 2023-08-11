set_os_variable(CodxAsterixED${ASTERIX_EDITION}Pattern "CodxAsterixED${ASTERIX_EDITION}" "CodxAsterix")
set_os_variable(CodxAsterixED${ASTERIX_EDITION}Elements "CodxAsterixED${ASTERIX_EDITION}Elements;CodxAsterixED${ASTERIX_EDITION}Elements1;CodxAsterixED${ASTERIX_EDITION}Elements2;CodxAsterixED${ASTERIX_EDITION}Elements3" "CodxAsterixElements")
set_os_variable(CodxAsterixED${ASTERIX_EDITION}Messages "CodxAsterixED${ASTERIX_EDITION}Messages" "CodxAsterixMessages")

find_codx_package_libraries(
  "CodxAsterix"
  "${CodxAsterixED${ASTERIX_EDITION}Pattern}"
  "${CodxAsterixED${ASTERIX_EDITION}Elements};${CodxAsterixED${ASTERIX_EDITION}Messages}"
  "ASTERIXED${ASTERIX_EDITION}/common/ASTERIXCommon.h"
  "${DEP_TARGET_NAME}/debug"
  "${DEP_TARGET_NAME}/release")

if(NOT WIN32)
  add_library(CodxAsterix::Elements1 ALIAS CodxAsterix::Elements)
  add_library(CodxAsterix::Elements2 ALIAS CodxAsterix::Elements)
  add_library(CodxAsterix::Elements3 ALIAS CodxAsterix::Elements)
endif(NOT WIN32)
