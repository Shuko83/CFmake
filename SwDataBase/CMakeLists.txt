cstoolkit_add_target(SwDataBase SHARED NAMESPACE Streamwork EXTENSION swdl RECURSIVE RECURSIVE_INCLUDE
    PUBLIC_LINK_LIBRARIES
		QstModules::QsException
		Streamwork::SwCore
		Streamwork::SwExecution
		Qt5::Core
		Qt5::Gui
		Qt5::Xml
)
