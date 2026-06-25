# Simple shared library with custom extension
cfmake_add_target(MyPlugin SHARED RECURSIVE
    EXTENSION "myplgn"
	PUBLIC_LINK_LIBRARIES MyCombinedStaticLib MyProject::MyLib
)
