# Simple shared library with dependencies
cstoolkit_add_target(MyLib SHARED RECURSIVE RECURSIVE_INTERFACE_INCLUDE
	PUBLIC_LINK_LIBRARIES TinyXML2::TinyXML2
)
