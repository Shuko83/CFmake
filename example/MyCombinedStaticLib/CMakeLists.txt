# Simple static library with combined dependencies
# Combined dependecies are included in the final artifact
cfmake_add_target(MyCombinedStaticLib STATIC RECURSIVE
	COMBINED_LINK_LIBRARIES MyStaticLib
)
