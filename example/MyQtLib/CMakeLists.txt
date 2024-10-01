# Simple static library
cstoolkit_add_target(MyQtLib SHARED RECURSIVE
    PUBLIC_LINK_LIBRARIES Qt5::Widgets
)
