rem script helps create installer

rem detect windows version
ver | find "6.1" > nul

IF ERRORLEVEL = 1 GOTO ARCHITECTURE
IF ERRORLEVEL = 0 GOTO WIN7

:WIN7
chcp 65001

:ARCHITECTURE
rem find target architecture
reg Query "HKLM\Hardware\Description\System\CentralProcessor\0" | find /i "x86" > NUL && set ARCHITECTURE=32BIT || set ARCHITECTURE=64BIT

rem Path to Inno Setup according to architecture
if %ARCHITECTURE%==32BIT set nsis_path="C:/Program Files/Inno Setup 5/iscc.exe"
if %ARCHITECTURE%==64BIT set nsis_path="C:/Program Files (x86)/Inno Setup 5/iscc.exe"

if not exist %nsis_path% (
	SET package_error="Package was not created!"
	SET /P promt="Coudn't find Inno Setup. Do you want to continue?[Y\N]"
	IF "%promt%" == "Y" GOTO PREPARE
	IF "%promt%" == "y" GOTO PREPARE
	ELSE GOTO ONEXIT
) 

:PREPARE
cd ..
cd
rem force qmake create new qm files
del /Q share\translations\*.qm
IF exist build ( 
	echo Build exists. Clearing.
	rd /s /q build\package
	del /s /q /f build\Makefile
	del /s /q /f build\*.exe
	del /s /q /f build\*.dll
) 
mkdir build && echo build created
cd build
cd

qmake -r CONFIG+=noTests ..\Valentina.pro
IF ERRORLEVEL 1 GOTO ERRORQMAKE1
IF ERRORLEVEL 0 GOTO MAKE

:MAKE
mingw32-make -j%NUMBER_OF_PROCESSORS%
IF ERRORLEVEL 1 GOTO ERRORMAKE
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
echo Done! %package_error%
@pause

