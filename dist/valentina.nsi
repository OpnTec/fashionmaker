; -------------------------------
; Start
 
 
  !define MUI_PRODUCT "Valentina"
  !define MUI_FILE "valentina"
  !define MUI_VERSION "0.2.2"
  !define MUI_BRANDINGTEXT "Valentina ${MUI_VERSION}"
  CRCCheck On
 
  ; Bij deze moeten we waarschijnlijk een absoluut pad gaan gebruiken
  ; dit moet effe uitgetest worden.
  !include "${NSISDIR}\Contrib\Modern UI\System.nsh"
 
 
;--------------------------------
;General
  Name "${MUI_BRANDINGTEXT}"
  Caption "${MUI_BRANDINGTEXT}"
  SetCompressor bzip2
  OutFile "${MUI_FILE}-install-v.${MUI_VERSION}.exe"
  
  ; Request application privileges for Windows Vista
  RequestExecutionLevel user
  
  ShowInstDetails show
  ShowUninstDetails show
  
  !define MUI_ICON "valentina\${MUI_FILE}.ico"
  !define MUI_UNICON "valentina\${MUI_FILE}.ico"
  ;!define MUI_SPECIALBITMAP "Bitmap.bmp"
  
;--------------------------------
;Folder selection page
 
InstallDir "$PROGRAMFILES\${MUI_PRODUCT}"
 
 
;--------------------------------
;Modern UI Configuration

  InstallColors 061C79 E5F0E2
  LicenseBkColor E5F0E2
  InstProgressFlags smooth colored
  
  !define MUI_WELCOMEPAGE  
  !define MUI_LICENSEPAGE
  !define MUI_DIRECTORYPAGE
  !define MUI_ABORTWARNING
  !define MUI_UNINSTALLER
  !define MUI_UNCONFIRMPAGE
  !define MUI_FINISHPAGE


;--------------------------------
;Page

  !insertmacro MUI_PAGE_WELCOME
  !insertmacro MUI_PAGE_LICENSE "valentina\license"
  !insertmacro MUI_PAGE_COMPONENTS
  !insertmacro MUI_PAGE_DIRECTORY
  !insertmacro MUI_PAGE_INSTFILES
  !insertmacro MUI_PAGE_FINISH

  !insertmacro MUI_UNPAGE_WELCOME
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES
  !insertmacro MUI_UNPAGE_FINISH
 
;--------------------------------
;Language
 
  !insertmacro MUI_LANGUAGE "English"
 
 
;-------------------------------- 
;Modern UI System
 
  ;!insertmacro MUI_SYSTEM 
  !include "MUI2.nsh"
 
;--------------------------------
;Data
 
  LicenseData "valentina\LICENSE"
 
;-------------------------------- 
;Installer Sections     
Section "Valentina  (required)"
 
;Add files
  SetOutPath "$INSTDIR"
  File /r "c:\pack\valentina\*.*"
 
;create desktop shortcut
  CreateShortCut "$DESKTOP\${MUI_PRODUCT}.lnk" "$INSTDIR\${MUI_FILE}.exe" ""
 
;create start-menu items
  CreateDirectory "$SMPROGRAMS\${MUI_PRODUCT}"
  CreateShortCut "$SMPROGRAMS\${MUI_PRODUCT}\Uninstall.lnk" "$INSTDIR\Uninstall.exe" "" "$INSTDIR\Uninstall.exe" 0
  CreateShortCut "$SMPROGRAMS\${MUI_PRODUCT}\${MUI_PRODUCT}.lnk" "$INSTDIR\${MUI_FILE}.exe" "" "$INSTDIR\${MUI_FILE}.exe" 0
 
;write uninstall information to the registry
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${MUI_PRODUCT}" "DisplayName" "${MUI_PRODUCT} (remove only)"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${MUI_PRODUCT}" "UninstallString" "$INSTDIR\Uninstall.exe"
 
  WriteUninstaller "$INSTDIR\Uninstall.exe"
 
SectionEnd 
 
 
;--------------------------------    
;Uninstaller Section  
Section "Uninstall"
 
;Delete Files 
  RMDir /r "$INSTDIR\*.*"    
 
;Remove the installation directory
  RMDir "$INSTDIR"
 
;Delete Start Menu Shortcuts
  Delete "$DESKTOP\${MUI_PRODUCT}.lnk"
  Delete "$SMPROGRAMS\${MUI_PRODUCT}\*.*"
  RmDir  "$SMPROGRAMS\${MUI_PRODUCT}"
 
;Delete Uninstaller And Unistall Registry Entries
  DeleteRegKey HKEY_LOCAL_MACHINE "SOFTWARE\${MUI_PRODUCT}"
  DeleteRegKey HKEY_LOCAL_MACHINE "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\${MUI_PRODUCT}"  
 
SectionEnd
 
 
;--------------------------------    
;MessageBox Section
 
 
;Function that calls a messagebox when installation finished correctly
Function .onInstSuccess
  MessageBox MB_OK "You have successfully installed ${MUI_PRODUCT}. Use the desktop icon to start the program."
FunctionEnd
 
 
Function un.onUninstSuccess
  MessageBox MB_OK "You have successfully uninstalled ${MUI_PRODUCT}."
FunctionEnd
 
 
;eof
