; fileversion.nsh
; Source http://nsis.sourceforge.net/GetDLLVersionLocal_as_define
;
; GetDLLVersionLocal only gives you two packed 32bit numbers, but 
; sometimes you might need the unpacked numbers at compile time. This is 
; where this ugly thing comes in.
;
; Example usage
; !insertmacro GetPEVersionLocal "$%windir%\system32\kernel32.dll" myver
; Outfile "$%temp%\hello ${myver_1}.${myver_2}.${myver_3}.${myver_4}.exe"

!macro GetPEVersionLocal file defbase
!verbose push
!verbose 2
!tempfile GetPEVersionLocal_nsi
!tempfile GetPEVersionLocal_exe
!define GetPEVersionLocal_doll "$"
!appendfile "${GetPEVersionLocal_nsi}" 'SilentInstall silent$\nRequestExecutionLevel user$\nOutFile "${GetPEVersionLocal_exe}"$\nPage instfiles$\nSection'
!appendfile "${GetPEVersionLocal_nsi}" '$\nFileOpen $0 "${GetPEVersionLocal_nsi}" w$\nGetDllVersion "${file}" $R0 $R1$\nIntOp $R2 $R0 / 0x00010000$\nIntOp $R3 $R0 & 0x0000FFFF$\nIntOp $R4 $R1 / 0x00010000$\nIntOp $R5 $R1 & 0x0000FFFF'
!appendfile "${GetPEVersionLocal_nsi}" '$\nFileWrite $0 "!define ${defbase}_1 $R2${GetPEVersionLocal_doll}\n"$\nFileWrite $0 "!define ${defbase}_2 $R3${GetPEVersionLocal_doll}\n"$\nFileWrite $0 "!define ${defbase}_3 $R4${GetPEVersionLocal_doll}\n"$\nFileWrite $0 "!define ${defbase}_4 $R5${GetPEVersionLocal_doll}\n"$\nFileClose $0$\nSectionEnd'
!system '"${NSISDIR}\makensis" /V2 "${GetPEVersionLocal_nsi}"' = 0
!system '"${GetPEVersionLocal_exe}"' = 0
!include "${GetPEVersionLocal_nsi}"
!delfile "${GetPEVersionLocal_nsi}"
!delfile "${GetPEVersionLocal_exe}"
!undef GetPEVersionLocal_nsi
!undef GetPEVersionLocal_exe
!undef GetPEVersionLocal_doll
!verbose pop
!macroend
