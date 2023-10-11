set_os_variable(CodxAsterixED${ASTERIX_EDITION}Pattern "CodxAsterixED${ASTERIX_EDITION}" "CodxASTERIX")
set_os_variable(CodxAsterixED${ASTERIX_EDITION}Elements "CodxAsterixED${ASTERIX_EDITION}Elements;CodxAsterixED${ASTERIX_EDITION}Elements1;CodxAsterixED${ASTERIX_EDITION}Elements2;CodxAsterixED${ASTERIX_EDITION}Elements3" "CodxASTERIXElements")
set_os_variable(CodxAsterixED${ASTERIX_EDITION}Messages "CodxAsterixED${ASTERIX_EDITION}Messages" "CodxASTERIXMessages")

find_codx_package_libraries(
  "CodxAsterix"
  "${CodxAsterixED${ASTERIX_EDITION}Pattern}"
  "${CodxAsterixED${ASTERIX_EDITION}Elements};${CodxAsterixED${ASTERIX_EDITION}Messages}"
  "ASTERIXED${ASTERIX_EDITION}/common/ASTERIXCommon.h"
  "${DEP_TARGET_NAME}/debug"
  "${DEP_TARGET_NAME}/release")

if(NOT WIN32)
  add_library(CodxAsterix::CodxASTERIXElements ALIAS CodxAsterix::Elements)
  add_library(CodxAsterix::CodxASTERIXMessages ALIAS CodxAsterix::Messages)
  add_library(CodxAsterix::Elements1 ALIAS CodxAsterix::Elements)
  add_library(CodxAsterix::Elements2 ALIAS CodxAsterix::Elements)
  add_library(CodxAsterix::Elements3 ALIAS CodxAsterix::Elements)
endif(NOT WIN32)
