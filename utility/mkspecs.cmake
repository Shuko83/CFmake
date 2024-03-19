set(CSTOOLKIT_BUILD_MKSPECS "win-msvc2015-x64")
set(CSTOOLKIT_BUILD_MKSPECS_QT "win-msvc2015-x64-Qt${QT_VERSION_MM}")

#qt {
#    qtGreaterThan( 5.9.9 ) : BUILD_MKSPEC = $${TOOLKIT.mkspec}-QT$${QT_VERSION_MM}
#    else : BUILD_MKSPEC = $${TOOLKIT.mkspec}-QT$$[QT_VERSION]
#}
#else: BUILD_MKSPEC = $${TOOLKIT.mkspec}
#