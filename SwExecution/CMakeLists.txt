cstoolkit_add_target(SwExecution SHARED NAMESPACE tt4-sw-cmake EXTENSION swdl RECURSIVE RECURSIVE_INCLUDE
    PUBLIC_LINK_LIBRARIES
		QstModules::QsException
		tt4-sw-cmake::SwCore
		Qt5::Core
		Qt5::Widgets
		Qt5::Xml
)
