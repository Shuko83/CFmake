@echo generation des vc proj
set QTDIR=C:\Qt\Qt5.5.0\5.5\msvc2013
set QMAKESPEC=win32-msvc2013
call "%ProgramFiles(x86)%\Microsoft Visual Studio 12.0\VC\vcvarsall.bat" x86
qmake -v
call MakeVcProjAll.bat
qmake -v

cd %~dp0

for %%A in (Debug Release) DO (msbuild.exe  /l:FileLogger,Microsoft.Build.Engine;logfile=BuildLog_%%A.log /m /t:rebuild /p:Configuration=%%A %~dp0\src\QsWidgets\QsWidget.vcxproj)

for %%A in (Debug Release) DO (msbuild.exe  /l:FileLogger,Microsoft.Build.Engine;logfile=BuildLog_%%A.log /m /t:rebuild /p:Configuration=%%A %~dp0\src\SwCore\SwCore.vcxproj)

for %%A in (Debug Release) DO (msbuild.exe  /l:FileLogger,Microsoft.Build.Engine;logfile=BuildLog_%%A.log /m /t:rebuild /p:Configuration=%%A %~dp0\src\SwExecution\SwExecution.vcxproj)

for %%A in (Debug Release) DO (msbuild.exe  /l:FileLogger,Microsoft.Build.Engine;logfile=BuildLog_%%A.log /m /t:rebuild /p:Configuration=%%A %~dp0\src\SwFoundation\SwFoundation.vcxproj)

for %%A in (Debug Release) DO (msbuild.exe  /l:FileLogger,Microsoft.Build.Engine;logfile=BuildLog_%%A.log /m /t:rebuild /p:Configuration=%%A %~dp0\src\SwConfiguration\SwConfiguration.vcxproj)

for %%A in (Debug Release) DO (msbuild.exe  /l:FileLogger,Microsoft.Build.Engine;logfile=BuildLog_%%A.log /m /t:rebuild /p:Configuration=%%A %~dp0\src\SwModel2\SwModel2.vcxproj)

for %%A in (Debug Release) DO (msbuild.exe  /l:FileLogger,Microsoft.Build.Engine;logfile=BuildLog_%%A.log /m /t:rebuild /p:Configuration=%%A %~dp0\src\SwDataBase\SwDataBase.vcxproj)

for %%A in (Debug Release) DO (msbuild.exe  /l:FileLogger,Microsoft.Build.Engine;logfile=BuildLog_%%A.log /m /t:rebuild /p:Configuration=%%A %~dp0\src\SwService\ServiceConfiguration\SwServiceConfiguration.vcxproj)

for %%A in (Debug Release) DO (msbuild.exe  /l:FileLogger,Microsoft.Build.Engine;logfile=BuildLog_%%A.log /m /t:rebuild /p:Configuration=%%A %~dp0\src\ServiceServiceMainWindow\SwServiceMainWindow.vcxproj)

for %%A in (Debug Release) DO (msbuild.exe  /l:FileLogger,Microsoft.Build.Engine;logfile=BuildLog_%%A.log /m /t:rebuild /p:Configuration=%%A %~dp0\src\ServiceShortcut\SwServiceShortcut.vcxproj)

for %%A in (Debug Release) DO (msbuild.exe  /l:FileLogger,Microsoft.Build.Engine;logfile=BuildLog_%%A.log /m /t:rebuild /p:Configuration=%%A %~dp0\src\ServiceTools\SwServiceTools.vcxproj)

for %%A in (Debug Release) DO (msbuild.exe  /l:FileLogger,Microsoft.Build.Engine;logfile=BuildLog_%%A.log /m /t:rebuild /p:Configuration=%%A %~dp0\src\SwGui\SwGui.vcxproj)

for %%A in (Debug Release) DO (msbuild.exe  /l:FileLogger,Microsoft.Build.Engine;logfile=BuildLog_%%A.log /m /t:rebuild /p:Configuration=%%A %~dp0\src\SwEditor\SwEditor.vcxproj)

for %%A in (Debug Release) DO (msbuild.exe  /l:FileLogger,Microsoft.Build.Engine;logfile=BuildLog_%%A.log /m /t:rebuild /p:Configuration=%%A %~dp0\src\SwRecord\SwRecord.vcxproj)

for %%A in (Debug Release) DO (msbuild.exe  /l:FileLogger,Microsoft.Build.Engine;logfile=BuildLog_%%A.log /m /t:rebuild /p:Configuration=%%A %~dp0\src\StreamworkEditor2\StreamworkEditor2.vcxproj)