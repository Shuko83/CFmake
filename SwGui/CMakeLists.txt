cstoolkit_add_target(SwGui SHARED NAMESPACE Streamwork EXTENSION swdl RECURSIVE RECURSIVE_INCLUDE
    PUBLIC_LINK_LIBRARIES
		QstBase::QsCore
		QstModules::QsException
		QstBase::QsGui
		Streamwork::SwCore
		Streamwork::SwExecution
		Streamwork::SwFoundation
		Streamwork::SwServiceMainWindow
		Streamwork::SwServiceShortcut
		Streamwork::SwServiceTools
		Qt5::Core
		Qt5::Designer
		Qt5::Gui
		Qt5::Network
		Qt5::Widgets
		Qt5::Xml
)
