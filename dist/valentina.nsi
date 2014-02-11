; -------------------------------
; Start
 
  !define MUI_PRODUCT "Valentina"
  !define MUI_FILE "valentina"
  !define MUI_VERSION "0.2.2"
  !define MUI_BRANDINGTEXT "Valentina ${MUI_VERSION}"
  !define WEBSITE_LINK "https://bitbucket.org/dismine/valentina"
  !define PUBLISHER "Roman Telezhinsky"
  CRCCheck On
 
  ; Bij deze moeten we waarschijnlijk een absoluut pad gaan gebruiken
  ; dit moet effe uitgetest worden.
  !include "${NSISDIR}\Contrib\Modern UI\System.nsh"
 
;--------------------------------
;General

  Name "${MUI_BRANDINGTEXT}"
  Caption "${MUI_BRANDINGTEXT}"
  SetCompressor bzip2
  OutFile "${MUI_FILE}-install-v.${MUI_VERSION}_32bit.exe"
  
  ; Request application privileges for Windows Vista
  RequestExecutionLevel user
  
  ShowInstDetails show
  ShowUninstDetails show
  
  !define MUI_ICON "valentina\${MUI_FILE}.ico"
  !define MUI_UNICON "valentina\${MUI_FILE}.ico"
  ;!define MUI_SPECIALBITMAP "Bitmap.bmp"
  
;--------------------------------
;Folder selection page

  ;Default installation folder
  InstallDir "$LOCALAPPDATA\${MUI_PRODUCT}"
 
  ;Get installation folder from registry if available
  InstallDirRegKey HKCU "$LOCALAPPDATA\${MUI_PRODUCT}" ""
 
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
;Language Selection Dialog Settings

  ;Remember the installer language
  !define MUI_LANGDLL_REGISTRY_ROOT "HKCU" 
  !define MUI_LANGDLL_REGISTRY_KEY "Software\${MUI_PRODUCT}" 
  !define MUI_LANGDLL_REGISTRY_VALUENAME "Installer Language"
   
;--------------------------------
;Language
 
  !define MUI_LANGDLL_ALLLANGUAGES
  !insertmacro MUI_LANGUAGE "English" ;first language is the default language
  !insertmacro MUI_LANGUAGE "German"
  !insertmacro MUI_LANGUAGE "French"
  !insertmacro MUI_LANGUAGE "Russian"
  !insertmacro MUI_LANGUAGE "Ukrainian"
  !insertmacro MUI_LANGUAGE "Czech"
  !insertmacro MUI_LANGUAGE "Hebrew"
  !insertmacro MUI_RESERVEFILE_LANGDLL

  Function .onInit
  !insertmacro MUI_LANGDLL_DISPLAY ;This has to come after the language macros
  FunctionEnd

;--------------------------------
;Reserve Files
  
  ;If you are using solid compression, files that are required before
  ;the actual installation should be stored first in the data block,
  ;because this will make your installer start faster.
  
  !insertmacro MUI_RESERVEFILE_LANGDLL  
 
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

;create start-menu items  
  !define START_LINK_DIR "$STARTMENU\Programs\${MUI_PRODUCT}"
  !define START_LINK_RUN "$STARTMENU\Programs\${MUI_PRODUCT}\${MUI_PRODUCT}.lnk"
  !define START_LINK_UNINSTALLER "$STARTMENU\Programs\${MUI_PRODUCT}\Uninstall ${MUI_PRODUCT}.lnk"

# In your main installer section...
  SetShellVarContext current
  CreateDirectory "${START_LINK_DIR}"
  CreateShortCut "${START_LINK_RUN}" "$INSTDIR\${MUI_FILE}.exe"
  CreateShortCut "${START_LINK_UNINSTALLER}" "$INSTDIR\Uninstall.exe"
 
;create desktop shortcut
  CreateShortCut "$DESKTOP\${MUI_PRODUCT}.lnk" "$INSTDIR\${MUI_FILE}.exe" ""
 
;write uninstall information to the registry
  !define UNINSTALLER_NAME "Uninstall.exe"
  !define REG_UNINSTALL "Software\Microsoft\Windows\CurrentVersion\Uninstall\${MUI_PRODUCT}"

  WriteRegStr HKCU "${REG_UNINSTALL}" "DisplayName" "${MUI_PRODUCT}"
  WriteRegStr HKCU "${REG_UNINSTALL}" "DisplayIcon" "$\"$INSTDIR\${MUI_FILE}.exe$\""
  WriteRegStr HKCU "${REG_UNINSTALL}" "Publisher" "${PUBLISHER}"
  WriteRegStr HKCU "${REG_UNINSTALL}" "DisplayVersion" "${MUI_VERSION}"
  WriteRegDWord HKCU "${REG_UNINSTALL}" "EstimatedSize" 51,4 ;MB
  WriteRegStr HKCU "${REG_UNINSTALL}" "HelpLink" "${WEBSITE_LINK}"
  WriteRegStr HKCU "${REG_UNINSTALL}" "URLInfoAbout" "${WEBSITE_LINK}"
  WriteRegStr HKCU "${REG_UNINSTALL}" "InstallLocation" "$\"$INSTDIR$\""
  WriteRegStr HKCU "${REG_UNINSTALL}" "InstallSource" "$\"$EXEDIR$\""
  WriteRegDWord HKCU "${REG_UNINSTALL}" "NoModify" 1
  WriteRegDWord HKCU "${REG_UNINSTALL}" "NoRepair" 1
  WriteRegStr HKCU "${REG_UNINSTALL}" "UninstallString" "$\"$INSTDIR\${UNINSTALLER_NAME}$\""
  WriteRegStr HKCU "${REG_UNINSTALL}" "Comments" "Uninstalls ${MUI_PRODUCT}."
 
  WriteUninstaller "$INSTDIR\${UNINSTALLER_NAME}"
 
SectionEnd 
 
 
;--------------------------------    
;Uninstaller Section  
Section "Uninstall"
 
;Delete Files 
  RMDir /r "$INSTDIR\*.*"    
 
;Remove the installation directory
  RMDir "$INSTDIR"
 
;Delete Start Menu Shortcuts
  SetShellVarContext current
  Delete "$DESKTOP\${MUI_PRODUCT}.lnk"
  Delete "${START_LINK_DIR}\*.*"
  RmDir  "${START_LINK_DIR}"
 
;Delete Uninstaller And Unistall Registry Entries
  DeleteRegKey HKCU "SOFTWARE\${MUI_PRODUCT}"
  DeleteRegKey HKCU "${REG_UNINSTALL}"  
 
SectionEnd
 
 
;eof
