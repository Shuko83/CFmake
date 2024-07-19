if (WIN32)
	set(OPENGL_LIB opengl32)
else()
	set(OPENGL_LIB OpenGL)
endif()


cstoolkit_add_target(StreamWorkEditor EXECUTABLE NAMESPACE tt4-sw-cmake RECURSIVE RECURSIVE_INCLUDE
    PUBLIC_LINK_LIBRARIES
		License::License
		QstBase::QsCore
		QstModules::QsException
		QstBase::QsGui
		QstModules::QsLog
		tt4-sw-cmake::SwCore
		tt4-sw-cmake::SwEditor
		tt4-sw-cmake::SwExecution
		tt4-sw-cmake::SwGui
		tt4-sw-cmake::SwModel
		tt4-sw-cmake::SwServiceConfiguration
		tt4-sw-cmake::SwServiceMainWindow
		tt4-sw-cmake::SwServiceShortcut
		tt4-sw-cmake::SwServiceTools
		Qt5::Core
		Qt5::Gui
		Qt5::OpenGL
		Qt5::PrintSupport
		Qt5::Widgets
		Qt5::Xml
	PRIVATE_LINK_LIBRARIES
		${OPENGL_LIB}
		ApiLicensing::ApiLicensing
		cryptopp-static
	LINK_OPTIONS
		-LARGEADDRESSAWARE
)
