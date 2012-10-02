rem xcopy /Y /S .\Dev\bin\vc\debug\jars\* .\Stable\bin\vc\debug\jars\
xcopy /Y .\Dev\bin\vc\debug\*.exe .\Stable\bin\vc\debug\
rem xcopy /Y .\Dev\bin\vc\debug\*.bat .\Stable\bin\vc\debug\
xcopy /Y .\Dev\bin\vc\debug\*.dll .\Stable\bin\vc\debug\
xcopy /Y .\Dev\bin\vc\debug\*.swdld .\Stable\bin\vc\debug\
xcopy /Y .\Dev\bin\vc\debug\*.pdb .\Stable\bin\vc\debug\
xcopy /Y .\Dev\lib\vc\debug\*.lib .\Stable\lib\vc\debug\

rem xcopy /Y /S .\Dev\bin\vc\release\jars\* .\Stable\bin\vc\release\jars\
xcopy /Y .\Dev\bin\vc\release\*.exe .\Stable\bin\vc\release\
xcopy /Y .\Dev\bin\vc\release\*.bat .\Stable\bin\vc\release\
xcopy /Y .\Dev\bin\vc\release\*.dll .\Stable\bin\vc\release\
xcopy /Y .\Dev\bin\vc\release\*.swdl .\Stable\bin\vc\release\
xcopy /Y .\Dev\lib\vc\release\*.lib .\Stable\lib\vc\release\

xcopy /Y /S  .\Dev\src\*.h .\Stable\src\
cd .\Stable\src\
del /F /S /Q  _*.h
for /f "tokens=* delims= " %%a in ('dir/b/s/ad _*') do (
rd /S/Q %%a
)
cd ..
