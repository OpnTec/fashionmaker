; NSIS installer script for Valentina
; --------------- Headers --------------
!include "MUI2.nsh"
!include "headers\fileassoc.nsh"
!include "headers\fileversion.nsh"
!include "FileFunc.nsh"
  
; --------------- General --------------
CRCCheck force
XPStyle on
SetCompressor /FINAL /SOLID lzma

!insertmacro un.GetTime

!define PACKAGE_FOLDER "valentina"

!define PRODUCT_VALENTINA "Valentina"
!define FILE_VALENTINA "valentina"
!define PRODUCT_TAPE "Tape"
!define FILE_TAPE "tape"

!insertmacro GetPEVersionLocal "${PACKAGE_FOLDER}\${FILE_VALENTINA}.exe" ver
!define MUI_VERSION "${ver_1}.${ver_2}.${ver_3}a" ; Delete "a" before release

!define MUI_BRANDINGTEXT "${PRODUCT_VALENTINA} ${MUI_VERSION}"
!define WEBSITE_LINK "http://www.valentina-project.org/"
!define PUBLISHER "Roman Telezhynskyi"

InstallDir "$LOCALAPPDATA\${PRODUCT_VALENTINA}"                 ; Default installation folder
Name "${MUI_BRANDINGTEXT}"                                ; Name displayed on installer
Icon "${PACKAGE_FOLDER}\${FILE_VALENTINA}.ico"
Caption "${MUI_BRANDINGTEXT}"

!define /date MYTIMESTAMP "%Y%m%d%H%M%S"
OutFile "${FILE_VALENTINA}_${MUI_VERSION}-${MYTIMESTAMP}_i386.exe"  ; Resulting installer filename

InstallDirRegKey HKCU "$LOCALAPPDATA\${PRODUCT_VALENTINA}" ""   ; Get installation folder from registry if available
LicenseData "${PACKAGE_FOLDER}\LICENSE_GPL.txt"
RequestExecutionLevel user                                ; Request application privileges for Windows Vista
  
ShowInstDetails show
ShowUninstDetails show

; ----------- Icon and Bitmap ---------  
!define MUI_ICON "${PACKAGE_FOLDER}\${FILE_VALENTINA}.ico"
!define MUI_UNICON "${PACKAGE_FOLDER}\${FILE_VALENTINA}.ico"
;!define MUI_SPECIALBITMAP "Bitmap.bmp"

; -------------------------------------
!define MUI_ABORTWARNING

;------------- Language Selection Dialog Settings --------------
;Remember the installer language
!define MUI_LANGDLL_REGISTRY_ROOT "HKCU" 
!define MUI_LANGDLL_REGISTRY_KEY "Software\${PRODUCT_VALENTINA}" 
!define MUI_LANGDLL_REGISTRY_VALUENAME "Installer Language"
!define REG_UNINSTALL "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_VALENTINA}"
!define REGISTRY_ROOT "HKCU"

;-------------- Install Pages -------------

!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_LICENSE "${PACKAGE_FOLDER}\LICENSE_GPL.txt"
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
    ; These indented statements modify settings for MUI_PAGE_FINISH
    !define MUI_FINISHPAGE_NOAUTOCLOSE
    !define MUI_FINISHPAGE_RUN "$INSTDIR\${FILE_VALENTINA}.exe"
    !define MUI_FINISHPAGE_RUN_CHECKED
    !define MUI_FINISHPAGE_RUN_TEXT "Launch ${PRODUCT_VALENTINA}"
    !define MUI_FINISHPAGE_SHOWREADME_NOTCHECKED
    !define MUI_FINISHPAGE_SHOWREADME "$INSTDIR\README.txt"
!insertmacro MUI_PAGE_FINISH

;-------------- Uninstall Pages -------------
!insertmacro MUI_UNPAGE_WELCOME
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
	; These indented statements modify settings for MUI_UNPAGE_FINISH
	!define MUI_UNFINISHPAGE_NOAUTOCLOSE
!insertmacro MUI_UNPAGE_FINISH
   
;--------------- Languages ---------------
!define MUI_LANGDLL_ALLLANGUAGES
!insertmacro MUI_LANGUAGE "English" ;first language is the default language
!insertmacro MUI_LANGUAGE "German"
!insertmacro MUI_LANGUAGE "French"
!insertmacro MUI_LANGUAGE "Russian"
!insertmacro MUI_LANGUAGE "Ukrainian"
!insertmacro MUI_LANGUAGE "Czech"
!insertmacro MUI_LANGUAGE "Hebrew"
!insertmacro MUI_LANGUAGE "Italian"
!insertmacro MUI_LANGUAGE "Dutch"
!insertmacro MUI_LANGUAGE "Spanish"
!insertmacro MUI_LANGUAGE "SpanishInternational"
!insertmacro MUI_LANGUAGE "SimpChinese"
!insertmacro MUI_LANGUAGE "TradChinese"
!insertmacro MUI_LANGUAGE "Japanese"
!insertmacro MUI_LANGUAGE "Korean" 
!insertmacro MUI_LANGUAGE "Danish"
!insertmacro MUI_LANGUAGE "Swedish"
!insertmacro MUI_LANGUAGE "Norwegian"
!insertmacro MUI_LANGUAGE "NorwegianNynorsk"
!insertmacro MUI_LANGUAGE "Finnish"
!insertmacro MUI_LANGUAGE "Greek"
!insertmacro MUI_LANGUAGE "Portuguese"
!insertmacro MUI_LANGUAGE "PortugueseBR"
!insertmacro MUI_LANGUAGE "Polish"
!insertmacro MUI_LANGUAGE "Slovak"
!insertmacro MUI_LANGUAGE "Croatian"
!insertmacro MUI_LANGUAGE "Bulgarian"
!insertmacro MUI_LANGUAGE "Hungarian"
!insertmacro MUI_LANGUAGE "Thai"
!insertmacro MUI_LANGUAGE "Romanian"
!insertmacro MUI_LANGUAGE "Latvian"
!insertmacro MUI_LANGUAGE "Macedonian"
!insertmacro MUI_LANGUAGE "Estonian"
!insertmacro MUI_LANGUAGE "Turkish"
!insertmacro MUI_LANGUAGE "Lithuanian"
!insertmacro MUI_LANGUAGE "Slovenian"
!insertmacro MUI_LANGUAGE "Serbian"
!insertmacro MUI_LANGUAGE "SerbianLatin"
!insertmacro MUI_LANGUAGE "Arabic"
!insertmacro MUI_LANGUAGE "Farsi"
!insertmacro MUI_LANGUAGE "Indonesian"
!insertmacro MUI_LANGUAGE "Mongolian"
!insertmacro MUI_LANGUAGE "Luxembourgish"
!insertmacro MUI_LANGUAGE "Albanian"
!insertmacro MUI_LANGUAGE "Breton"
!insertmacro MUI_LANGUAGE "Belarusian"
!insertmacro MUI_LANGUAGE "Icelandic"
!insertmacro MUI_LANGUAGE "Malay"
!insertmacro MUI_LANGUAGE "Bosnian"
!insertmacro MUI_LANGUAGE "Kurdish"
!insertmacro MUI_LANGUAGE "Irish"
!insertmacro MUI_LANGUAGE "Uzbek"
!insertmacro MUI_LANGUAGE "Galician"
!insertmacro MUI_LANGUAGE "Afrikaans"
!insertmacro MUI_LANGUAGE "Catalan"
!insertmacro MUI_LANGUAGE "Esperanto"

; ------------- Reserve Files ---------------------
;If you are using solid compression, files that are required before
;the actual installation should be stored first in the data block,
;because this will make your installer start faster.
!insertmacro MUI_RESERVEFILE_LANGDLL

; ------------- Installer Functions ---------------
Function checkAlreadyInstalled
	; check for already installed instance
	ClearErrors 
	ReadRegStr $R0 "${REGISTRY_ROOT}" "${REG_UNINSTALL}" "UninstallString"
	StrCmp $R0 "" done
	MessageBox MB_OKCANCEL|MB_ICONEXCLAMATION \
	"${PRODUCT_VALENTINA} is already installed. $\n$\nClick `OK` to remove the \
	previous version or `Cancel` to cancel this upgrade." \
	IDOK UnInstall
	Abort
	;Run the uninstaller
	UnInstall:
    ClearErrors
    ExecWait '$R0 _?=$INSTDIR' ;Do not copy the uninstaller to a temp file
    
    ;uninstall.exe is still here
    ;Delete Files 
	RMDir /r "$INSTDIR\*.*" 
    ;Remove the installation directory
    RMDir "$INSTDIR"

    IfErrors no_remove_uninstaller done
      ;You can either use Delete /REBOOTOK in the uninstaller or add some code
      ;here to remove the uninstaller. Use a registry key to check
      ;whether the user has chosen to uninstall. If you are using an uninstaller
      ;components page, make sure all sections are uninstalled.
    no_remove_uninstaller:
		MessageBox MB_OK|MB_ICONSTOP "Error while uninstalling \
		previously installed version. Please uninstall it manually \
		and start the installer again."
		Quit  
    done:
FunctionEnd

Function .onInit
  !insertmacro MUI_LANGDLL_DISPLAY ;This has to come after the language macros
  Call checkAlreadyInstalled
FunctionEnd

;-------------- Uninstaller Functions -------------
Function un.onInit
  !insertmacro MUI_UNGETLANGUAGE
FunctionEnd
 
;-------------- Installer -------------------------
Section 
 
;Add files
SetOutPath "$INSTDIR" ; Set output path to the installation directory.
File /r "${PACKAGE_FOLDER}\*.*"

;create start-menu items  
!define START_LINK_DIR "$SMPROGRAMS\${PRODUCT_VALENTINA}"
!define START_LINK_RUN "$SMPROGRAMS\${PRODUCT_VALENTINA}\${PRODUCT_VALENTINA}.lnk"
!define START_LINK_RUN_TAPE "$SMPROGRAMS\${PRODUCT_VALENTINA}\${PRODUCT_TAPE}.lnk"
!define START_LINK_UNINSTALLER "$SMPROGRAMS\${PRODUCT_VALENTINA}\Uninstall ${PRODUCT_VALENTINA}.lnk"

# In your main installer section...
SetShellVarContext current
CreateDirectory "${START_LINK_DIR}"
CreateShortCut "${START_LINK_RUN}" "$INSTDIR\${FILE_VALENTINA}.exe"
CreateShortCut "${START_LINK_RUN_TAPE}" "$INSTDIR\${FILE_TAPE}.exe"
CreateShortCut "${START_LINK_UNINSTALLER}" "$INSTDIR\Uninstall.exe"
 
;create desktop shortcut
CreateShortCut "$DESKTOP\${PRODUCT_VALENTINA}.lnk" "$INSTDIR\${FILE_VALENTINA}.exe" ""
CreateShortCut "$DESKTOP\${PRODUCT_TAPE}.lnk" "$INSTDIR\${FILE_TAPE}.exe" ""
 
;write uninstall information to the registry
!define UNINSTALLER_NAME "Uninstall.exe"
  
; File associations
; Valentina
!insertmacro APP_ASSOCIATE "val" "valentina.pattern" "text/plain" "${PRODUCT_VALENTINA} pattern file" "$INSTDIR\${FILE_VALENTINA}.exe,0" "Open with ${PRODUCT_VALENTINA}" "$INSTDIR\${FILE_VALENTINA}.exe $\"%1$\""
; Tape
!insertmacro APP_ASSOCIATE "vit" "valentina.IndividualMeasurements" "text/plain" "${PRODUCT_VALENTINA} individual measurement file" "$INSTDIR\measurements.ico" "Open with ${PRODUCT_TAPE}" "$INSTDIR\${FILE_TAPE}.exe $\"%1$\""
!insertmacro APP_ASSOCIATE "vst" "valentina.StandardMeasurements" "text/plain" "${PRODUCT_VALENTINA} standard measurement file" "$INSTDIR\measurements.ico" "Open with ${PRODUCT_TAPE}" "$INSTDIR\${FILE_TAPE}.exe $\"%1$\""
!insertmacro UPDATEFILEASSOC 

WriteRegStr "${REGISTRY_ROOT}" "${REG_UNINSTALL}" "DisplayName" "${PRODUCT_VALENTINA}"
WriteRegStr "${REGISTRY_ROOT}" "${REG_UNINSTALL}" "DisplayIcon" "$\"$INSTDIR\${FILE_VALENTINA}.exe$\""
WriteRegStr "${REGISTRY_ROOT}" "${REG_UNINSTALL}" "Publisher" "${PUBLISHER}"
WriteRegStr "${REGISTRY_ROOT}" "${REG_UNINSTALL}" "DisplayVersion" "${MUI_VERSION}"

; Run GetSize after coping all files
${GetSize} "$INSTDIR" "/S=0K" $0 $1 $2
IntFmt $0 "0x%08X" $0
WriteRegDWord "${REGISTRY_ROOT}" "${REG_UNINSTALL}" "EstimatedSize" "$0"

WriteRegStr "${REGISTRY_ROOT}" "${REG_UNINSTALL}" "HelpLink" "${WEBSITE_LINK}"
WriteRegStr "${REGISTRY_ROOT}" "${REG_UNINSTALL}" "URLInfoAbout" "${WEBSITE_LINK}"
WriteRegStr "${REGISTRY_ROOT}" "${REG_UNINSTALL}" "InstallLocation" "$\"$INSTDIR$\""
WriteRegStr "${REGISTRY_ROOT}" "${REG_UNINSTALL}" "InstallSource" "$\"$EXEDIR$\""
WriteRegDWord "${REGISTRY_ROOT}" "${REG_UNINSTALL}" "NoModify" 1
WriteRegDWord "${REGISTRY_ROOT}" "${REG_UNINSTALL}" "NoRepair" 1
WriteRegStr "${REGISTRY_ROOT}" "${REG_UNINSTALL}" "UninstallString" "$\"$INSTDIR\${UNINSTALLER_NAME}$\""
WriteRegStr "${REGISTRY_ROOT}" "${REG_UNINSTALL}" "Comments" "Uninstalls ${PRODUCT_VALENTINA}."
 
WriteUninstaller "$INSTDIR\${UNINSTALLER_NAME}" ; Location of the uninstaller
 
SectionEnd 

;--------------------------------    
;Uninstaller Section  
Section "Uninstall"
;Generate list and include it in script at compile-time
!execute 'unList.exe /DATE=0 /INSTDIR=${PACKAGE_FOLDER} /LOG=Test.log /PREFIX="	" /UNDIR_VAR=$INSTDIR /MB=0'
;Log contain commands for removing files
!include "Test.log"

;Delete Start Menu Shortcuts
SetShellVarContext current
Delete "$DESKTOP\${PRODUCT_VALENTINA}.lnk"
Delete "$DESKTOP\${PRODUCT_TAPE}.lnk"
Delete "${START_LINK_DIR}\*.*"
RmDir  "${START_LINK_DIR}"

; Removing file associations  
!insertmacro APP_UNASSOCIATE "val" "valentina.pattern"  
!insertmacro APP_UNASSOCIATE "vit" "valentina.IndividualMeasurements"  
!insertmacro APP_UNASSOCIATE "vst" "valentina.StandardMeasurements"  
 
;Delete Uninstaller And Unistall Registry Entries
DeleteRegKey "${REGISTRY_ROOT}" "SOFTWARE\${PRODUCT_VALENTINA}"
DeleteRegKey "${REGISTRY_ROOT}" "${REG_UNINSTALL}"     
 
Delete "$INSTDIR\${UNINSTALLER_NAME}"
 
;Remove the installation directory
RMDir "$INSTDIR"
 
SectionEnd
 
;eof
