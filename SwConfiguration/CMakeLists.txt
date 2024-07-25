cstoolkit_add_target(SwConfiguration SHARED NAMESPACE Streamwork EXTENSION swdl RECURSIVE RECURSIVE_INCLUDE
    PUBLIC_LINK_LIBRARIES
		QstBase::QsCore
		QstModules::QsException
		QstBase::QsGui
		Streamwork::SwCore
		Streamwork::SwExecution
		Streamwork::SwGui
		Streamwork::SwServiceConfiguration
		Qt5::Core
		Qt5::Gui
		Qt5::Widgets
		Qt5::Xml
)
