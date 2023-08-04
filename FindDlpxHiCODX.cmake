set_os_variable(CodxLOCED8Pattern "CodxLOCED8" "CodxLOC")
set_os_variable(CodxLOCED8Elements "CodxLOCED8Elements;CodxLOCED8Elements1;CodxLOCED8Elements2;CodxLOCED8Elements3;CodxLOCED8Elements4;CodxLOCED8Elements5" "CodxLOCElements")

find_codx_package_libraries(
  "DlpxHiCODX"
  "${CodxLOCED8Pattern}"
  "${CodxLOCED8Elements}"
  "LOCED8/common/LOCCommon.h"
  "${DEP_TARGET_NAME}/debug"
  "${DEP_TARGET_NAME}/release")

if(NOT WIN32)
  add_library(DlpxHiCODX::Elements1 ALIAS DlpxHiCODX::Elements)
  add_library(DlpxHiCODX::Elements2 ALIAS DlpxHiCODX::Elements)
  add_library(DlpxHiCODX::Elements3 ALIAS DlpxHiCODX::Elements)
  add_library(DlpxHiCODX::Elements4 ALIAS DlpxHiCODX::Elements)
  add_library(DlpxHiCODX::Elements5 ALIAS DlpxHiCODX::Elements)
endif(NOT WIN32)
