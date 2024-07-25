cstoolkit_add_target(SwServiceMainWindow SHARED NAMESPACE Streamwork EXTENSION swdl RECURSIVE RECURSIVE_INCLUDE
    PUBLIC_LINK_LIBRARIES
		Streamwork::SwCore
		Qt5::Core
		Qt5::Widgets
)
