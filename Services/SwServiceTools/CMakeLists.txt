cstoolkit_add_target(SwServiceTools SHARED NAMESPACE Streamwork EXTENSION swdl RECURSIVE RECURSIVE_INCLUDE
    PUBLIC_LINK_LIBRARIES
		QstModules::QsException
		Streamwork::SwCore
		Qt5::Core
		Qt5::Gui
)
