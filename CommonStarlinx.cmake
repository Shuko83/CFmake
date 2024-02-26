
include(Common)
include(generate_target_info)

set(CMAKE_PROJECT_INCLUDE "${CMAKE_SOURCE_DIR}/cmake/Version.cmake")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# # Allow to call CommonStarlinx before project()
# set(CMAKE_PROJECT_INCLUDE "${CMAKE_SOURCE_DIR}/cmake/CommonStarlinx.cmake")

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY AUTOGEN_SOURCE_GROUP "Generated Files")

set(ARTIFACTORY_URL "https://artifactory.divst:8081/artifactory" CACHE STRING "Artifactory base URL")

set(Qt5_VERSION "5.9.6" CACHE INTERNAL "Qt5 version")
set(Qt5_ARCH "msvc2015_64" CACHE INTERNAL "Qt5 architecture")
set(Qt5_DIR "C:/Qt/Qt${Qt5_VERSION}/${Qt5_VERSION}/${Qt5_ARCH}/lib/cmake/Qt5")

add_compile_definitions(UNICODE)

function(get_dependency_starlinx NAME)

  set(DD_ARCHS "x64")
  set(DD_TARGETS "win-msvc2015")

  # CODX CxConversionUtils
  if(NAME STREQUAL "CxConversionUtils")
    define_dependency(
      NAME "CxConversionUtils"
      OUTPUT_PATH "${CMAKE_PREFIX_PATH}/Frameworks/CODX/Core"
      REMOTE_PATH "${ARTIFACTORY_URL}/release/Frameworks.CODX.Core/${CODX_CORE_VERSION}/%TARGET%${TARGET_ARCH_SEPARATOR}%ARCH%/Frameworks.CODX.Core.ConversionUtils_${CODX_CORE_VERSION}_%TARGET%-%ARCH%.zip"
      TARGETS "${CODX_CORE_TARGETS}"
      CREATEDIR)
  endif(NAME STREQUAL "CxConversionUtils")

  # CODX CxLicensing
  if(NAME STREQUAL "CxLicensing")
    define_dependency(
      NAME "CxLicensing"
      OUTPUT_PATH "${CMAKE_PREFIX_PATH}/Frameworks/CODX/Core"
      REMOTE_PATH "${ARTIFACTORY_URL}/release/Frameworks.CODX.Core/${CODX_CORE_VERSION}/%TARGET%${TARGET_ARCH_SEPARATOR}%ARCH%/Frameworks.CODX.Core.CxLicensing_${CODX_CORE_VERSION}_%TARGET%-%ARCH%.zip"
      TARGETS "${CODX_CORE_TARGETS}"
      CREATEDIR)
  endif(NAME STREQUAL "CxLicensing")

  # CODX CxMessagesUtils
  if(NAME STREQUAL "CxMessagesUtils")
    define_dependency(
      NAME "CxMessagesUtils"
      OUTPUT_PATH "${CMAKE_PREFIX_PATH}/Frameworks/CODX/Core"
      REMOTE_PATH "${ARTIFACTORY_URL}/release/Frameworks.CODX.Core/${CODX_CORE_VERSION}/%TARGET%${TARGET_ARCH_SEPARATOR}%ARCH%/Frameworks.CODX.Core.MessagesUtils_${CODX_CORE_VERSION}_%TARGET%-%ARCH%.zip"
      TARGETS "${CODX_CORE_TARGETS}"
      CREATEDIR)
  endif(NAME STREQUAL "CxMessagesUtils")

  # CODX CxUtils
  if(NAME STREQUAL "CxUtils")
    define_dependency(
      NAME "CxUtils"
      OUTPUT_PATH "${CMAKE_PREFIX_PATH}/Frameworks/CODX/Core"
      REMOTE_PATH "${ARTIFACTORY_URL}/release/Frameworks.CODX.Core/${CODX_CORE_VERSION}/%TARGET%${TARGET_ARCH_SEPARATOR}%ARCH%/Frameworks.CODX.Core.CxUtils_${CODX_CORE_VERSION}_%TARGET%-%ARCH%.zip"
      TARGETS "${CODX_CORE_TARGETS}"
      CREATEDIR)
  endif(NAME STREQUAL "CxUtils")

  # CODX L11
  if(NAME STREQUAL "CodxL11")
    define_dependency(
      NAME "CodxL11"
      DIRNAME "L11ED${L11_EDITION}"
      OUTPUT_PATH "${CMAKE_PREFIX_PATH}/Protocols/CODX"
      REMOTE_PATH "${ARTIFACTORY_URL}/release/Protocols/CODX/L11/ED${L11_EDITION}/${CODX_L11_VERSION}/internal/L11ED${L11_EDITION}-internal_${CODX_L11_VERSION}_${CODX_L11_IDENTIFIER}_%TARGET%-%ARCH%.zip"
      FIND_PATH "${CMAKE_PREFIX_PATH}/Protocols/CODX/L11ED${L11_EDITION}"
      TARGETS "${CODX_MESSAGES_TARGETS}"
      CREATEDIR)
  endif(NAME STREQUAL "CodxL11")

  # CODX L16
  if(NAME STREQUAL "CodxL16")
    define_dependency(
      NAME "CodxL16"
      DIRNAME "L16ED${L16_EDITION}"
      OUTPUT_PATH "${CMAKE_PREFIX_PATH}/Protocols/CODX"
      REMOTE_PATH "${ARTIFACTORY_URL}/release/Protocols/CODX/L16/ED${L16_EDITION}/${CODX_L16_VERSION}/internal/L16ED${L16_EDITION}-internal_${CODX_L16_VERSION}_${CODX_L16_IDENTIFIER}_%TARGET%-%ARCH%.zip"
      FIND_PATH "${CMAKE_PREFIX_PATH}/Protocols/CODX/L16ED${L16_EDITION}"
      TARGETS "${CODX_MESSAGES_TARGETS}"
      CREATEDIR)
  endif(NAME STREQUAL "CodxL16")

  # CODX L22
  if(NAME STREQUAL "CodxL22")
    define_dependency(
      NAME "CodxL22"
      DIRNAME "L22ED${L22_EDITION}"
      OUTPUT_PATH "${CMAKE_PREFIX_PATH}/Protocols/CODX"
      REMOTE_PATH "${ARTIFACTORY_URL}/release/Protocols/CODX/L22/ED${L22_EDITION}/${CODX_L22_VERSION}/internal/L22ED${L22_EDITION}-internal_${CODX_L22_VERSION}_${CODX_L22_IDENTIFIER}_%TARGET%-%ARCH%.zip"
      FIND_PATH "${CMAKE_PREFIX_PATH}/Protocols/CODX/L22ED${L22_EDITION}"
      TARGETS "${CODX_MESSAGES_TARGETS}"
      CREATEDIR)
  endif(NAME STREQUAL "CodxL22")

  # CODX Asterix
  if(NAME STREQUAL "CodxAsterix")
    define_dependency(
      NAME "CodxAsterix"
      DIRNAME "ASTERIXED${ASTERIX_EDITION}"
      OUTPUT_PATH "${CMAKE_PREFIX_PATH}/Protocols/CODX"
      REMOTE_PATH "${ARTIFACTORY_URL}/release/Protocols/CODX/ASTERIX/ED${ASTERIX_EDITION}/${CODX_ASTERIX_VERSION}/internal/ASTERIX_ED${ASTERIX_EDITION}_${CODX_ASTERIX_VERSION}_internal_%TARGET%-%ARCH%_Delivery.zip"
      FIND_PATH "${CMAKE_PREFIX_PATH}/Protocols/CODX/ASTERIXED${ASTERIX_EDITION}"
      TARGETS "${CODX_MESSAGES_TARGETS}"
      CREATEDIR)
  endif(NAME STREQUAL "CodxAsterix")

  # CODX L16OPSTranslation
  if(NAME STREQUAL "L16OPSTranslation")
    define_dependency(
      NAME "L16OPSTranslation"
      OUTPUT_PATH "${CMAKE_PREFIX_PATH}/Protocols/CODX"
      REMOTE_PATH "${ARTIFACTORY_URL}/release/Protocols/CODX/5616_CDC/1.0.2/internal/5616_CDC_L16_L11_1.0.2_%TARGET%-%ARCH%_Delivery.zip"
      FIND_PATH "${CMAKE_PREFIX_PATH}/Protocols/CODX/L16OPSTranslation"
      TARGETS "${CODX_MESSAGES_TARGETS}"
      CREATEDIR)
  endif(NAME STREQUAL "L16OPSTranslation")

  # DCLI L16MsgJUtils
  if(NAME STREQUAL "DCLIL16MsgJUtils")
    define_dependency(
      NAME "DCLIL16MsgJUtils"
      DIRNAME "L16MsgJUtils"
      OUTPUT_PATH "${CMAKE_PREFIX_PATH}/Libraries/DCLI"
      REMOTE_PATH "${ARTIFACTORY_URL}/release/Libraries/DCLI/ExportControlFree/L16MsgJUtils/1.0.0.2/%TARGET%/%ARCH%/L16MsgJUtils_1.0.0.2_%TARGET%-%ARCH%.zip"
      CREATEDIR)
  endif(NAME STREQUAL "DCLIL16MsgJUtils")

  # DCLI Utils
  if(NAME STREQUAL "DCLIUtils")
    define_dependency(
      NAME "DCLIUtils"
      DIRNAME "Utils"
      OUTPUT_PATH "${CMAKE_PREFIX_PATH}/Libraries/DCLI"
      REMOTE_PATH "${ARTIFACTORY_URL}/snapshot/DLPX-PI.SVN.Exports/Libraries.DCLI.Utils_C1111_Suricate.zip"
      CREATEDIR)
  endif(NAME STREQUAL "DCLIUtils")

  # APILicensing
  if(NAME STREQUAL "APILicensing")
    define_dependency(
      NAME "Licensing"
      OUTPUT_PATH "${CMAKE_PREFIX_PATH}/Libraries"
      REMOTE_PATH "${ARTIFACTORY_URL}/release/Libraries.Licensing/5.1.0/%TARGET%/%ARCH%/Libraries.Licensing-5.1.0-%TARGET%-%ARCH%.zip"
      TARGETS "${APILICENSING_TARGETS}"
      CREATEDIR)
  endif(NAME STREQUAL "APILicensing")

  # APITacticalX
  if(NAME STREQUAL "APITacticalX")
    define_dependency(
      NAME "ApiTacticalX"
      OUTPUT_PATH "${CMAKE_PREFIX_PATH}/Libraries/INM"
      REMOTE_PATH "${ARTIFACTORY_URL}/snapshot/DLPX-PI.SVN.Exports/Libraries.DCLI.ApiTacticalX_trunk.zip"
      CREATEDIR)
  endif(NAME STREQUAL "APITacticalX")

  # LinkManagerFramework
  if(NAME STREQUAL "LinkManagerFramework")
    define_dependency(
      NAME "LnkMgrFwk"
      OUTPUT_PATH "${CMAKE_PREFIX_PATH}/Libraries/INM"
      REMOTE_PATH "${ARTIFACTORY_URL}/release/Libraries/LnkMgrFwk/1.0.2.0/%TARGET%${TARGET_ARCH_SEPARATOR}/%ARCH%/LnkMgrFwk_1.0.2.0_%TARGET%-%ARCH%.zip"
      TARGETS "${LINKMANAGERFRAMEWORK_TARGETS}"
      CREATEDIR)
  endif(NAME STREQUAL "LinkManagerFramework")

  # LogX
  if(NAME STREQUAL "LogX")
    set_os_variable(LOGX_VERSION "1.3.7" "1.3.8")
    define_dependency(
      NAME "LogX"
      OUTPUT_PATH "${CMAKE_PREFIX_PATH}/Libraries"
      REMOTE_PATH "${ARTIFACTORY_URL}/release/Libraries/LogX/${LOGX_VERSION}/LogX_${LOGX_VERSION}_%TARGET%-%ARCH%.zip"
      TARGETS "${LOGX_TARGETS}"
      CREATEDIR)
  endif(NAME STREQUAL "LogX")

  # ReadOLM
  if(NAME STREQUAL "ReadOLM")
    define_dependency(
      NAME "ReadOLM"
      OUTPUT_PATH "${CMAKE_PREFIX_PATH}/Libraries/INM"
      REMOTE_PATH "${ARTIFACTORY_URL}/snapshot/DLPX-PI.SVN.Exports/Libraries.ReadOLM_1.1.0.zip"
      CREATEDIR)
  endif(NAME STREQUAL "ReadOLM")

  # CodxGenerator
  if(NAME STREQUAL "CodxGenerator")
    download_dependency(
      DIRNAME "CodxGenerator"
      OUTPUT_PATH "${CMAKE_PREFIX_PATH}/Products/CODX"
      REMOTE_PATH "${ARTIFACTORY_URL}/release/Frameworks.CODX.MessagesGenerator/${CODX_GENERATOR_VERSION}/MessageGenerator_${CODX_GENERATOR_VERSION}.zip"
      CREATEDIR)
    set(CodxGenerator_DIR "${CMAKE_PREFIX_PATH}/Products/CODX/CodxGenerator/release" PARENT_SCOPE)
  endif(NAME STREQUAL "CodxGenerator")

  # DlpxHiGenerator
  if(NAME STREQUAL "DlpxHiGenerator")
    download_dependency(
      DIRNAME "DlpxHiGenerator"
      OUTPUT_PATH "${CMAKE_PREFIX_PATH}/Products/DLPX-HI"
      REMOTE_PATH "${ARTIFACTORY_URL}/release/DLPX.HI.Generator/${DLPX_HI_GENERATOR_VERSION}/DlpxHiGenerator_${DLPX_HI_GENERATOR_VERSION}_c_%ARCH%.zip"
      ARCHS "x64"
      TARGETS "2015"
      CREATEDIR)
    set(DlpxHiGenerator_DIR "${CMAKE_PREFIX_PATH}/Products/DLPX-HI/DlpxHiGenerator" PARENT_SCOPE)
  endif(NAME STREQUAL "DlpxHiGenerator")

  # Qt5
  if(NAME STREQUAL "Qt5")
    if(NOT WIN32)
      download_dependency(
        DIRNAME "Qt-${Qt5_VERSION}"
        OUTPUT_PATH "${CMAKE_PREFIX_PATH}/3rdParty"
        REMOTE_PATH "${ARTIFACTORY_URL}/thirdParty/Qt/${Qt5_VERSION}/%TARGET%/%ARCH%/Qt-${Qt5_VERSION}_%TARGET%-%ARCH%.zip"
        ARCHS "x64"
        TARGETS "linux-gcc11.3")
    endif(NOT WIN32)
    find_package(Qt5 ${Qt5_VERSION} COMPONENTS Core REQUIRED)
  endif(NAME STREQUAL "Qt5")

  # Poco
  if(NAME STREQUAL "Poco")
    define_dependency(
      NAME "Poco"
      DIRNAME "Poco-1.10.1"
      OUTPUT_PATH "${CMAKE_PREFIX_PATH}/3rdParty"
      REMOTE_PATH "${ARTIFACTORY_URL}/thirdParty/Poco/1.10.1/%TARGET%/%ARCH%/Poco-1.10.1-1_%TARGET%-%ARCH%.zip"
      COMPONENTS "Data" "DataSQLite" "Foundation" "JSON" "Net" "Util" "XML"
      CMAKE)
  endif(NAME STREQUAL "Poco")

  # GeographicLib
  if(NAME STREQUAL "GeographicLib")
    define_dependency(
      NAME "GeographicLib"
      DIRNAME "GeographicLib-1.50"
      OUTPUT_PATH "${CMAKE_PREFIX_PATH}/3rdParty"
      REMOTE_PATH "${ARTIFACTORY_URL}/thirdParty/GeographicLib/1.50/%TARGET%/%ARCH%/GeographicLib-1.50-1_%TARGET%-%ARCH%.zip"
      CMAKE)
  endif(NAME STREQUAL "GeographicLib")

  # NMEA
  if(NAME STREQUAL "NMEA")
    define_dependency(
      NAME "NMEA"
      DIRNAME "NMEA-0.5.3"
      OUTPUT_PATH "${CMAKE_PREFIX_PATH}/3rdParty"
      REMOTE_PATH "${ARTIFACTORY_URL}/thirdParty/NMEA/0.5.3/%TARGET%/%ARCH%/NMEA-0.5.3_%TARGET%-%ARCH%.zip"
      CREATEDIR)
  endif(NAME STREQUAL "NMEA")

  # VortexOpenSplice
  if(NAME STREQUAL "VortexOpenSplice")
    define_dependency(
      NAME "VortexOpenSplice"
      DIRNAME "VortexOpenSplice-6.11.2p5"
      OUTPUT_PATH "${CMAKE_PREFIX_PATH}/3rdParty"
      REMOTE_PATH "${ARTIFACTORY_URL}/thirdParty/VortexOpenSplice/6.11.2p5/%TARGET%/%ARCH%/VortexOpenSplice_6.11.2p5_%TARGET%-%ARCH%.zip"
      CREATEDIR)
  endif(NAME STREQUAL "VortexOpenSplice")

  # TinyXML2
  if(NAME STREQUAL "TinyXML2")
    define_dependency(
      NAME "TinyXML2"
      DIRNAME "TinyXML2-5.0.1"
      OUTPUT_PATH "${CMAKE_PREFIX_PATH}/3rdParty"
      REMOTE_PATH "${ARTIFACTORY_URL}/thirdParty/TinyXML2/5.0.1/%TARGET%/%ARCH%/TinyXML2_5.0.1_%TARGET%-%ARCH%.zip"
      TARGETS "${TINYXML2_TARGETS}"
      CREATEDIR)
  endif(NAME STREQUAL "TinyXML2")

  # GoogleTest
  if(NAME STREQUAL "GoogleTest")
    define_dependency(
      NAME "GTest"
      DIRNAME "GoogleTest-1.8.1"
      OUTPUT_PATH "${CMAKE_PREFIX_PATH}/3rdParty"
      REMOTE_PATH "${ARTIFACTORY_URL}/thirdParty/GoogleTest/1.8.1/%TARGET%/%ARCH%/GoogleTest-1.8.1-1_%TARGET%-%ARCH%.zip"
      CMAKE)
  endif(NAME STREQUAL "GoogleTest")

  # gRPC
  if(NAME STREQUAL "gRPC")
    define_dependency(
      NAME "gRPC"
      DIRNAME "gRPC-1.45.0"
      OUTPUT_PATH "${CMAKE_PREFIX_PATH}/3rdParty"
      REMOTE_PATH "${ARTIFACTORY_URL}/thirdParty/gRPC/1.45.0/%TARGET%/%ARCH%/gRPC_1.45.0_%TARGET%-%ARCH%.zip"
      CMAKE)
  endif(NAME STREQUAL "gRPC")

  # SYSCOM
  if(NAME STREQUAL "SYSCOM")
    define_dependency(
      NAME "SYSCOM"
      DIRNAME "SYS_COM_v1.0.1"
      OUTPUT_PATH "${CMAKE_PREFIX_PATH}/Protocols"
      REMOTE_PATH "${ARTIFACTORY_URL}/snapshot/DLPX-PI/CLAv3/develop/SYS_COM_v1.0.1_%TARGET%-%ARCH%.zip")
  endif(NAME STREQUAL "SYSCOM")

  # ACE+TAO
  if(NAME STREQUAL "ACETAO")
    define_dependency(
      NAME "ACETAO"
      DIRNAME "ACE+TAO-2.2a"
      OUTPUT_PATH "${CMAKE_PREFIX_PATH}/3rdParty"
      REMOTE_PATH "${ARTIFACTORY_URL}/snapshot/DLPX-PI/CLAv3/develop/ACE+TAO-2.2a_%TARGET%-%ARCH%.zip")
  endif(NAME STREQUAL "ACETAO")

  # VCRedist
  if(NAME STREQUAL "VCRedist")
    download_dependency(
      DIRNAME "VCRedist-14.0.23026.0"
      OUTPUT_PATH "${CMAKE_PREFIX_PATH}/3rdParty"
      REMOTE_PATH "${ARTIFACTORY_URL}/thirdParty/VisualRedistribuable/%TARGET%/%ARCH%/vc_redist.%ARCH%.exe"
      ARCHS "x64"
      TARGETS "2015")
    set(VCRedist_ROOT "${CMAKE_PREFIX_PATH}/3rdParty/VCRedist-14.0.23026.0" PARENT_SCOPE)
  endif(NAME STREQUAL "VCRedist")

  # HASP for GEMALTO RTE
  if(NAME STREQUAL "HASP")
    download_dependency(
      DIRNAME "HASP-8.4"
      OUTPUT_PATH "${CMAKE_PREFIX_PATH}/3rdParty"
      REMOTE_PATH "${ARTIFACTORY_URL}/thirdParty/HASP/8.4/Installer/Windows/hasp_rt.exe")
    download_dependency(
      DIRNAME "HASP-8.4"
      OUTPUT_PATH "${CMAKE_PREFIX_PATH}/3rdParty"
      REMOTE_PATH "${ARTIFACTORY_URL}/thirdParty/HASP/8.4/Installer/Windows/haspdinst.exe")
    set(HASP_ROOT "${CMAKE_PREFIX_PATH}/3rdParty/HASP-8.4" PARENT_SCOPE)
  endif(NAME STREQUAL "HASP")

  # CrashRpt
  if(NAME STREQUAL "CrashRpt")
    define_dependency(
      NAME "CrashRpt"
      DIRNAME "CrashRpt-2.0.2"
      OUTPUT_PATH "${CMAKE_PREFIX_PATH}/3rdParty"
      REMOTE_PATH "${ARTIFACTORY_URL}/thirdParty/CrashRpt/2.0.2/CrashRpt-2.0.2.zip"
      FIND_PATH "${CMAKE_PREFIX_PATH}/3rdParty/CrashRpt-2.0.2/Delivery"
      CREATEDIR)
  endif(NAME STREQUAL "CrashRpt")

  # Tracy
  if(NAME STREQUAL "Tracy" AND ENABLE_TELEMETRY)
    define_dependency(
      NAME "Tracy"
      DIRNAME "TracyClient-0.9.0"
      OUTPUT_PATH "${CMAKE_PREFIX_PATH}/3rdParty"
      REMOTE_PATH "${ARTIFACTORY_URL}/thirdParty/TracyClient/0.9.0/%TARGET%/%ARCH%/TracyClient-0.9.0_%TARGET%-%ARCH%.zip"
      CMAKE)
    find_package(Threads)
    add_compile_definitions(ENABLE_TELEMETRY)
  endif(NAME STREQUAL "Tracy" AND ENABLE_TELEMETRY)

  
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
