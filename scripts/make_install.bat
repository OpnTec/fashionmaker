rem script helps create installer

rem find target architecture
IF "%PROCESSOR_ARCHITECTURE%"=="x86" (set ARCHITECTURE=32BIT) else (set ARCHITECTURE=64BIT)

rem Path to Inno Setup according to architecture
if %ARCHITECTURE%==32BIT set nsis_path="C:/Program Files/Inno Setup 5/iscc.exe"
if %ARCHITECTURE%==64BIT set nsis_path="C:/Program Files (x86)/Inno Setup 5/iscc.exe"

if not exist %nsis_path% (
  echo Coudn't find Inno Setup. Package will not be created.
)

:CONTINUE
rem detect windows version
ver | find "6.1" > nul

IF ERRORLEVEL = 1 GOTO PREPARE
IF ERRORLEVEL = 0 GOTO WIN7

:WIN7
chcp 65001 

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
	del /q /f build\src\app\tape\obj\dialogabouttape.o
	del /q /f build\src\app\valentina\obj\dialogaboutapp.o
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
echo Done!
@pause

