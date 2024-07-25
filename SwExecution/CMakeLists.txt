cstoolkit_add_target(SwExecution SHARED NAMESPACE Streamwork EXTENSION swdl RECURSIVE RECURSIVE_INCLUDE
    PUBLIC_LINK_LIBRARIES
		QstModules::QsException
		Streamwork::SwCore
		Qt5::Core
		Qt5::Widgets
		Qt5::Xml
)
