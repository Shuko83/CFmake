find_package_unique_libraries(
  "CrashRpt"
  "crashrpt"
  "CrashRpt.h"
  "${TARGET_NAME}")

find_program(CrashSaver_EXECUTABLE
  NAMES CrashSaver.exe
  PATHS ${CrashRpt_ROOT}/${TARGET_NAME})
