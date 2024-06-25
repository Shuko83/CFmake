VERSION = 1.2.3 

#Console in debug and release
CONFIG( debug, debug|release ): CONFIG *= console
CONFIG( release, debug|release ): CONFIG *= console

greaterThan(QT_MINOR_VERSION, 4): {
	windows: LIBS += opengl32.lib
	linux: LIBS += -lopengl32
}

QMAKE_LFLAGS += /LARGEADDRESSAWARE

RC_ICONS = ..\StreamWorkEditor.ico

LIBS += opengl32.lib