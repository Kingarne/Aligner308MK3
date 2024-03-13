!addplugindir ".\plugins"
!define AVer 308

!define DEF_INSTDIR "c:\Schill Reglerteknik AB\Aligner ${AVer} V3"
!define PROJDIR "..\Aligner"
!define PROJBIN "..\release"
!define REPORTBIN "..\ReporterLib\ReporterLib\bin\Release"

!searchparse /file ${PROJDIR}\Aligner.rc `FILEVERSION` VER_MAJOR `,` VER_MINOR1 `,` VER_MINOR2 `,` VER_MINOR3
!define ALIGNERVERSION "${VER_MAJOR}.${VER_MINOR1}.${VER_MINOR2}.${VER_MINOR3}"

Name "Aligner ${AVer}"

VIAddVersionKey "ProductName" "Aligner ${AVer}Installation"
VIAddVersionKey "Comments" "Aligner ${AVer} Application"
VIAddVersionKey "CompanyName" "Schill Reglerteknik AB"
VIAddVersionKey "LegalTrademarks" "Schill Reglerteknik AB"
VIAddVersionKey "LegalCopyright" "Schill Reglerteknik AB"
VIAddVersionKey "FileDescription" ""
VIAddVersionKey "FileVersion" ${ALIGNERVERSION}
VIAddVersionKey "ProductVersion" ${ALIGNERVERSION}
VIAddVersionKey "PrivateBuild" "${ALIGNERVERSION} ${__DATE__} ${__TIME__}"

VIProductVersion ${ALIGNERVERSION}

!include "MUI.nsh"

OutFile ".\Aligner${AVer}_Install.exe"
!define MUI_ICON "${PROJDIR}\res\Aligner.ico"
!define MUI_HEADERIMAGE
!define MUI_HEADERIMAGE_BITMAP ".\Align-logo.bmp"
!define MUI_HEADERIMAGE_RIGHT
!define MUI_HEADERIMAGE_BITMAP_NOSTRETCH
!define MUI_WELCOMEFINISHPAGE_BITMAP ".\Align-welcome.bmp"
!define MUI_FINISHPAGE_NOAUTOCLOSE
!define MUI_WELCOMEPAGE_TEXT "Install Aligner-software"
!insertmacro MUI_PAGE_WELCOME
;!insertmacro MUI_PAGE_COMPONENTS
!define MUI_DIRECTORYPAGE_VARIABLE $INSTDIR
!define MUI_DIRECTORYPAGE_TEXT_DESTINATION "Select Installation Directory"
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH


!insertmacro MUI_UNPAGE_WELCOME
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_UNPAGE_FINISH

!insertmacro MUI_LANGUAGE "English"


Function un.onInit

FunctionEnd

Function .onInit
 ; DetailPrint ${ALIGNERVERSION}
  !insertmacro INSTALLOPTIONS_EXTRACT "instdlg.ini"
  StrCpy $INSTDIR "${DEF_INSTDIR}"
FunctionEnd
 

Section "MainSection" SEC01
	
  SetOutPath $INSTDIR
  File "${PROJBIN}\Aligner ${AVer}.exe"
  File "${REPORTBIN}\*.dll"
  File ".\dep\*.ocx"


  File ".\a${AVer}.reg"        
  ExecWait '"regedit.exe" /s "$OUTDIR\a${AVer}.reg"' 
  
  CreateShortCut "$DESKTOP\Aligner${AVer}.lnk" "$INSTDIR\Aligner${AVer}.exe"  
  WriteRegStr HKCU "SOFTWARE\Schill Reglerteknik AB\Aligner ${AVer} V3" "ConnectionString" "$INSTDIR\Aligner${AVer}.mdb"
  WriteRegStr HKCU "SOFTWARE\Schill Reglerteknik AB\Aligner ${AVer} V3" "Directory" "$INSTDIR"

  RegDLL $OUTDIR\olch2x8.ocx

  Delete ".\a${AVer}.reg"

  WriteUninstaller "$INSTDIR\Uninstall.exe"

SectionEnd

Section "SectionSel2" SEC02

SectionEnd

Section "Uninstall"

     Processes::KillProcess "aligner${AVer}.exe"
     Delete "$SMSTARTUP\aligner${AVer}.lnk"   
     
   RMDir /R  /REBOOTOK  "$INSTDIR"

SectionEnd    
