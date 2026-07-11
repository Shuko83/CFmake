# Simple static library
cstoolkit_add_target(MyQtExe EXECUTABLE RECURSIVE
    PRIVATE_LINK_LIBRARIES MyQtLib Qt6::Gui
    PLUGINS MyLib
)
