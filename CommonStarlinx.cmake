
include(Common)
include(generate_target_info)

set(CMAKE_PROJECT_INCLUDE "${CMAKE_SOURCE_DIR}/cmake/Version.cmake")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# # Allow to call CommonStarlinx before project()
# set(CMAKE_PROJECT_INCLUDE "${CMAKE_SOURCE_DIR}/cmake/CommonStarlinx.cmake")

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(ARTIFACTORY_URL "http://artifactory.divst:8081/artifactory" CACHE STRING "Artifactory base URL")

set(Qt5_VERSION "5.9.6" CACHE INTERNAL "Qt5 version")
set(Qt5_ARCH "msvc2015_64" CACHE INTERNAL "Qt5 architecture")

add_compile_definitions(UNICODE)

function(get_dependency_starlinx NAME)

  set(DD_ARCHS "x64")
  set(DD_TARGETS "win-msvc2015")

  # APILicensing
  if(NAME STREQUAL "APILicensing")
    define_dependency(
      NAME "Licensing"
      OUTPUT_PATH "${CMAKE_PREFIX_PATH}/Libraries"
      REMOTE_PATH "${ARTIFACTORY_URL}/release/Libraries.Licensing/5.1.0/%TARGET%/%ARCH%/Libraries.Licensing-5.1.0-%TARGET%-%ARCH%.zip"
      CREATEDIR)
  endif(NAME STREQUAL "APILicensing")

  # Cryptopp
  if(NAME STREQUAL "Cryptopp")
  define_dependency(
    NAME "Cryptopp"
    OUTPUT_PATH "${CMAKE_PREFIX_PATH}/3rdParty"
    REMOTE_PATH "${ARTIFACTORY_URL}/thirdParty/Cryptopp/5.6.5.1/%TARGET%/%ARCH%/Cryptopp_5.6.5.1_%TARGET%-%ARCH%.zip"
    CREATEDIR)
  endif(NAME STREQUAL "Cryptopp")

  # License
  if(NAME STREQUAL "License")
  define_dependency(
    NAME "License"
    OUTPUT_PATH "${CMAKE_PREFIX_PATH}/Libraries"
    REMOTE_PATH "${ARTIFACTORY_URL}/release/Libraries/License/1.1.1/License_1.1.1_%TARGET%-%ARCH%-QT${Qt5_VERSION}.zip"
    CREATEDIR)
  endif(NAME STREQUAL "License")
  
  # Qt5
  if(NAME STREQUAL "Qt5")
    set(Qt5_DIR "C:/Qt/Qt${Qt5_VERSION}/${Qt5_VERSION}/${Qt5_ARCH}/lib/cmake/Qt5")
    find_package(Qt5 ${Qt5_VERSION} COMPONENTS Core Gui Widgets Xml Network REQUIRED)
  endif(NAME STREQUAL "Qt5")

  # QstBase
  if(NAME STREQUAL "QstBase")
    define_dependency(
      NAME "QstBase"
      OUTPUT_PATH "${CMAKE_PREFIX_PATH}/Libraries/Qst/Base"
      REMOTE_PATH "${ARTIFACTORY_URL}/release/Libraries/Qst/Base/2.0.0/QsCore/QsCore_2.0.0_%TARGET%_%ARCH%_QT${Qt5_VERSION}.zip"
      CREATEDIR
      NOFIND)
    define_dependency(
      NAME "QstBase"
      OUTPUT_PATH "${CMAKE_PREFIX_PATH}/Libraries/Qst/Base"
      REMOTE_PATH "${ARTIFACTORY_URL}/release/Libraries/Qst/Base/2.0.0/QsGui/QsGui_2.0.0_%TARGET%_%ARCH%_QT${Qt5_VERSION}.zip"
      CREATEDIR)
  endif(NAME STREQUAL "QstBase")

  # QstModules
  if(NAME STREQUAL "QstModules")
    define_dependency(
      NAME "QstModules"
      OUTPUT_PATH "${CMAKE_PREFIX_PATH}/Libraries/Qst/Modules"
      REMOTE_PATH "${ARTIFACTORY_URL}/release/Libraries/Qst/Modules/2.0.0/QsException/QsException_2.0.0_%TARGET%_%ARCH%_QT${Qt5_VERSION}.zip"
      CREATEDIR
      NOFIND)
    define_dependency(
      NAME "QstModules"
      OUTPUT_PATH "${CMAKE_PREFIX_PATH}/Libraries/Qst/Modules"
      REMOTE_PATH "${ARTIFACTORY_URL}/release/Libraries/Qst/Modules/2.0.0/QsLog/QsLog_2.0.0_%TARGET%_%ARCH%_QT${Qt5_VERSION}.zip"
      CREATEDIR)
  endif(NAME STREQUAL "QstModules")

endfunction(get_dependency_starlinx NAME)

get_dependency_starlinx(Qt5)