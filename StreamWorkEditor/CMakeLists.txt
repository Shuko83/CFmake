if (WIN32)
	set(OPENGL_LIB opengl32)
else()
	set(OPENGL_LIB OpenGL::OpenGL)
endif()

cstoolkit_add_target(StreamWorkEditor EXECUTABLE NAMESPACE Streamwork RECURSIVE RECURSIVE_INCLUDE
    PUBLIC_LINK_LIBRARIES
		License::License
		QstBase::QsCore
		QstModules::QsException
		QstBase::QsGui
		QstModules::QsLog
		Streamwork::SwCore
		Streamwork::SwEditor
		Streamwork::SwExecution
		Streamwork::SwGui
		Streamwork::SwModel
		Streamwork::SwServiceConfiguration
		Streamwork::SwServiceMainWindow
		Streamwork::SwServiceShortcut
		Streamwork::SwServiceTools
		Qt5::Core
		Qt5::CorePrivate
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
	PLUGINS 
		Streamwork::SwEditor
		Streamwork::SwExecution 
		Streamwork::SwFoundation 
		Streamwork::SwGui 
		Streamwork::SwModel 
		Streamwork::SwServiceMainWindow
)
