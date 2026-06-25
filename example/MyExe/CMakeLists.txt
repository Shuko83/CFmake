#Simple executable with some dependencies
cfmake_add_target(MyExe EXECUTABLE RECURSIVE
	PRIVATE_LINK_LIBRARIES MyStaticLib
	PRIVATE_LINK_LIBRARIES MyLib
)

cfmake_deploy_with_target(MyExe DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/deployed/" "src" "$<TARGET_FILE_DIR:MyCombinedStaticLib>" DESTINATION "config")
cfmake_deploy_with_target(MyExe FILES "${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt" "../MyLib/include/MyClass.h" "$<TARGET_FILE:MyPlugin>" DESTINATION "demo")