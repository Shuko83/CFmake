#Simple executable with some dependencies
cfmake_add_target(gallery EXECUTABLE
	PRIVATE_LINK_LIBRARIES Qt5::Qml Qt5::QuickControls2
	SOURCES_FILES "src/gallery.cpp"
	RESOURCES_FILES "resources/gallery.qrc"
)

#cfmake_deploy_with_target(MyExe DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/deployed/" "src" "$<TARGET_FILE_DIR:MyCombinedStaticLib>" DESTINATION "config")
#cfmake_deploy_with_target(MyExe FILES "${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt" "../MyLib/include/MyClass.h" "$<TARGET_FILE:MyPlugin>" DESTINATION "byfiles")