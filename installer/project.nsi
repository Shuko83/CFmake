!include _properties.nsh

SetCompressor lzma


!define PRODUCT_DIR_REGKEY "Software\Microsoft\Windows\CurrentVersion\App Paths\${EXE_NAME}"



;--------------------------------
; MUI 1.67 compatible ------
;--------------------------------
!include "MUI.nsh"

; MUI Settings 

;--------------------------------
;Interface Configuration

!define MUI_HEADERIMAGE
!define MUI_HEADERIMAGE_BITMAP "${D_HEADER_IMAGE}" ; optional
;!define MUI_BGCOLOR ${D_BACKGROUND_COLOR} 

!define MUI_ABORTWARNING

;--------------------------------
;Icons

!define MUI_ICON "${NSISDIR}\Contrib\Graphics\Icons\modern-install.ico"

;--------------------------------
;Langage

; Language Selection Dialog Settings
!define MUI_LANGDLL_REGISTRY_VALUENAME "NSIS:Language"

;--------------------------------
;Pages

; le Titre sera affiché sur 3 lignes... du coup plus de problème de nom tronqué
!define MUI_WELCOMEPAGE_TITLE_3LINES
; Welcome page
!insertmacro MUI_PAGE_WELCOME
; License page
!define MUI_LICENSEPAGE_RADIOBUTTONS
!insertmacro MUI_PAGE_LICENSE "license.rtf"
; Directory Components
!insertmacro MUI_PAGE_COMPONENTS
; Directory page
!insertmacro MUI_PAGE_DIRECTORY
; Instfiles page
!insertmacro MUI_PAGE_INSTFILES
; Finish page
!define MUI_FINISHPAGE_RUN "$INSTDIR\${FULL_EXE_NAME}"
;!define MUI_FINISHPAGE_RUN_PARAMETERS ""
;!define MUI_FINISHPAGE_SHOWREADME "$INSTDIR\readme.txt"
!insertmacro MUI_PAGE_FINISH

;--------------------------------
;Langage

; Language files
!insertmacro MUI_LANGUAGE "English"
!insertmacro MUI_LANGUAGE "French"


; MUI end ------

;--------------------------------
;macro de vérification des droits administrateur
;--------------------------------
!macro VerifyUserIsAdmin
UserInfo::GetAccountType
pop $0
${If} $0 != "admin"
        messageBox mb_iconstop "Administrator rights required!"
        setErrorLevel 740 ;ERROR_ELEVATION_REQUIRED
        quit
${EndIf}
!macroend
;--------------------------------

;--------------------------------
;macro permettant de vérifier si l'application est en cours avant de désinstaller
;--------------------------------
!macro FindProc processName
    nsExec::Exec "taskkill /F /FI $\"IMAGENAME eq ${processName}$\""     
!macroend

;--------------------------------
; The name of the installer
;--------------------------------

Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "${PRODUCT_NAME}.${PRODUCT_VERSION}.${PRODUCT_BUILD}.${PRODUCT_DATE}_Setup.exe"
InstallDir "C:\${PRODUCT_NAME}"


ShowInstDetails hide


; Request application privileges for Windows Vista
RequestExecutionLevel admin
ShowInstDetails hide

;--------------------------------------------
; Section STREAMWORK
;--------------------------------------------

Section "StreamWorkEditor2" SECSTREAMWORK
  SectionIn 1
  
  SetOutPath "$INSTDIR"
  SetOverwrite ifnewer
  
  ;File ${RELATIVE_SOURCE_PATH}\${FULL_EXE_NAME_DEBUG}

  ;CopyFiles ${EXE_NAME} ${FULL_EXE_NAME}
  
  !include _files.nsh
  
  AccessControl::GrantOnFile "$INSTDIR" "(S-1-5-32-545)" "FullAccess"
 
SectionEnd





;--------------------------------------------
; Section Principales (Fichiers & raccourcis)
;--------------------------------------------


  LangString DESC_STREAMWORK ${LANG_ENGLISH} "StreamWorkEditor2"
  LangString DESC_STREAMWORK ${LANG_FRENCH} "StreamWorkEditor2"
  
  ;Assign language strings to sections
  !insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN     
    !insertmacro MUI_DESCRIPTION_TEXT ${SECSTREAMWORK} $(DESC_STREAMWORK)
  !insertmacro MUI_FUNCTION_DESCRIPTION_END 
  
;--------------------------------------------
; Section AdditionalIcons
;--------------------------------------------

Section -AdditionalIcons
  WriteIniStr "$INSTDIR\${PRODUCT_NAME}.url" "InternetShortcut" "URL" "${PRODUCT_WEB_SITE}"
  CreateShortCut "$SMPROGRAMS\${PRODUCT_PUBLISHER}\${PRODUCT_NAME}\${PRODUCT_VERSION}\Website.lnk" "$INSTDIR\${PRODUCT_NAME}.url"
SectionEnd

