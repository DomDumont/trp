; example2.nsi
;
; This script is based on example1.nsi, but it remember the directory, 
; has uninstall support and (optionally) installs start menu shortcuts.
;
; It will install example2.nsi into a directory that the user selects,

;--------------------------------

; The name of the installer
Name "T.R.P."

; The file to write
OutFile "trp_setup.exe"

; The default installation directory
InstallDir $PROGRAMFILES\trp

; Registry key to check for directory (so if you install again, it will 
; overwrite the old one automatically)
InstallDirRegKey HKLM "Software\trp" "Install_Dir"

; Request application privileges for Windows Vista
RequestExecutionLevel admin

!include "LogicLib.nsh"
;--------------------------------
;Definitions

!define SHCNE_ASSOCCHANGED 0x8000000
!define SHCNF_IDLIST 0

;--------------------------------

; Pages

Page components
Page directory
Page instfiles

UninstPage uninstConfirm
UninstPage instfiles

;--------------------------------

; The stuff to install
Section "T.R.P. (required)"

  SectionIn RO
  
  ; Set output path to the installation directory.
  SetOutPath $INSTDIR
  
  ; Put file there
  
  File /r win32-version\*.*

  
  ; Write the installation path into the registry
  WriteRegStr HKLM SOFTWARE\trp "Install_Dir" "$INSTDIR"
  
  ; Write the uninstall keys for Windows
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\trp" "DisplayName" "T.R.P."
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\trp" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\trp" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\trp" "NoRepair" 1
  WriteUninstaller "uninstall.exe"
  
  WriteRegStr HKCR ".rsc" "" "TRP.Script"
  WriteRegStr HKCR ".rsc" "PerceivedType" "text"
  WriteRegStr HKCR "TRP.Script" "" "TRP Script File"
  WriteRegStr HKCR "TRP.Script\DefaultIcon" "" "$INSTDIR\ratata.exe,2"
  ReadRegStr $R0 HKCR "TRP.Script\shell\open\command" ""
  ${If} $R0 == ""
    WriteRegStr HKCR "TRP.Script\shell" "" "open"
    WriteRegStr HKCR "TRP.Script\shell\open\command" "" '$INSTDIR\ratata.exe "%1"'
  ${EndIf}

  WriteRegStr HKCR ".rap" "" "TRP.Project"
  WriteRegStr HKCR ".rap" "PerceivedType" "text"
  WriteRegStr HKCR "TRP.Project" "" "TRP Project File"
  WriteRegStr HKCR "TRP.Project\DefaultIcon" "" "$INSTDIR\ratata.exe,1"
  ReadRegStr $R0 HKCR "TRP.Project\shell\open\command" ""
  ${If} $R0 == ""
    WriteRegStr HKCR "TRP.Project\shell" "" "open"
    WriteRegStr HKCR "TRP.Project\shell\open\command" "" '$INSTDIR\ratata.exe "%1"'
  ${EndIf}
  
SectionEnd

; Optional section (can be disabled by the user)
Section "Start Menu Shortcuts"

  CreateDirectory "$SMPROGRAMS\trp"
  CreateShortCut "$SMPROGRAMS\trp\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
  CreateShortCut "$SMPROGRAMS\trp\Rototo.lnk" "$INSTDIR\rototo.exe" "" "$INSTDIR\rototo.exe" 0
  CreateShortCut "$SMPROGRAMS\trp\Ratata.lnk" "$INSTDIR\ratata.exe" "" "$INSTDIR\ratata.exe" 0
  CreateShortCut "$SMPROGRAMS\trp\HTML Manual.lnk" "$INSTDIR\docs\trp-manual.html" "" "$INSTDIR\docs\trp-manual.html" 0  
  CreateShortCut "$SMPROGRAMS\trp\PDF Manual.lnk" "$INSTDIR\docs\trp-manual.pdf" "" "$INSTDIR\docs\trp-manual.pdf" 0  
  CreateShortCut "$SMPROGRAMS\trp\Version.lnk" "$INSTDIR\version.txt" "" "$INSTDIR\version.txt" 0  

SectionEnd

;--------------------------------

; Uninstaller

Section "Uninstall"

  ReadRegStr $R0 HKCR ".rsc" ""
  StrCmp $R0 "TRP.Script" 0 +2
  DeleteRegKey HKCR ".rsc"

  ReadRegStr $R0 HKCR ".rap" ""
  StrCmp $R0 "TRP.Project" 0 +2
  DeleteRegKey HKCR ".rap"

  DeleteRegKey HKCR "TRP.Script"
  DeleteRegKey HKCR "TRP.Project"
  
  System::Call 'Shell32::SHChangeNotify(i ${SHCNE_ASSOCCHANGED}, i ${SHCNF_IDLIST}, i 0, i 0)'
  
  ; Remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\trp"
  DeleteRegKey HKLM SOFTWARE\trp

  ; Remove files and uninstaller
  Delete $INSTDIR\*.*
  Delete $INSTDIR\uninstall.exe

  ; Remove shortcuts, if any
  Delete "$SMPROGRAMS\trp\*.*"

  ; Remove directories used
  RMDir "$SMPROGRAMS\trp"
  RMDir /R "$INSTDIR"

SectionEnd
