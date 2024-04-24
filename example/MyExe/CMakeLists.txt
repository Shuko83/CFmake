#Simple executable with some dependencies
cstoolkit_add_target(MyExe EXECUTABLE RECURSIVE
	PRIVATE_LINK_LIBRARIES MyLib #MyCombinedStaticLib
)
