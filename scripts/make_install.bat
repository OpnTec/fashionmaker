rem script helps create installer

rem Path to NSIS
set nsis_path="C:\Program Files\NSIS\makensisw.exe"
if not exist %nsis_path% (
	SET /P promt="Coudn't find NSIS. Do you want to continue?[Y\N]"
	IF "%promt%" == "Y" GOTO PREPARE
	IF "%promt%" == "y" GOTO PREPARE
	ELSE GOTO ONEXIT
) 

:PREPARE
cd ..
cd
mkdir build
cd build
cd

qmake -r ..\Valentina.pro
IF ERRORLEVEL 1 GOTO ERRORQMAKE1
IF ERRORLEVEL 0 GOTO MAKE

:MAKE
mingw32-make -j%NUMBER_OF_PROCESSORS%
IF ERRORLEVEL 1 GOTO ERRORMAKE
IF ERRORLEVEL 0 GOTO QMAKE2

:QMAKE2
rem Run second time because first time qmake didn't find all files. They will be available only after build.
qmake -r ..\Valentina.pro
IF ERRORLEVEL 1 GOTO ERRORQMAKE2
IF ERRORLEVEL 0 GOTO MAKEINSTALL

:MAKEINSTALL
mingw32-make install
IF ERRORLEVEL 1 GOTO ERRORMAKEINSTALL
IF ERRORLEVEL 0 GOTO ONEXIT

:ERRORMAKEINSTALL
echo Failed to create installer!
@pause
exit /b 1
:ERRORMAKE
echo Failed to build project!
@pause
exit /b 1
:ERRORQMAKE2
echo Failed to make the second run qmake!
@pause
exit /b 1
:ERRORQMAKE1
echo Failed to make the first run qmake!
@pause
exit /b 1 
:ONEXIT
echo Done!
@pause