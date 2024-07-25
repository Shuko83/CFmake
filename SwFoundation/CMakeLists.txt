cstoolkit_add_target(SwFoundation SHARED NAMESPACE Streamwork EXTENSION swdl RECURSIVE RECURSIVE_INCLUDE
    PUBLIC_LINK_LIBRARIES
		QstModules::QsException
		Streamwork::SwCore
		Streamwork::SwExecution
		Streamwork::SwServiceShortcut
		Qt5::Core
		Qt5::Gui
		Qt5::Xml
)
