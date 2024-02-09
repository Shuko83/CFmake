find_package_libraries(
  "QstModules"
  "QsException;QsLog"
  "QsLog.h"
  "${DEP_TARGET_NAME}-QT${Qt5_VERSION}/debug"
  "${DEP_TARGET_NAME}-QT${Qt5_VERSION}/release")
