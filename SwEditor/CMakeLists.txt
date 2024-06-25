cstoolkit_add_target(SwEditor SHARED NAMESPACE tt4-sw-cmake EXTENSION swdl RECURSIVE RECURSIVE_INCLUDE
    PUBLIC_LINK_LIBRARIES
		QstModules::QsException
		tt4-sw-cmake::SwCore
		tt4-sw-cmake::SwExecution
		tt4-sw-cmake::SwGui
		Qt5::Core
		Qt5::Gui
		Qt5::Widgets
		Qt5::Xml
)
