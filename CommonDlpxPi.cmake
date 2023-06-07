################################################################################
# Common includes
################################################################################

include(Common)
include(LibrariesCodx)
include(LibrariesDlpxHi)
include(LibrariesVortexOpenSplice)

################################################################################
# Workspace definition
################################################################################

set(ARTIFACTORY_URL "http://10.20.1.10:8081/artifactory" CACHE STRING "Artifactory base URL")
set(GIT_URL "https://10.19.74.16/dlpx-pi" CACHE STRING "Git base URL")
set(JENKINS_URL "https://10.19.74.19" CACHE STRING "Jenkins base URL")
set(NEXUS_URL "https://10.19.74.15" CACHE STRING "Nexus base URL")
set(SUBVERSION_URL "https://10.20.1.30/svn" CACHE STRING "Subversion base URL")

################################################################################
# Project definition
################################################################################

set(FrameworksDLPXPICore_NAME "Frameworks.DLPX-PI.Core" CACHE INTERNAL "Frameworks.DLPX-PI.Core sub-project name")
set(LibrariesDLPXPICorrelator_NAME "Libraries.DLPX-PI.Correlator" CACHE INTERNAL "Libraries.DLPX-PI.Correlator sub-project name")
set(LibrariesDLPXPIReadOLMWrapper_NAME "Libraries.DLPX-PI.ReadOLMWrapper" CACHE INTERNAL "Libraries.DLPX-PI.ReadOLMWrapper sub-project name")
set(LibrariesDLPXPISNC_NAME "Libraries.DLPX-PI.SNC" CACHE INTERNAL "Libraries.DLPX-PI.SNC sub-project name")
set(LibrariesDLPXPITacticalX_NAME "Libraries.DLPX-PI.TacticalX" CACHE INTERNAL "Libraries.DLPX-PI.TacticalX sub-project name")
set(ProductsDLPXPIAsterix204DLP_NAME "Products.DLPX-PI.Asterix204DLP" CACHE INTERNAL "Products.DLPX-PI.Asterix204DLP sub-project name")
set(ProductsDLPXPIL22DLP_NAME "Products.DLPX-PI.L22DLP" CACHE INTERNAL "Products.DLPX-PI.L22DLP sub-project name")
set(ProductsDLPXPITestDLP_NAME "Products.DLPX-PI.TestDLP" CACHE INTERNAL "Products.DLPX-PI.TestDLP sub-project name")
set(ProtocolsDLPXPIL11_NAME "Protocols.DLPX-PI.L11" CACHE INTERNAL "Protocols.DLPX-PI.L11 sub-project name")
set(ProtocolsDLPXPIL16_NAME "Protocols.DLPX-PI.L16" CACHE INTERNAL "Protocols.DLPX-PI.L16 sub-project name")
set(ProtocolsDLPXPIL22_NAME "Protocols.DLPX-PI.L22" CACHE INTERNAL "Protocols.DLPX-PI.L22 sub-project name")

set(FrameworksDLPXPICore_VERSION "develop-aix" CACHE STRING "Frameworks.DLPX-PI.Core sub-project version")
set(LibrariesDLPXPICorrelator_VERSION "develop" CACHE STRING "Libraries.DLPX-PI.Correlator sub-project version")
set(LibrariesDLPXPIReadOLMWrapper_VERSION "develop" CACHE STRING "Libraries.DLPX-PI.ReadOLMWrapper sub-project version")
set(LibrariesDLPXPISNC_VERSION "develop" CACHE STRING "Libraries.DLPX-PI.SNC sub-project version")
set(LibrariesDLPXPITacticalX_VERSION "develop" CACHE STRING "Libraries.DLPX-PI.TacticalX sub-project version")
set(ProductsDLPXPIAsterix204DLP_VERSION "develop" CACHE STRING "Products.DLPX-PI.Asterix204DLP sub-project version")
set(ProductsDLPXPIL22DLP_VERSION "develop" CACHE STRING "Products.DLPX-PI.L22DLP sub-project version")
set(ProductsDLPXPITestDLP_VERSION "develop" CACHE STRING "Products.DLPX-PI.TestDLP sub-project version")
set(ProtocolsDLPXPIL11_VERSION "develop" CACHE STRING "Protocols.DLPX-PI.L11 sub-project version")
set(ProtocolsDLPXPIL16_VERSION "develop" CACHE STRING "Protocols.DLPX-PI.L16 sub-project version")
set(ProtocolsDLPXPIL22_VERSION "develop" CACHE STRING "Protocols.DLPX-PI.L22 sub-project version")

set(L11_EDITION "8" CACHE STRING "L11 edition")
set(L16_EDITION "8" CACHE STRING "L16 edition")
set(L22_EDITION "6" CACHE STRING "L22 edition")

set(CODX_CORE_VERSION "1.6.2" CACHE STRING "CODX Core version")
set(CODX_GENERATOR_VERSION "10.3.10" CACHE STRING "CODX Generator version")
set(CODX_L11_VERSION "10.3.10.060800" CACHE STRING "CODX L11 version")
set(CODX_L16_VERSION "10.3.10.060800" CACHE STRING "CODX L16 version")
set(CODX_L22_VERSION "10.3.11.060701" CACHE STRING "CODX L22 version")

set(DLPX_HI_VERSION "3.2.4" CACHE STRING "DLPX-HI version")
set(DLPX_HI_COMMON_VERSION "3.2.2" CACHE STRING "DLPX-HI Common version")
set(DLPX_HI_CODX_VERSION "10.3.9.030204" CACHE STRING "DLPX-HI CODX version")
set(DLPX_HI_GENERATOR_VERSION "3.2.3" CACHE STRING "DLPX-HI Generator version")

set(Qt5_VERSION "5.9.6" CACHE INTERNAL "Qt5 version")
set(Qt5_ARCH "msvc2015_64" CACHE INTERNAL "Qt5 architecture")

set(ARTIFACTORY_DIR "snapshot" CACHE INTERNAL "Artifactory directory")

set(ENABLE_NATO_RESTRICTION TRUE CACHE INTERNAL "Enable NATO restriction")
set(ENABLE_ITAR_RESTRICTION FALSE CACHE INTERNAL "Enable ITAR restriction")

################################################################################
# Project options
################################################################################

# ...

################################################################################
# Compilation definition
################################################################################

# ...

################################################################################
# Compilation options
################################################################################

option(ENABLE_TELEMETRY "Enable telemetry" FALSE)

################################################################################
# Sub-projects definition
################################################################################

function(get_sub_project_dlpxpi NAME)

  # Frameworks.DLPX-PI.Core
  if(NAME STREQUAL "Core")
    define_sub_project(
      NAME "${FrameworksDLPXPICore_NAME}"
      REMOTE_PATH "${ARTIFACTORY_URL}/${ARTIFACTORY_DIR}/DLPX-PI/${FrameworksDLPXPICore_NAME}/${FrameworksDLPXPICore_VERSION}/${FrameworksDLPXPICore_NAME}.n.${FrameworksDLPXPICore_VERSION}.zip")
  endif(NAME STREQUAL "Core")

  # Libraries.DLPX-PI.Correlator
  if(NAME STREQUAL "Correlator")
    define_sub_project(
      NAME "${LibrariesDLPXPICorrelator_NAME}"
      REMOTE_PATH "${ARTIFACTORY_URL}/${ARTIFACTORY_DIR}/DLPX-PI/${LibrariesDLPXPICorrelator_NAME}/${LibrariesDLPXPICorrelator_VERSION}/${LibrariesDLPXPICorrelator_NAME}.n.${LibrariesDLPXPICorrelator_VERSION}.zip")
  endif(NAME STREQUAL "Correlator")

  # Libraries.DLPX-PI.ReadOLMWrapper
  if(NAME STREQUAL "ReadOLMWrapper")
    define_sub_project(
      NAME "${LibrariesDLPXPIReadOLMWrapper_NAME}"
      REMOTE_PATH "${ARTIFACTORY_URL}/${ARTIFACTORY_DIR}/DLPX-PI/${LibrariesDLPXPIReadOLMWrapper_NAME}/${LibrariesDLPXPIReadOLMWrapper_VERSION}/${LibrariesDLPXPIReadOLMWrapper_NAME}.n.${LibrariesDLPXPIReadOLMWrapper_VERSION}.zip")
  endif(NAME STREQUAL "ReadOLMWrapper")

  # Libraries.DLPX-PI.SNC
  if(NAME STREQUAL "SNC")
    define_sub_project(
      NAME "${LibrariesDLPXPISNC_NAME}"
      REMOTE_PATH "${ARTIFACTORY_URL}/${ARTIFACTORY_DIR}/DLPX-PI/${LibrariesDLPXPISNC_NAME}/${LibrariesDLPXPISNC_VERSION}/${LibrariesDLPXPISNC_NAME}.ni.${LibrariesDLPXPISNC_VERSION}.zip")
  endif(NAME STREQUAL "SNC")

  # Libraries.DLPX-PI.TacticalX
  if(NAME STREQUAL "TacticalX")
    define_sub_project(
      NAME "${LibrariesDLPXPITacticalX_NAME}"
      REMOTE_PATH "${ARTIFACTORY_URL}/${ARTIFACTORY_DIR}/DLPX-PI/${LibrariesDLPXPITacticalX_NAME}/${LibrariesDLPXPITacticalX_VERSION}/${LibrariesDLPXPITacticalX_NAME}.ni.${LibrariesDLPXPITacticalX_VERSION}.zip")
  endif(NAME STREQUAL "TacticalX")

  # Protocols.DLPX-PI.L11
  if(NAME STREQUAL "L11")
    define_sub_project(
      NAME "${ProtocolsDLPXPIL11_NAME}"
      REMOTE_PATH "${ARTIFACTORY_URL}/${ARTIFACTORY_DIR}/DLPX-PI/${ProtocolsDLPXPIL11_NAME}/${ProtocolsDLPXPIL11_VERSION}/${ProtocolsDLPXPIL11_NAME}.n.${ProtocolsDLPXPIL11_VERSION}.zip")
  endif(NAME STREQUAL "L11")

  # Protocols.DLPX-PI.L16
  if(NAME STREQUAL "L16")
    define_sub_project(
      NAME "${ProtocolsDLPXPIL16_NAME}"
      REMOTE_PATH "${ARTIFACTORY_URL}/${ARTIFACTORY_DIR}/DLPX-PI/${ProtocolsDLPXPIL16_NAME}/${ProtocolsDLPXPIL16_VERSION}/${ProtocolsDLPXPIL16_NAME}.n.${ProtocolsDLPXPIL16_VERSION}.zip")
  endif(NAME STREQUAL "L16")

  # Protocols.DLPX-PI.L22
  if(NAME STREQUAL "L22")
    define_sub_project(
      NAME "${ProtocolsDLPXPIL22_NAME}"
      REMOTE_PATH "${ARTIFACTORY_URL}/${ARTIFACTORY_DIR}/DLPX-PI/${ProtocolsDLPXPIL22_NAME}/${ProtocolsDLPXPIL22_VERSION}/${ProtocolsDLPXPIL22_NAME}.n.${ProtocolsDLPXPIL22_VERSION}.zip")
  endif(NAME STREQUAL "L22")

endfunction(get_sub_project_dlpxpi NAME)

################################################################################
# Dependencies definition
################################################################################

function(get_dependency_dlpxpi NAME)

  set(DD_ARCHS "x64")
  set(DD_TARGETS "win-msvc2015")

  # CODX CxConversionUtils
  if(NAME STREQUAL "CxConversionUtils")
    define_dependency(
      NAME "CxConversionUtils"
      OUTPUT_PATH "${CMAKE_PREFIX_PATH}/Frameworks/CODX/Core"
      REMOTE_PATH "${ARTIFACTORY_URL}/release/Frameworks.CODX.Core/${CODX_CORE_VERSION}/%TARGET%/%ARCH%/Frameworks.CODX.Core.ConversionUtils_${CODX_CORE_VERSION}_%TARGET%-%ARCH%.zip"
      CREATEDIR)
  endif(NAME STREQUAL "CxConversionUtils")

  # CODX CxLicensing
  if(NAME STREQUAL "CxLicensing")
    define_dependency(
      NAME "CxLicensing"
      OUTPUT_PATH "${CMAKE_PREFIX_PATH}/Frameworks/CODX/Core"
      REMOTE_PATH "${ARTIFACTORY_URL}/release/Frameworks.CODX.Core/${CODX_CORE_VERSION}/%TARGET%/%ARCH%/Frameworks.CODX.Core.CxLicensing_${CODX_CORE_VERSION}_%TARGET%-%ARCH%.zip"
      CREATEDIR)
  endif(NAME STREQUAL "CxLicensing")

  # CODX CxMessagesUtils
  if(NAME STREQUAL "CxMessagesUtils")
    define_dependency(
      NAME "CxMessagesUtils"
      OUTPUT_PATH "${CMAKE_PREFIX_PATH}/Frameworks/CODX/Core"
      REMOTE_PATH "${ARTIFACTORY_URL}/release/Frameworks.CODX.Core/${CODX_CORE_VERSION}/%TARGET%/%ARCH%/Frameworks.CODX.Core.MessagesUtils_${CODX_CORE_VERSION}_%TARGET%-%ARCH%.zip"
      CREATEDIR)
  endif(NAME STREQUAL "CxMessagesUtils")

  # CODX CxUtils
  if(NAME STREQUAL "CxUtils")
    define_dependency(
      NAME "CxUtils"
      OUTPUT_PATH "${CMAKE_PREFIX_PATH}/Frameworks/CODX/Core"
      REMOTE_PATH "${ARTIFACTORY_URL}/release/Frameworks.CODX.Core/${CODX_CORE_VERSION}/%TARGET%/%ARCH%/Frameworks.CODX.Core.CxUtils_${CODX_CORE_VERSION}_%TARGET%-%ARCH%.zip"
      CREATEDIR)
  endif(NAME STREQUAL "CxUtils")

  # CODX L11
  if(NAME STREQUAL "CodxL11")
    define_dependency(
      NAME "CodxL11"
      DIRNAME "L11ED${L11_EDITION}"
      OUTPUT_PATH "${CMAKE_PREFIX_PATH}/Protocols/CODX"
      REMOTE_PATH "${ARTIFACTORY_URL}/release/Protocols/CODX/L11/ED${L11_EDITION}/${CODX_L11_VERSION}/internal/L11_ED${L11_EDITION}_${CODX_L11_VERSION}_internal_%TARGET%-%ARCH%_Delivery.zip"
      FIND_PATH "${CMAKE_PREFIX_PATH}/Protocols/CODX/L11ED${L11_EDITION}"
      CREATEDIR)
  endif(NAME STREQUAL "CodxL11")

  # CODX L16
  if(NAME STREQUAL "CodxL16")
    define_dependency(
      NAME "CodxL16"
      DIRNAME "L16ED${L16_EDITION}"
      OUTPUT_PATH "${CMAKE_PREFIX_PATH}/Protocols/CODX"
      REMOTE_PATH "${ARTIFACTORY_URL}/release/Protocols/CODX/L16/ED${L16_EDITION}/${CODX_L16_VERSION}/internal/L16_ED${L16_EDITION}_${CODX_L16_VERSION}_internal_%TARGET%-%ARCH%_Delivery.zip"
      FIND_PATH "${CMAKE_PREFIX_PATH}/Protocols/CODX/L16ED${L16_EDITION}"
      CREATEDIR)
  endif(NAME STREQUAL "CodxL16")

  # CODX L22
  if(NAME STREQUAL "CodxL22")
    define_dependency(
      NAME "CodxL22"
      DIRNAME "L22ED${L22_EDITION}"
      OUTPUT_PATH "${CMAKE_PREFIX_PATH}/Protocols/CODX"
      REMOTE_PATH "${ARTIFACTORY_URL}/release/Protocols/CODX/L22/ED${L22_EDITION}/${CODX_L22_VERSION}/internal/L22_ED${L22_EDITION}_${CODX_L22_VERSION}_internal_%TARGET%-%ARCH%_Delivery.zip"
      FIND_PATH "${CMAKE_PREFIX_PATH}/Protocols/CODX/L22ED${L22_EDITION}"
      CREATEDIR)
  endif(NAME STREQUAL "CodxL22")

  # CODX L16OPSTranslation
  if(NAME STREQUAL "L16OPSTranslation")
    define_dependency(
      NAME "L16OPSTranslation"
      OUTPUT_PATH "${CMAKE_PREFIX_PATH}/Protocols/CODX"
      REMOTE_PATH "${ARTIFACTORY_URL}/release/L16OPS_Translations/1.1.6/CODX.STANAG_5616.1.1.6.3.cpp11.zip"
      FIND_PATH "${CMAKE_PREFIX_PATH}/Protocols/CODX/L16OPSTranslation/Delivery"
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
      REMOTE_PATH "${ARTIFACTORY_URL}/release/Libraries/LnkMgrFwk/1.0.2.0/%TARGET%/%ARCH%/LnkMgrFwk_1.0.2.0_%TARGET%-%ARCH%.zip"
      CREATEDIR)
  endif(NAME STREQUAL "LinkManagerFramework")

  # LogX
  if(NAME STREQUAL "LogX")
    define_dependency(
      NAME "LogX"
      OUTPUT_PATH "${CMAKE_PREFIX_PATH}/Libraries"
      REMOTE_PATH "${ARTIFACTORY_URL}/release/Libraries/LogX/1.3.4/LogX_1.3.4.zip"
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
    set(Qt5_DIR "C:/Qt/Qt${Qt5_VERSION}/${Qt5_VERSION}/${Qt5_ARCH}/lib/cmake/Qt5")
    set(Qt5Core_DIR "C:/Qt/Qt${Qt5_VERSION}/${Qt5_VERSION}/${Qt5_ARCH}/lib/cmake/Qt5Core")
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

  # VortexOpenSplice
  if(NAME STREQUAL "VortexOpenSplice")
    define_dependency(
      NAME "VortexOpenSplice"
      DIRNAME "VortexOpenSplice-6.11.2p5"
      OUTPUT_PATH "${CMAKE_PREFIX_PATH}/3rdParty"
      REMOTE_PATH "${ARTIFACTORY_URL}/thirdParty/VortexOpenSplice/6.11.2p5/%TARGET%/%ARCH%/VortexOpenSplice_6.11.2p5_%TARGET%_%ARCH%.zip"
      CREATEDIR)
  endif(NAME STREQUAL "VortexOpenSplice")

  # TinyXML2
  if(NAME STREQUAL "TinyXML2")
    define_dependency(
      NAME "TinyXML2"
      DIRNAME "TinyXML2-5.0.1"
      OUTPUT_PATH "${CMAKE_PREFIX_PATH}/3rdParty"
      REMOTE_PATH "${ARTIFACTORY_URL}/thirdParty/TinyXML2/5.0.1/%TARGET%/%ARCH%/TinyXML2_5.0.1_%TARGET%-%ARCH%.zip"
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

endfunction(get_dependency_dlpxpi NAME)

function(get_dependency_components_dlpxpi NAME COMPONENTS)

  set(DD_ARCHS "x64")
  set(DD_TARGETS "win-msvc2015")

  # DLPX-HI
  if(NAME STREQUAL "DlpxHi")

    foreach(COMPONENT IN LISTS COMPONENTS)

      # DLPX-HI Common
      if(COMPONENT STREQUAL "Common")
        define_dependency(
          NAME "DlpxHiCommon"
          DIRNAME "DlpxHiCommon"
          OUTPUT_PATH "${CMAKE_PREFIX_PATH}/Libraries/DLPX-HI"
          REMOTE_PATH "${ARTIFACTORY_URL}/release/DLPX.HI.Common/${DLPX_HI_COMMON_VERSION}/DlpxHiCommon_${DLPX_HI_COMMON_VERSION}_%TARGET%_%ARCH%.zip"
          CREATEDIR)
        list(APPEND ${NAME}_LIBRARIES "${DlpxHiCommon_LIBRARIES}")
      endif(COMPONENT STREQUAL "Common")

      # DLPX-HI DlpxStates
      if(COMPONENT STREQUAL "DlpxStates")
        define_dependency(
          NAME "DlpxHiDlpxStates"
          DIRNAME "DlpxStates"
          OUTPUT_PATH "${CMAKE_PREFIX_PATH}/Libraries/DLPX-HI"
          REMOTE_PATH "${ARTIFACTORY_URL}/release/DLPX.HI.Models/LOC/ED${L16_EDITION}/${DLPX_HI_VERSION}/DlpxStates_LOCED${L16_EDITION}_${DLPX_HI_VERSION}_%TARGET%_%ARCH%_Delivery.zip"
          CREATEDIR)
        list(APPEND ${NAME}_LIBRARIES "${DlpxHiDlpxStates_LIBRARIES}")
      endif(COMPONENT STREQUAL "DlpxStates")

      # DLPX-HI MDlpConsolidated
      if(COMPONENT STREQUAL "MDlpConsolidated")
        define_dependency(
          NAME "DlpxHiMDlpConsolidated"
          DIRNAME "MDlpConsolidated"
          OUTPUT_PATH "${CMAKE_PREFIX_PATH}/Libraries/DLPX-HI"
          REMOTE_PATH "${ARTIFACTORY_URL}/release/DLPX.HI.Models/L16/ED${L16_EDITION}/${DLPX_HI_VERSION}/MDlpConsolidated_L16ED${L16_EDITION}_${DLPX_HI_VERSION}_%TARGET%_%ARCH%_Delivery.zip"
          CREATEDIR)
        list(APPEND ${NAME}_LIBRARIES "${DlpxHiMDlpConsolidated_LIBRARIES}")
      endif(COMPONENT STREQUAL "MDlpConsolidated")

      # DLPX-HI MDlpSettings
      if(COMPONENT STREQUAL "MDlpSettings")
        define_dependency(
          NAME "DlpxHiMDlpSettings"
          DIRNAME "MDlpSettings"
          OUTPUT_PATH "${CMAKE_PREFIX_PATH}/Libraries/DLPX-HI"
          REMOTE_PATH "${ARTIFACTORY_URL}/release/DLPX.HI.Models/LOC/ED${L16_EDITION}/${DLPX_HI_VERSION}/MDlpSettings_LOCED${L16_EDITION}_${DLPX_HI_VERSION}_%TARGET%_%ARCH%_Delivery.zip"
          CREATEDIR)
        list(APPEND ${NAME}_LIBRARIES "${DlpxHiMDlpSettings_LIBRARIES}")
      endif(COMPONENT STREQUAL "MDlpSettings")

      # DLPX-HI MDlpSurv
      if(COMPONENT STREQUAL "MDlpSurv")
        define_dependency(
          NAME "DlpxHiMDlpSurv"
          DIRNAME "MDlpSurv"
          OUTPUT_PATH "${CMAKE_PREFIX_PATH}/Libraries/DLPX-HI"
          REMOTE_PATH "${ARTIFACTORY_URL}/release/DLPX.HI.Models/LOC/ED${L16_EDITION}/${DLPX_HI_VERSION}/MDlpSurv_LOCED${L16_EDITION}_${DLPX_HI_VERSION}_%TARGET%_%ARCH%_Delivery.zip"
          CREATEDIR)
        list(APPEND ${NAME}_LIBRARIES "${DlpxHiMDlpSurv_LIBRARIES}")
      endif(COMPONENT STREQUAL "MDlpSurv")

      # DLPX-HI TnpL11
      if(COMPONENT STREQUAL "TnpL11")
        define_dependency(
          NAME "DlpxHiTnpL11"
          DIRNAME "TnpL11"
          OUTPUT_PATH "${CMAKE_PREFIX_PATH}/Libraries/DLPX-HI"
          REMOTE_PATH "${ARTIFACTORY_URL}/release/DLPX.HI.Models/L11/ED${L11_EDITION}/${DLPX_HI_VERSION}/TnpL11_L11ED${L11_EDITION}_${DLPX_HI_VERSION}_%TARGET%_%ARCH%_Delivery.zip"
          CREATEDIR)
        list(APPEND ${NAME}_LIBRARIES "${DlpxHiTnpL11_LIBRARIES}")
      endif(COMPONENT STREQUAL "TnpL11")

      # DLPX-HI TnpL16
      if(COMPONENT STREQUAL "TnpL16")
        define_dependency(
          NAME "DlpxHiTnpL16"
          DIRNAME "TnpL16"
          OUTPUT_PATH "${CMAKE_PREFIX_PATH}/Libraries/DLPX-HI"
          REMOTE_PATH "${ARTIFACTORY_URL}/release/DLPX.HI.Models/L16/ED${L16_EDITION}/${DLPX_HI_VERSION}/TnpL16_L16ED${L16_EDITION}_${DLPX_HI_VERSION}_%TARGET%_%ARCH%_Delivery.zip"
          CREATEDIR)
        list(APPEND ${NAME}_LIBRARIES "${DlpxHiTnpL16_LIBRARIES}")
      endif(COMPONENT STREQUAL "TnpL16")

      # DLPX-HI TnpL22
      if(COMPONENT STREQUAL "TnpL22")
        define_dependency(
          NAME "DlpxHiTnpL22"
          DIRNAME "TnpL22"
          OUTPUT_PATH "${CMAKE_PREFIX_PATH}/Libraries/DLPX-HI"
          REMOTE_PATH "${ARTIFACTORY_URL}/release/DLPX.HI.Models/L22/ED${L22_EDITION}/${DLPX_HI_VERSION}/TnpL22_L22ED${L22_EDITION}_${DLPX_HI_VERSION}_%TARGET%_%ARCH%_Delivery.zip"
          CREATEDIR)
        list(APPEND ${NAME}_LIBRARIES "${DlpxHiTnpL22_LIBRARIES}")
      endif(COMPONENT STREQUAL "TnpL22")

      # DLPX-HI TnpSettingsL11
      if(COMPONENT STREQUAL "TnpSettingsL11")
        define_dependency(
          NAME "DlpxHiTnpSettingsL11"
          DIRNAME "TnpSettingsL11"
          OUTPUT_PATH "${CMAKE_PREFIX_PATH}/Libraries/DLPX-HI"
          REMOTE_PATH "${ARTIFACTORY_URL}/release/DLPX.HI.Models/L11/ED${L11_EDITION}/${DLPX_HI_VERSION}/TnpSettingsL11_L11ED${L11_EDITION}_${DLPX_HI_VERSION}_%TARGET%_%ARCH%_Delivery.zip"
          CREATEDIR)
        list(APPEND ${NAME}_LIBRARIES "${DlpxHiTnpSettingsL11_LIBRARIES}")
      endif(COMPONENT STREQUAL "TnpSettingsL11")

      # DLPX-HI TnpSettingsL16
      if(COMPONENT STREQUAL "TnpSettingsL16")
        define_dependency(
          NAME "DlpxHiTnpSettingsL16"
          DIRNAME "TnpSettingsL16"
          OUTPUT_PATH "${CMAKE_PREFIX_PATH}/Libraries/DLPX-HI"
          REMOTE_PATH "${ARTIFACTORY_URL}/release/DLPX.HI.Models/L16/ED${L16_EDITION}/${DLPX_HI_VERSION}/TnpSettingsL16_L16ED${L16_EDITION}_${DLPX_HI_VERSION}_%TARGET%_%ARCH%_Delivery.zip"
          CREATEDIR)
        list(APPEND ${NAME}_LIBRARIES "${DlpxHiTnpSettingsL16_LIBRARIES}")
      endif(COMPONENT STREQUAL "TnpSettingsL16")

      # DLPX-HI TnpSettingsL22
      if(COMPONENT STREQUAL "TnpSettingsL22")
        define_dependency(
          NAME "DlpxHiTnpSettingsL22"
          DIRNAME "TnpSettingsL22"
          OUTPUT_PATH "${CMAKE_PREFIX_PATH}/Libraries/DLPX-HI"
          REMOTE_PATH "${ARTIFACTORY_URL}/release/DLPX.HI.Models/L22/ED${L22_EDITION}/${DLPX_HI_VERSION}/TnpSettingsL22_L22ED${L22_EDITION}_${DLPX_HI_VERSION}_%TARGET%_%ARCH%_Delivery.zip"
          CREATEDIR)
        list(APPEND ${NAME}_LIBRARIES "${DlpxHiTnpSettingsL22_LIBRARIES}")
      endif(COMPONENT STREQUAL "TnpSettingsL22")

      # DLPX-HI TnpSurvL11
      if(COMPONENT STREQUAL "TnpSurvL11")
        define_dependency(
          NAME "DlpxHiTnpSurvL11"
          DIRNAME "TnpSurvL11"
          OUTPUT_PATH "${CMAKE_PREFIX_PATH}/Libraries/DLPX-HI"
          REMOTE_PATH "${ARTIFACTORY_URL}/release/DLPX.HI.Models/L11/ED${L11_EDITION}/${DLPX_HI_VERSION}/TnpSurvL11_L11ED${L11_EDITION}_${DLPX_HI_VERSION}_%TARGET%_%ARCH%_Delivery.zip"
          CREATEDIR)
        list(APPEND ${NAME}_LIBRARIES "${DlpxHiTnpSurvL11_LIBRARIES}")
      endif(COMPONENT STREQUAL "TnpSurvL11")

      # DLPX-HI TnpSurvL16
      if(COMPONENT STREQUAL "TnpSurvL16")
        define_dependency(
          NAME "DlpxHiTnpSurvL16"
          DIRNAME "TnpSurvL16"
          OUTPUT_PATH "${CMAKE_PREFIX_PATH}/Libraries/DLPX-HI"
          REMOTE_PATH "${ARTIFACTORY_URL}/release/DLPX.HI.Models/L16/ED${L16_EDITION}/${DLPX_HI_VERSION}/TnpSurvL16_L16ED${L16_EDITION}_${DLPX_HI_VERSION}_%TARGET%_%ARCH%_Delivery.zip"
          CREATEDIR)
        list(APPEND ${NAME}_LIBRARIES "${DlpxHiTnpSurvL16_LIBRARIES}")
      endif(COMPONENT STREQUAL "TnpSurvL16")

      # DLPX-HI TnpSurvL22
      if(COMPONENT STREQUAL "TnpSurvL22")
        define_dependency(
          NAME "DlpxHiTnpSurvL22"
          DIRNAME "TnpSurvL22"
          OUTPUT_PATH "${CMAKE_PREFIX_PATH}/Libraries/DLPX-HI"
          REMOTE_PATH "${ARTIFACTORY_URL}/release/DLPX.HI.Models/L22/ED${L22_EDITION}/${DLPX_HI_VERSION}/TnpSurvL22_L22ED${L22_EDITION}_${DLPX_HI_VERSION}_%TARGET%_%ARCH%_Delivery.zip"
          CREATEDIR)
        list(APPEND ${NAME}_LIBRARIES "${DlpxHiTnpSurvL22_LIBRARIES}")
      endif(COMPONENT STREQUAL "TnpSurvL22")

      # DLPX-HI ZTA
      if(COMPONENT STREQUAL "ZTA")
        define_dependency(
          NAME "DlpxHiZTA"
          DIRNAME "ZTA"
          OUTPUT_PATH "${CMAKE_PREFIX_PATH}/Libraries/DLPX-HI"
          REMOTE_PATH "${ARTIFACTORY_URL}/release/DLPX.HI.Models/L11/ED${L11_EDITION}/${DLPX_HI_VERSION}/ZTA_L11ED${L11_EDITION}_${DLPX_HI_VERSION}_%TARGET%_%ARCH%_Delivery.zip"
          CREATEDIR)
        list(APPEND ${NAME}_LIBRARIES "${DlpxHiZTA_LIBRARIES}")
      endif(COMPONENT STREQUAL "ZTA")

      # DLPX-HI CODX
      if(COMPONENT STREQUAL "CODX")
        define_dependency(
          NAME "DlpxHiCODX"
          DIRNAME "DlpxHiCODX"
          OUTPUT_PATH "${CMAKE_PREFIX_PATH}/Libraries/DLPX-HI"
          REMOTE_PATH "${ARTIFACTORY_URL}/release/DLPX.HI.CODX.LOC/ED${L16_EDITION}/${DLPX_HI_CODX_VERSION}/DlpxHiCodx_LOC_ED${L16_EDITION}_${DLPX_HI_CODX_VERSION}_%TARGET%_%ARCH%_Delivery.zip"
          FIND_PATH "${CMAKE_PREFIX_PATH}/Libraries/DLPX-HI/DlpxHiCODX"
          CREATEDIR)
        list(APPEND ${NAME}_LIBRARIES "${DlpxHiCODX_LIBRARIES}")
      endif(COMPONENT STREQUAL "CODX")

    endforeach(COMPONENT IN LISTS COMPONENTS)

  endif(NAME STREQUAL "DlpxHi")

  list(SORT ${NAME}_LIBRARIES)
  list(REMOVE_DUPLICATES ${NAME}_LIBRARIES)
  set(${NAME}_LIBRARIES ${${NAME}_LIBRARIES} PARENT_SCOPE)

endfunction(get_dependency_components_dlpxpi NAME)
