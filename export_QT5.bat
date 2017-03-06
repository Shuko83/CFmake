xcopy /Y %QTDIR%\bin\Qt5Core.dll %~dp0\bin\vc\release_QT5
xcopy /Y %QTDIR%\bin\Qt5Gui.dll %~dp0\bin\vc\release_QT5
xcopy /Y %QTDIR%\bin\Qt5Network.dll %~dp0\bin\vc\release_QT5
xcopy /Y %QTDIR%\bin\Qt5Widgets.dll %~dp0\bin\vc\release_QT5
xcopy /Y %QTDIR%\bin\Qt5Xml.dll %~dp0\bin\vc\release_QT5

xcopy /Y %QTDIR%\bin\Qt5Cored.dll %~dp0\bin\vc\debug_QT5
xcopy /Y %QTDIR%\bin\Qt5Guid.dll %~dp0\bin\vc\debug_QT5
xcopy /Y %QTDIR%\bin\Qt5Networkd.dll %~dp0\bin\vc\debug_QT5
xcopy /Y %QTDIR%\bin\Qt5Widgetsd.dll %~dp0\bin\vc\debug_QT5
xcopy /Y %QTDIR%\bin\Qt5Xmld.dll %~dp0\bin\vc\debug_QT5

xcopy /Y %QTDIR%\bin\Qt5Cored.pdb %~dp0\bin\vc\debug_QT5
xcopy /Y %QTDIR%\bin\Qt5Guid.pdb %~dp0\bin\vc\debug_QT5
xcopy /Y %QTDIR%\bin\Qt5Networkd.pdb %~dp0\bin\vc\debug_QT5
xcopy /Y %QTDIR%\bin\Qt5Widgetsd.pdb %~dp0\bin\vc\debug_QT5
xcopy /Y %QTDIR%\bin\Qt5Xmld.pdb %~dp0\bin\vc\debug_QT5

xcopy /Y %~dp0\..\Qst\bin\vc\debug_QT5\* %~dp0\bin\vc\debug_QT5
xcopy /Y %~dp0\..\Qst\bin\vc\release_QT5\* %~dp0\bin\vc\release_QT5