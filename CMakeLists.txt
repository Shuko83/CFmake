cmake_minimum_required(VERSION 3.21)

project(tt4-sw-cmake)

if(PROJECT_IS_TOP_LEVEL)
    include("${CMAKE_SOURCE_DIR}/cstoolkit/CSToolkitStarlinx.cmake")

	cstoolkit_fetch_artifactory("QstBase" URL "http://10.20.1.10:8081/artifactory/snapshot/Libraries/QstBase/beta-cmake/QstBase_beta-cmake_win-msvc2015-x64.7z")
	cstoolkit_fetch_artifactory("QstModules" URL "http://10.20.1.10:8081/artifactory/snapshot/Libraries/QstModules/beta-cmake/QstModules_beta-cmake_win-msvc2015-x64.7z")
	cstoolkit_fetch_artifactory("Cryptopp" URL "http://10.20.1.10:8081/artifactory/thirdParty/Cryptopp/5.6.5-2/Cryptopp_5.6.5-2_win-msvc2015-x64.7z")
	cstoolkit_fetch_artifactory("License" URL "http://10.20.1.10:8081/artifactory/snapshot/Libraries/License/beta-cmake/License_beta-cmake_win-msvc2015-x64.7z")
	
	cstoolkit_fetch_artifactory("ApiLicensing" URL "http://10.20.1.10:8081/artifactory/release/Libraries/ApiLicensing/5.2.0/ApiLicensing_5.2.0_win-msvc2015-x64.7z")


    add_subdirectory(SwCore)
    add_subdirectory(SwExecution)
    add_subdirectory(SwFoundation)
    add_subdirectory(Services/SwServiceMainWindow)
    add_subdirectory(SwGui)
    add_subdirectory(Services/SwServiceConfiguration)
    add_subdirectory(Services/SwServiceShortcut)
    add_subdirectory(Services/SwServiceTools)
    add_subdirectory(SwEditor)
    add_subdirectory(SwModel)
    add_subdirectory(StreamWorkEditor)
    add_subdirectory(SwConfiguration)
    add_subdirectory(SwDataBase)
    add_subdirectory(SwLauncher)
    add_subdirectory(SwRecord)
endif()
