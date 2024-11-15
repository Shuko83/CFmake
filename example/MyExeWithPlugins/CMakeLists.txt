# Simple executable with some dependencies and Plugins
# Plugins are dynamic libraries needed for execution but not for compilation
cstoolkit_add_target(MyExeWithPlugins EXECUTABLE RECURSIVE
	PRIVATE_LINK_LIBRARIES MyStaticLib
	PRIVATE_LINK_LIBRARIES MyLib
	PLUGINS MyPlugin DESTINATION "a"
	PLUGINS MyQtLib DESTINATION "dest"
	PLUGINS Boost::chrono DESTINATION "dest/dest2"
	PLUGINS Boost::filesystem DESTINATION "files"
	PLUGINS Boost::atomic DESTINATION "c"
)

cstoolkit_streamwork_generate_devpaths(MyExeWithPlugins OUTPUT $<CONFIG>DevPaths.xml)
