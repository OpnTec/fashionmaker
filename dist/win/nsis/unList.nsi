;-----------------------------------------------------------------------------------------------
; Generate list of files and directories for uninstaller with command line support (/? for help)
; Create installer and use like external program that generate list of files.
;
; 2006 Shengalts Aleksander aka Instructor (Shengalts@mail.ru)
;-----------------------------------------------------------------------------------------------
; - added file filter as command option
; 2005 Patrick Graf (patrick.graf@gmx.net)
;-----------------------------------------------------------------------------------------------
; - combine unList and unListDate to one script
; 2005 Matei "Ambient.Impact" Stanca (ambient.impact@rogers.com)
;-----------------------------------------------------------------------------------------------

Name "unList v1.5"
OutFile "unList.exe"
Caption "$(^Name)"
SubCaption 2 " "
SubCaption 3 " "

!include "FileFunc.nsh"
!insertmacro GetTime
!insertmacro GetParameters
!insertmacro GetOptions
!insertmacro GetExePath

!include "TextFunc.nsh"
!insertmacro FileReadFromEnd
!insertmacro FileJoin
!insertmacro TrimNewLines

!include "Sections.nsh"

Var DATE
Var FILEFILTER
Var LOG
Var PREFIX
Var UNDIR_VAR
Var MB
Var RADIOBUTTON

DirText '$\nSetup will generate list of files and directories for uninstaller.'\
	'Source Folder'
Page directory
Page components
Page instfiles

Section "unList" unList
	StrCpy $DATE '0'
	call main
SectionEnd

Section /o "unListDate" unListDate
	StrCpy $DATE '1'
	call main
SectionEnd

Function .onSelChange
	!insertmacro StartRadioButtons $RADIOBUTTON
	!insertmacro RadioButton ${unList}
	!insertmacro RadioButton ${unListDate}
	!insertmacro EndRadioButtons
FunctionEnd

Function .onInit
	StrCpy $RADIOBUTTON ${unList}
	${GetExePath} $EXEDIR
	${GetParameters} $R0
	StrCmp $R0 '/?' +2
	StrCmp $R0 '-?' 0 GetOptions

	MessageBox MB_OK \
	  `|   Command line options:$\n\
	   |   $\n\
	   |   /DATE=[0|1]                  Only files not modified after installation are deleted$\n\
	   |                                           default: 0$\n\
	   |   /INSTDIR=[path]          Get file list from this path$\n\
	   |                                           default: [exe directory]$\n\
	   |   /FILTER=[*.*]               Only files matching the given filter are added$\n\
	   |                                           default: *.*$\n\
	   |   /LOG=[file]                    Ouput file list$\n\
	   |                                           default: unList.txt$\n\
	   |   /PREFIX=[text]              Insert text (spaces) at the beginning$\n\
	   |                                           default: empty$\n\
	   |   /UNDIR_VAR=[$$var]     Uninstall directory variable$\n\
	   |                                           default: $$INSTDIR$\n\
	   |   /MB=[1|0]                      Show or not MessageBoxes$\n\
	   |                                           default: 1$\n\
	   $\n\
	   $\n\
	   Example:$\n\
	   unList.exe /DATE=1  /INSTDIR=C:\a  /LOG=C:\a.log  /PREFIX="  "  /UNDIR_VAR=$$R0`
	quit

	GetOptions:
	${GetOptions} '$R0' '/DATE=' $R1
	StrCmp $R1 '' 0 +3
	StrCpy $DATE '0'
	Goto +2
	StrCpy $DATE $R1

	${GetOptions} '$R0' '/INSTDIR=' $R1
	StrCmp $R1 '' 0 +3
	StrCpy $INSTDIR '$EXEDIR'
	goto +2
	StrCpy $INSTDIR $R1

	${GetOptions} '$R0' '/FILTER=' $R1
	StrCmp $R1 '' 0 +3
	StrCpy $FILEFILTER '*.*'
	goto +2
	StrCpy $FILEFILTER $R1

	${GetOptions} '$R0' '/LOG=' $R1
	StrCmp $R1 '' 0 +3
	StrCpy $LOG 'unList.txt'
	goto +2
	StrCpy $LOG $R1

	${GetOptions} '$R0' '/PREFIX=' $R1
	StrCmp $R1 '' 0 +3
	StrCpy $PREFIX ''
	goto +2
	StrCpy $PREFIX $R1

	${GetOptions} '$R0' '/UNDIR_VAR=' $R1
	StrCmp $R1 '' 0 +3
	StrCpy $UNDIR_VAR '$$INSTDIR'
	goto +2
	StrCpy $UNDIR_VAR $R1

	${GetOptions} '$R0' '/MB=' $R1
	StrCmp $R1 '' 0 +3
	StrCpy $MB '1'
	goto +2
	StrCpy $MB $R1

	StrCmp $R0 '' +2
	call main
FunctionEnd

Function main
	ClearErrors
	SearchPath $INSTDIR $INSTDIR
	IfErrors error
	StrLen $R5 $INSTDIR
	IntOp $R5 $R5 + 1

	InitPluginsDir
	GetTempFileName $R1 $PLUGINSDIR
	GetTempFileName $R2 $PLUGINSDIR
	GetTempFileName $R3 $PLUGINSDIR
	ExpandEnvStrings $R0 %COMSPEC%

	nsExec::Exec '"$R0" /C DIR "$INSTDIR\$FILEFILTER" /A-D /B /S /ON>"$R1"'
	FileOpen $R4 $R2 w
	${FileReadFromEnd} '$R1' FilesCallback
	FileClose $R4

	nsExec::Exec '"$R0" /C DIR "$INSTDIR\*.*" /AD /B /S /ON>"$R1"'
	FileOpen $R4 $R3 w
	${FileReadFromEnd} '$R1' DirectoriesCallback
	FileClose $R4

	${FileJoin} '$R2' '$R3' '$R2'
	Delete '$EXEDIR\$LOG'
	Rename $R2 '$EXEDIR\$LOG'
	IfErrors 0 success
	Delete $LOG
	Rename $R2 $LOG
	IfErrors 0 success

	error:
	StrCmp $MB 0 quit
	HideWindow
	MessageBox MB_OK|MB_ICONEXCLAMATION 'error'
	quit

	success:
	StrCmp $MB 0 quit
	HideWindow
	MessageBox MB_OK|MB_ICONINFORMATION '"$LOG" was successfully generated'

	quit:
	quit
FunctionEnd

Function FilesCallback
	System::Call 'user32::OemToChar(t r9, t .r9)'
	${TrimNewLines} '$9' $9

	StrCmp $DATE '0' 0 unListDate
	StrCpy $9 $9 '' $R5
	FileWrite $R4 `$PREFIXDelete "$UNDIR_VAR\$9"$\r$\n`
	goto end

	unListDate:
	${GetTime} '$9' 'M' $0 $1 $2 $3 $4 $5 $6

	StrCpy $3 '$${'
	StrCpy $9 $9 '' $R5
	FileWrite $R4 `$PREFIX$3un.GetTime} "$UNDIR_VAR\$9" "M" $$0 $$1 $$2 $$3 $$4 $$5 $$6$\r$\n`
	FileWrite $R4 `$PREFIXIfErrors +5$\r$\n`
	FileWrite $R4 `$PREFIXStrCmp "$$0/$$1/$$2-$$4:$$5:$$6" "$0/$1/$2-$4:$5:$6" 0 +3$\r$\n`
	FileWrite $R4 `$PREFIXDelete "$UNDIR_VAR\$9"$\r$\n`
	FileWrite $R4 `$PREFIXgoto +2$\r$\n`
	FileWrite $R4 `$PREFIXDetailPrint "Not deleted: $UNDIR_VAR\$9"$\r$\n$\r$\n`

	end:
	Push 0
FunctionEnd

Function DirectoriesCallback
	System::Call 'user32::OemToChar(t r9, t .r9)'
	${TrimNewLines} '$9' $9
	StrCpy $9 $9 '' $R5

	FileWrite $R4 `$PREFIXRMDir "$UNDIR_VAR\$9"$\r$\n`

	Push 0
FunctionEnd
