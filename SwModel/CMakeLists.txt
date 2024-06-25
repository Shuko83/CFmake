cstoolkit_add_target(SwModel SHARED NAMESPACE tt4-sw-cmake EXTENSION swdl RECURSIVE RECURSIVE_INCLUDE
    PUBLIC_LINK_LIBRARIES
		QstModules::QsException
		tt4-sw-cmake::SwCore
		tt4-sw-cmake::SwExecution
		Qt5::Core
		Qt5::Widgets
		Qt5::Xml
)
