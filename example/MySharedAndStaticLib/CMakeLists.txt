# Simple shared library with dependencies
cfmake_add_target(MySharedAndStaticLib SHARED_AND_STATIC RECURSIVE
	PUBLIC_LINK_LIBRARIES TinyXML2::TinyXML2
)
