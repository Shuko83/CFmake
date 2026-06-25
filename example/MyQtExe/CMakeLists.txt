# Simple static library
cfmake_add_target(MyQtExe EXECUTABLE RECURSIVE
    PRIVATE_LINK_LIBRARIES MyQtLib Qt5::Gui
    PLUGINS MyLib
)
