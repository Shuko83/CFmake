cstoolkit_add_target(SwGui SHARED NAMESPACE tt4-sw-cmake EXTENSION swdl RECURSIVE RECURSIVE_INCLUDE
    PUBLIC_LINK_LIBRARIES
		QstBase::QsCore
		QstModules::QsException
		QstBase::QsGui
		tt4-sw-cmake::SwCore
		tt4-sw-cmake::SwExecution
		tt4-sw-cmake::SwFoundation
		tt4-sw-cmake::SwServiceMainWindow
		tt4-sw-cmake::SwServiceShortcut
		tt4-sw-cmake::SwServiceTools
		Qt5::Core
		Qt5::Designer
		Qt5::Gui
		Qt5::Network
		Qt5::Widgets
		Qt5::Xml
)
