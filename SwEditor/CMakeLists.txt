cstoolkit_add_target(SwEditor SHARED NAMESPACE Streamwork EXTENSION swdl RECURSIVE RECURSIVE_INCLUDE
    PUBLIC_LINK_LIBRARIES
		QstModules::QsException
		Streamwork::SwCore
		Streamwork::SwExecution
		Streamwork::SwGui
		Qt5::Core
		Qt5::Gui
		Qt5::Widgets
		Qt5::Xml
)
