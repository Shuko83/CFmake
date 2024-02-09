find_package_libraries(
  "QstBase"
  "QsCore;QsGui"
  "QsGlobal.h"
  "${DEP_TARGET_NAME}-QT${Qt5_VERSION}/debug"
  "${DEP_TARGET_NAME}-QT${Qt5_VERSION}/release")
