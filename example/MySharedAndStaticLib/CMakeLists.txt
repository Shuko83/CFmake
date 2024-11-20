# Simple shared library with dependencies
cstoolkit_add_target(MySharedAndStaticLib SHARED_AND_STATIC RECURSIVE
	PUBLIC_LINK_LIBRARIES tinyxml2
)
