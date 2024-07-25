cstoolkit_add_target(SwServiceConfiguration SHARED NAMESPACE Streamwork EXTENSION swdl RECURSIVE RECURSIVE_INCLUDE
    PUBLIC_LINK_LIBRARIES
		QstModules::QsException
		Streamwork::SwCore
		Streamwork::SwFoundation
		Streamwork::SwGui
		Qt5::Core
		Qt5::Widgets
		Qt5::Xml
)
