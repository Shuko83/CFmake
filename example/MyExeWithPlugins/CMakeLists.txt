# Simple executable with some dependencies and Plugins
# Plugins are dynamic libraries needed for execution but not for compilation
cfmake_add_target(MyExeWithPlugins EXECUTABLE RECURSIVE
	PRIVATE_LINK_LIBRARIES MyStaticLib
	PLUGINS MyPlugin DESTINATION "a"
	PLUGINS MyQtLib DESTINATION "b"
	PLUGINS Boost::timer DESTINATION "x"
	PLUGINS Boost::thread DESTINATION "y"
	PLUGINS Boost::log DESTINATION "z"
)

cfmake_streamwork_generate_devpaths(MyExeWithPlugins OUTPUT $<CONFIG>DevPaths.xml)

file(TOUCH "${CMAKE_CURRENT_BINARY_DIR}/deploy.stamp.depend")

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/deploy.stamp"
	COMMAND ${CMAKE_COMMAND} -E rename "${CMAKE_CURRENT_BINARY_DIR}/deploy.stamp.depend" "${CMAKE_CURRENT_BINARY_DIR}/deploy.stamp"
	COMMAND ${CMAKE_COMMAND} -E touch "${CMAKE_CURRENT_BINARY_DIR}/deploy.stamp.depend"
	DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/deploy.stamp.depend"
	COMMENT "Force Post-build event"
	COMMAND_EXPAND_LISTS
	USES_TERMINAL
)

target_sources(MyExeWithPlugins PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/deploy.stamp")
source_group("CMake Rules" FILES "${CMAKE_CURRENT_BINARY_DIR}/deploy.stamp")
