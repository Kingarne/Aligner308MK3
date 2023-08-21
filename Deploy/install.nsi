!addplugindir ".\plugins"

!define DEF_INSTDIR "c:\Schill Reglerteknik AB\Aligner308"
!define PROJDIR "..\Aligner"
!define PROJBIN "..\release"
!define REPORTBIN "..\ReporterLib\ReporterLib\bin\Release"

!searchparse /file  ${PROJDIR}\Aligner.rc `FILEVERSION` VER_MAJOR `.` VER_MINOR1 `.` VER_MINOR2 `.` VER_MINOR3 ``
!define ALIGNERVERSION "${VER_MAJOR}.${VER_MINOR1}.${VER_MINOR2}.${VER_MINOR3}"

Name "Aligner 308"

VIAddVersionKey "ProductName" "Aligner 308 Installation"
VIAddVersionKey "Comments" "Aligner 308 Application"
VIAddVersionKey "CompanyName" "Schill Reglerteknik AB"
VIAddVersionKey "LegalTrademarks" "Schill Reglerteknik AB"
VIAddVersionKey "LegalCopyright" "Schill Reglerteknik AB"
VIAddVersionKey "FileDescription" ""
VIAddVersionKey "FileVersion" ${ALIGNERVERSION}
VIAddVersionKey "ProductVersion" "1.2.0.0"
VIAddVersionKey "PrivateBuild" "1.2.0.0 ${__DATE__} ${__TIME__}"

VIProductVersion "1.2.0.0"

!include "MUI.nsh"

;Name "AT ODEN ${ATGVERSION}"
OutFile ".\Aligner308_Install.exe"
;Icon "${NSISDIR}\Contrib\Graphics\Icons\modern-install-colorful.ico"
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
;Page custom ODENSelect
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

Function ODENSelect
  !insertmacro INSTALLOPTIONS_DISPLAY "instdlg.ini"
FunctionEnd


Function .onInit
 ; DetailPrint ${ALIGNERVERSION}
  !insertmacro INSTALLOPTIONS_EXTRACT "instdlg.ini"
  StrCpy $INSTDIR "${DEF_INSTDIR}"
FunctionEnd
 

Section "MainSection" SEC01
	 ;!insertmacro INSTALLOPTIONS_READ $1 "instdlg.ini" "Field 2" "State"
; $2 = DS
 
;  SetOutPath "${ATGDIR}\ATG"
    
;  ${If} $1 == "ODEN"
  
  ;SetOutPath "$LocalAppdata\Air Target\Ontin" 
  ;File "${PROJDIR}\Install Files\torini.xml"
  SetOutPath $INSTDIR
  File "${PROJBIN}\*.exe"
  File "${REPORTBIN}\*.dll"
  


  ;CreateShortCut "$SMSTARTUP\Ontin-Scoring.lnk" "${ATGDIR}\Ontin-Scoring.exe"        
 
;	${EndIf} 


  WriteUninstaller "$INSTDIR\Uninstall.exe"

SectionEnd

Section "SectionSel2" SEC02

SectionEnd

Section "Uninstall"

     Processes::KillProcess "aligner.exe"
     Delete "$SMSTARTUP\aligner.lnk"   
     
   RMDir /R  /REBOOTOK  "$INSTDIR"

SectionEnd    
