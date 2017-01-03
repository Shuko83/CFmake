cd %~dp0
set v="2.0"

rmdir /s /q %v%
mkdir %v%
cd %v%
mkdir msvc2008
cd ..



rem recupérer tous les fichiers à compresser dans le setup
xcopy /E /R /S ..\bin\vc\debug .\%v%\msvc2008\bin\debug\
xcopy /E /R /S ..\bin\vc\release .\%v%\msvc2008\bin\release\
xcopy /E /R /S ..\doc .\%v%\doc\
xcopy /E /R /S ..\src .\%v%\src\

REM nettoyer les sources
cd .\%v%\src\
for /f "delims=" %%a In ('dir /b  "." ') Do (
		rmdir /s /q %%a\GeneratedFiles
		rmdir /s /q %%a\debug
		rmdir /s /q %%a\release
		del /f /q %%a\*.vc*proj*
)

cd SwService\

for /f "delims=" %%a In ('dir /b  "." ') Do (
		rmdir /s /q %%a\GeneratedFiles
		rmdir /s /q %%a\debug
		rmdir /s /q %%a\release
		del /f /q %%a\*.vc*proj*
)

REM récupération de l'heure et formatage sans ":" et 0 à la place de l'espace quand heure avant 10H00
set producttime=%time:~0,-3%
set producttime=%producttime::=%
set producttime=%producttime: =0%

cd ..\..\..

"c:\Program Files (x86)\NSIS\makensis.exe" /DPRODUCT_VERSION=%v% /DPRODUCT_BUILD=0 /DPRODUCT_DATE=%date:/=%_%producttime% project.nsi  