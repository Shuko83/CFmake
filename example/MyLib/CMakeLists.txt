# Simple shared library with dependencies
cstoolkit_add_target(MyLib SHARED RECURSIVE
	PUBLIC_LINK_LIBRARIES tinyxml2
)
