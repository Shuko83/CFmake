#Simple executable with some dependencies
cstoolkit_add_target(MyExe EXECUTABLE RECURSIVE
	PRIVATE_LINK_LIBRARIES MyStaticLib
	PRIVATE_LINK_LIBRARIES MyLib Boost::atomic
)

cstoolkit_deploy_with_target(MyExe DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/deployed/" "src" "$<TARGET_FILE_DIR:MyCombinedStaticLib>" DESTINATION "config")
cstoolkit_deploy_with_target(MyExe FILES "${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt" "../MyLib/include/MyClass.h" "$<TARGET_FILE:MyPlugin>" DESTINATION "byfiles")