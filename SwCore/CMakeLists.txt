if (WIN32)
	set (WINDOWS_DEPENDENCIES "Psapi imagehlp")
endif()

cstoolkit_add_target(SwCore SHARED NAMESPACE Streamwork RECURSIVE RECURSIVE_INCLUDE
    PUBLIC_LINK_LIBRARIES
		License::License
		QstBase::QsCore
		QstModules::QsException
		QstBase::QsGui
		QstModules::QsLog
		Qt5::Core
		Qt5::Gui
		Qt5::Widgets
		Qt5::Xml
	PRIVATE_LINK_LIBRARIES
		${WINDOWS_DEPENDENCIES}
		ApiLicensing::ApiLicensing
		cryptopp-static
	COMPILE_DEFINITIONS
		UNICODE
		_CRT_SECURE_NO_WARNINGS
)
