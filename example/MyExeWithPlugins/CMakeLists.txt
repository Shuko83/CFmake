# Simple executable with some dependencies and Plugins
# Plugins are dynamic libraries needed for execution but not for compilation
cstoolkit_add_target(MyExeWithPlugins EXECUTABLE RECURSIVE
	PLUGINS MyPlugin
)
