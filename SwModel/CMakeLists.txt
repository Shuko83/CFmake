cstoolkit_add_target(SwModel SHARED NAMESPACE Streamwork EXTENSION swdl RECURSIVE RECURSIVE_INCLUDE
    PUBLIC_LINK_LIBRARIES
		QstModules::QsException
		Streamwork::SwCore
		Streamwork::SwExecution
		Qt5::Core
		Qt5::Widgets
		Qt5::Xml
)
