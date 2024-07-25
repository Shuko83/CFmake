cstoolkit_add_target(SwRecord SHARED NAMESPACE Streamwork EXTENSION swdl RECURSIVE RECURSIVE_INCLUDE
    PUBLIC_LINK_LIBRARIES
		QstModules::QsException
		Streamwork::SwConfiguration
		Streamwork::SwCore
		Streamwork::SwExecution
		Streamwork::SwFoundation
		Streamwork::SwGui
		Streamwork::SwServiceShortcut
		Qt5::Core
		Qt5::Widgets
		Qt5::Xml
)
