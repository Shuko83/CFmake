cstoolkit_add_target(SwConfiguration SHARED NAMESPACE tt4-sw-cmake EXTENSION swdl RECURSIVE RECURSIVE_INCLUDE
    PUBLIC_LINK_LIBRARIES
		QstBase::QsCore
		QstModules::QsException
		QstBase::QsGui
		tt4-sw-cmake::SwCore
		tt4-sw-cmake::SwExecution
		tt4-sw-cmake::SwGui
		tt4-sw-cmake::SwServiceConfiguration
		Qt5::Core
		Qt5::Gui
		Qt5::Widgets
		Qt5::Xml
)
