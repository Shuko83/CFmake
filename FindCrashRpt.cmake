find_package_unique_libraries(
  "CrashRpt"
  "CrashRpt"
  "CrashRpt.h"
  "${DEP_TARGET_NAME}")

find_program(CrashSaver_EXECUTABLE
  NAMES CrashSaver.exe
  PATHS ${CrashRpt_ROOT}/${DEP_TARGET_NAME})
