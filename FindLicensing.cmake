# TODO: Use unique name when available
set_os_variable(ApiLicensing "ApiLicensing" "ApiLicensingLinux")

find_package_libraries(
  "Licensing"
  "${ApiLicensing}"
  "ILicensing.h"
  "${DEP_TARGET_NAME}/debug"
  "${DEP_TARGET_NAME}/release")
