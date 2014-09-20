; -------------------------------
; Start
 
  !define MUI_PRODUCT "Valentina"
  !define MUI_FILE "valentina"
  !define MUI_VERSION "0.2.8-alpha"
  !define MUI_BRANDINGTEXT "Valentina ${MUI_VERSION}"
  !define WEBSITE_LINK "http://www.valentina-project.org/"
  !define PUBLISHER "Roman Telezhynskyi"
  CRCCheck On
 
  ; Bij deze moeten we waarschijnlijk een absoluut pad gaan gebruiken
  ; dit moet effe uitgetest worden.
  !include "${NSISDIR}\Contrib\Modern UI\System.nsh"
  !include "FileAssociation.nsh"
 
;--------------------------------
;General

  Name "${MUI_BRANDINGTEXT}"
  Icon "valentina\${MUI_FILE}.ico"
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
  !define REG_UNINSTALL "Software\Microsoft\Windows\CurrentVersion\Uninstall\${MUI_PRODUCT}"
  !define REGISTRY_ROOT "HKCU"
   
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
  !insertmacro MUI_LANGUAGE "Italian"
  !insertmacro MUI_LANGUAGE "Dutch"
  
;  !insertmacro MUI_LANGUAGE "Spanish"
;  !insertmacro MUI_LANGUAGE "SpanishInternational"
;  !insertmacro MUI_LANGUAGE "SimpChinese"
;  !insertmacro MUI_LANGUAGE "TradChinese"
;  !insertmacro MUI_LANGUAGE "Japanese"
;  !insertmacro MUI_LANGUAGE "Korean" 
;  !insertmacro MUI_LANGUAGE "Danish"
;  !insertmacro MUI_LANGUAGE "Swedish"
;  !insertmacro MUI_LANGUAGE "Norwegian"
;  !insertmacro MUI_LANGUAGE "NorwegianNynorsk"
;  !insertmacro MUI_LANGUAGE "Finnish"
;  !insertmacro MUI_LANGUAGE "Greek"
;  !insertmacro MUI_LANGUAGE "Portuguese"
;  !insertmacro MUI_LANGUAGE "PortugueseBR"
;  !insertmacro MUI_LANGUAGE "Polish"
;  !insertmacro MUI_LANGUAGE "Slovak"
;  !insertmacro MUI_LANGUAGE "Croatian"
;  !insertmacro MUI_LANGUAGE "Bulgarian"
;  !insertmacro MUI_LANGUAGE "Hungarian"
;  !insertmacro MUI_LANGUAGE "Thai"
;  !insertmacro MUI_LANGUAGE "Romanian"
;  !insertmacro MUI_LANGUAGE "Latvian"
;  !insertmacro MUI_LANGUAGE "Macedonian"
;  !insertmacro MUI_LANGUAGE "Estonian"
;  !insertmacro MUI_LANGUAGE "Turkish"
;  !insertmacro MUI_LANGUAGE "Lithuanian"
;  !insertmacro MUI_LANGUAGE "Slovenian"
;  !insertmacro MUI_LANGUAGE "Serbian"
;  !insertmacro MUI_LANGUAGE "SerbianLatin"
;  !insertmacro MUI_LANGUAGE "Arabic"
;  !insertmacro MUI_LANGUAGE "Farsi"
;  !insertmacro MUI_LANGUAGE "Indonesian"
;  !insertmacro MUI_LANGUAGE "Mongolian"
;  !insertmacro MUI_LANGUAGE "Luxembourgish"
;  !insertmacro MUI_LANGUAGE "Albanian"
;  !insertmacro MUI_LANGUAGE "Breton"
;  !insertmacro MUI_LANGUAGE "Belarusian"
;  !insertmacro MUI_LANGUAGE "Icelandic"
;  !insertmacro MUI_LANGUAGE "Malay"
;  !insertmacro MUI_LANGUAGE "Bosnian"
;  !insertmacro MUI_LANGUAGE "Kurdish"
;  !insertmacro MUI_LANGUAGE "Irish"
;  !insertmacro MUI_LANGUAGE "Uzbek"
;  !insertmacro MUI_LANGUAGE "Galician"
;  !insertmacro MUI_LANGUAGE "Afrikaans"
;  !insertmacro MUI_LANGUAGE "Catalan"
;  !insertmacro MUI_LANGUAGE "Esperanto"
;  !insertmacro MUI_LANGUAGE "Asturian"
  !insertmacro MUI_RESERVEFILE_LANGDLL

  Function .onInit
  !insertmacro MUI_LANGDLL_DISPLAY ;This has to come after the language macros
  
  ReadRegStr $R0 "${REGISTRY_ROOT}" "${REG_UNINSTALL}" "UninstallString"
  StrCmp $R0 "" done
 
  MessageBox MB_OKCANCEL|MB_ICONEXCLAMATION \
  "${MUI_PRODUCT} is already installed. $\n$\nClick `OK` to remove the \
  previous version or `Cancel` to cancel this upgrade." \
  IDOK uninst
  Abort
 
  ;Run the uninstaller
  uninst:
    ClearErrors
    ExecWait '$R0 _?=$INSTDIR' ;Do not copy the uninstaller to a temp file

    IfErrors no_remove_uninstaller done
      ;You can either use Delete /REBOOTOK in the uninstaller or add some code
      ;here to remove the uninstaller. Use a registry key to check
      ;whether the user has chosen to uninstall. If you are using an uninstaller
      ;components page, make sure all sections are uninstalled.
  no_remove_uninstaller:
 
  done:
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

  WriteRegStr "${REGISTRY_ROOT}" "${REG_UNINSTALL}" "DisplayName" "${MUI_PRODUCT}"
  WriteRegStr "${REGISTRY_ROOT}" "${REG_UNINSTALL}" "DisplayIcon" "$\"$INSTDIR\${MUI_FILE}.exe$\""
  WriteRegStr "${REGISTRY_ROOT}" "${REG_UNINSTALL}" "Publisher" "${PUBLISHER}"
  WriteRegStr "${REGISTRY_ROOT}" "${REG_UNINSTALL}" "DisplayVersion" "${MUI_VERSION}"
  WriteRegDWord "${REGISTRY_ROOT}" "${REG_UNINSTALL}" "EstimatedSize" 51,4 ;MB
  WriteRegStr "${REGISTRY_ROOT}" "${REG_UNINSTALL}" "HelpLink" "${WEBSITE_LINK}"
  WriteRegStr "${REGISTRY_ROOT}" "${REG_UNINSTALL}" "URLInfoAbout" "${WEBSITE_LINK}"
  WriteRegStr "${REGISTRY_ROOT}" "${REG_UNINSTALL}" "InstallLocation" "$\"$INSTDIR$\""
  WriteRegStr "${REGISTRY_ROOT}" "${REG_UNINSTALL}" "InstallSource" "$\"$EXEDIR$\""
  WriteRegDWord "${REGISTRY_ROOT}" "${REG_UNINSTALL}" "NoModify" 1
  WriteRegDWord "${REGISTRY_ROOT}" "${REG_UNINSTALL}" "NoRepair" 1
  WriteRegStr "${REGISTRY_ROOT}" "${REG_UNINSTALL}" "UninstallString" "$\"$INSTDIR\${UNINSTALLER_NAME}$\""
  WriteRegStr "${REGISTRY_ROOT}" "${REG_UNINSTALL}" "Comments" "Uninstalls ${MUI_PRODUCT}."
 
  WriteUninstaller "$INSTDIR\${UNINSTALLER_NAME}"
  
  ${registerExtension} "$INSTDIR\${MUI_FILE}.exe" ".val" "Valentina_File"
 
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
  DeleteRegKey "${REGISTRY_ROOT}" "SOFTWARE\${MUI_PRODUCT}"
  DeleteRegKey "${REGISTRY_ROOT}" "${REG_UNINSTALL}"  
  
  ${unregisterExtension} ".val" "Valentina_File"
 
SectionEnd
 
 
;eof
