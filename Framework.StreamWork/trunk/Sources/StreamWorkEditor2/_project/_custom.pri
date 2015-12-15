VERSION = 1.2.3 

#Console in release
CONFIG( release, debug|release ): CONFIG *= console

greaterThan(QT_MINOR_VERSION, 4): {
	windows: LIBS += opengl32.lib
	linux: LIBS += -lopengl32
}

RC_ICONS = ..\StreamWorkEditor2.ico