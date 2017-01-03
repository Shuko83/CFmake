set PATHSAVE=%PATH%
set PATH=%QTDIR%\bin
qmake -tp vc
set PATH=%PATHSAVE%
