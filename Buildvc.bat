@echo generation des vc proj
set QTDIR=C:\Qt\4.7.3
set QMAKESPEC=win32-msvc2008
call "%ProgramFiles(x86)%\Microsoft Visual Studio 9.0\VC\vcvarsall.bat" x86

call MakeVcProjAll.bat

cd %~dp0

for %%A in (Debug Release) DO (msbuild.exe  /l:FileLogger,Microsoft.Build.Engine;logfile=BuildLog_%%A.log /m /t:rebuild /p:Configuration=%%A StreamWork.sln)