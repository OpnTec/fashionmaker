; fileassoc.nsh
; File association helper macros
; Written by Saivert
; Adaptation for Windows Vista and higher by Roman Telezhynskyi
;
; Features automatic backup system and UPDATEFILEASSOC macro for
; shell change notification.
;
; |> How to use <|
; To associate a file with an application so you can double-click it in explorer, use
; the APP_ASSOCIATE macro like this:
;
;   Example:
;   !insertmacro APP_ASSOCIATE "txt" "myapp.textfile" "text/plain" \
; "Description of txt files" "$INSTDIR\myapp.exe,0" "Open with myapp" \ 
; "$INSTDIR\myapp.exe $\"%1$\""
;
; Never insert the APP_ASSOCIATE macro multiple times, it is only ment
; to associate an application with a single file and using the
; the "open" verb as default. To add more verbs (actions) to a file
; use the APP_ASSOCIATE_ADDVERB macro.
;
;   Example:
;   !insertmacro APP_ASSOCIATE_ADDVERB "myapp.textfile" "edit" "text/plain" \
; "Edit with myapp" "$INSTDIR\myapp.exe /edit $\"%1$\""
;
; To have access to more options when registering the file association use the
; APP_ASSOCIATE_EX macro. Here you can specify the verb and what verb is to be the
; standard action (default verb). 
;
; And finally: To remove the association from the registry use the APP_UNASSOCIATE
; macro. Here is another example just to wrap it up:
;   !insertmacro APP_UNASSOCIATE "txt" "myapp.textfile"
;
; |> Note <|
; When defining your file class string always use the short form of your application title
; then a period (dot) and the type of file. This keeps the file class sort of unique.
;   Examples:
;   Winamp.File
;   Winamp.Playlist
;   NSIS.Script
;   Photoshop.JPEGFile
;
; |> Tech info <|
; The registry key layout for a file association is:
;HKEY_LOCAL_MACHINE
;    Software
;        Classes
;            .mp3
;                (default)=Winamp.File
;            Winamp.File
;                (default)=Winamp Media File
;                DefaultIcon
;                    (default)=c:\program files\winamp\winamp.exe,1
;                    shell
;                        Play=&Play in Winamp
;                            command
;                                (default)="c:\program files\winamp\winamp.exe" "%1$"
;                        Open
;                            command     
;                                (default)="c:\program files\winamp\winamp.exe" "%1$"
;                        ListBookmark
;                            (default)=Add to Winamp's &Bookmark list
;                            command
;                                (default)="c:\program files\winamp\winamp.exe"/BOOKMARK "%1$"
;                        Enqueue
;                            (default)=&Enqueue in Winamp
;                            command
;                                (default)="c:\program files\winamp\winamp.exe$"/ADD "%1$"
 
!macro APP_ASSOCIATE EXT FILECLASS MIME DESCRIPTION ICON COMMANDTEXT COMMAND
  ; Backup the previously associated file class
  ReadRegStr $R0 HKCU "Software\Classes\.${EXT}" ""
  WriteRegStr HKCU "Software\Classes\.${EXT}" "${FILECLASS}_backup" "$R0"

  WriteRegStr HKCU "Software\Classes\.${EXT}" "" "${FILECLASS}"
  WriteRegStr HKCU "Software\Classes\.${EXT}" "Content Type" "${MIME}"  
  WriteRegStr HKCU "Software\Classes\.${EXT}\OpenWithProgIds" "${FILECLASS}" ""
 
  WriteRegStr HKCU "Software\Classes\${FILECLASS}" "" "${DESCRIPTION}"
  WriteRegStr HKCU "Software\Classes\${FILECLASS}\DefaultIcon" "" "${ICON}"
  WriteRegStr HKCU "Software\Classes\${FILECLASS}\shell" "" "open"
  WriteRegStr HKCU "Software\Classes\${FILECLASS}\shell\open" "" "${COMMANDTEXT}"
  WriteRegStr HKCU "Software\Classes\${FILECLASS}\shell\open\command" "" "${COMMAND}"
  ; Set application by default for current user (since Windows Vista)
  ; Backup the previously choice
  ReadRegStr $R0 HKCU "Software\Microsoft\Windows\CurrentVersion\Explorer\FileExts\.${EXT}\UserChoice" ""
  WriteRegStr HKCU "Software\Classes\.${EXT}" "UserChoice_backup" "$R0"
  
  DeleteRegKey HKCU "Software\Microsoft\Windows\CurrentVersion\Explorer\FileExts\.${EXT}\UserChoice"
  WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Explorer\FileExts\.${EXT}\UserChoice" "Progid" "${FILECLASS}"
!macroend

!macro APP_ASSOCIATE_EX EXT FILECLASS MIME DESCRIPTION ICON VERB DEFAULTVERB SHELLNEW COMMANDTEXT COMMAND
  ; Backup the previously associated file class
  ReadRegStr $R0 HKCU "Software\Classes\.${EXT}" ""
  WriteRegStr HKCU "Software\Classes\.${EXT}" "${FILECLASS}_backup" "$R0"
 
  WriteRegStr HKCU "Software\Classes\.${EXT}" "" "${FILECLASS}"
  WriteRegStr HKCU "Software\Classes\.${EXT}" "Content Type" "${MIME}"  
  WriteRegStr HKCU "Software\Classes\.${EXT}\OpenWithProgIds" "${FILECLASS}" ""
  StrCmp "${SHELLNEW}" "0" +2
  WriteRegStr HKCU "Software\Classes\.${EXT}\ShellNew" "NullFile" ""
 
  WriteRegStr HKCU "Software\Classes\${FILECLASS}" "" "${DESCRIPTION}"
  WriteRegStr HKCU "Software\Classes\${FILECLASS}\DefaultIcon" "" "${ICON}"
  WriteRegStr HKCU "Software\Classes\${FILECLASS}\shell" "" "${DEFAULTVERB}"
  WriteRegStr HKCU "Software\Classes\${FILECLASS}\shell\${VERB}" "" "${COMMANDTEXT}"
  WriteRegStr HKCU "Software\Classes\${FILECLASS}\shell\${VERB}\command" "" "${COMMAND}"
!macroend
 
!macro APP_ASSOCIATE_ADDVERB FILECLASS VERB COMMANDTEXT COMMAND
  WriteRegStr HKCU "Software\Classes\${FILECLASS}\shell\${VERB}" "" "${COMMANDTEXT}"
  WriteRegStr HKCU "Software\Classes\${FILECLASS}\shell\${VERB}\command" "" "${COMMAND}"
!macroend
 
!macro APP_ASSOCIATE_REMOVEVERB FILECLASS VERB
  DeleteRegKey HKCR "Software\Classes\${FILECLASS}\shell\${VERB}"
!macroend
 
!macro APP_UNASSOCIATE EXT FILECLASS
  ; Backup the previously associated file class
  ReadRegStr $R0 HKCU "Software\Classes\.${EXT}" "${FILECLASS}_backup"
  WriteRegStr HKCU "Software\Classes\.${EXT}" "" "$R0"
  DeleteRegValue HKCU "Software\Classes\.${EXT}" "${FILECLASS}_backup"
  DeleteRegKey HKCU "Software\Classes\${FILECLASS}"
  
  ; Backup application by default for current user
  ReadRegStr $R0 HKCU "Software\Classes\.${EXT}" "UserChoice_backup"
  DeleteRegKey HKCU "Software\Microsoft\Windows\CurrentVersion\Explorer\FileExts\.${EXT}\UserChoice"
  WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Explorer\FileExts\.${EXT}\UserChoice" "Progid" "$R0"
  DeleteRegValue HKCU "Software\Classes\.${EXT}" "UserChoice_backup"
!macroend
 
 
; !defines for use with SHChangeNotify
!ifdef SHCNE_ASSOCCHANGED
!undef SHCNE_ASSOCCHANGED
!endif
!define SHCNE_ASSOCCHANGED 0x08000000
!ifdef SHCNF_FLUSH
!undef SHCNF_FLUSH
!endif
!define SHCNF_FLUSH        0x1000
 
!macro UPDATEFILEASSOC
; Using the system.dll plugin to call the SHChangeNotify Win32 API function so we
; can update the shell.
  System::Call "shell32::SHChangeNotify(i,i,i,i) (${SHCNE_ASSOCCHANGED}, ${SHCNF_FLUSH}, 0, 0)"
!macroend
 
;EOF
