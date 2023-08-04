# TODO: Use ${DEP_TARGET_NAME} when underscore has been replaced in path

# TODO: Restore unique version on next DLPX-HI release
if(WIN32)
  find_package_libraries(
    "LogX"
    "LogX"
    "LogX_1.3.4_win-msvc2015_x64_Debug/Delivery/include/LogX_Export.h"
    "LogX_1.3.4_win-msvc2015_x64_Debug/Delivery/lib/Debug"
    "LogX_1.3.4_win-msvc2015_x64_Release/Delivery/lib/Release")
else(WIN32)
  find_package_libraries(
    "LogX"
    "LogX"
    "LogX/LogX_Export.hpp"
    "linux-gcc10.2.1-x64/debug"
    "linux-gcc10.2.1-x64/release")
endif(WIN32)
