cstoolkit_add_target(SwServiceMainWindow SHARED NAMESPACE tt4-sw-cmake EXTENSION swdl RECURSIVE RECURSIVE_INCLUDE
    PUBLIC_LINK_LIBRARIES
		tt4-sw-cmake::SwCore
		Qt5::Core
		Qt5::Widgets
)
