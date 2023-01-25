find_package_libraries(
  "ReadOLM"
  "ReadOLM_FILTER_SEGMENT;ReadOLM_JRE_SEGMENT;ReadOLM_L16_SEGMENT"
  "ReadOLM_Constantes.h"
  "${TARGET_NAME}-QT${Qt5_VERSION}/debug"
  "${TARGET_NAME}-QT${Qt5_VERSION}/release")
