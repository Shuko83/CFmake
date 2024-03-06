SetCompressor lzma
!include FileFunc.nsh
!insertmacro GetOptions
!include LogicLib.nsh
!include _properties.nsh

!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}-${PRODUCT_FULL_BUILD_NAME}"

;--------------------------------
; MUI 1.67 compatible ------
;--------------------------------

!include "MUI.nsh"
!include "LogicLib.nsh"

; MUI Settings

;--------------------------------
;Interface Configuration

!define MUI_HEADERIMAGE
!define MUI_HEADERIMAGE_BITMAP "${D_HEADER_IMAGE}"

!define MUI_ABORTWARNING

;--------------------------------
;Icons

!define MUI_ICON "${NSISDIR}\Contrib\Graphics\Icons\modern-install.ico"
!define MUI_UNICON "${NSISDIR}\Contrib\Graphics\Icons\modern-uninstall.ico"

;--------------------------------
;Langage

; Language Selection Dialog Settings
!define MUI_LANGDLL_REGISTRY_ROOT HKLM
!define MUI_LANGDLL_REGISTRY_KEY "${PRODUCT_UNINST_KEY}"
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
!insertmacro MUI_PAGE_COMPONENTS

; Directory page
!insertmacro MUI_PAGE_DIRECTORY

; Instfiles page
!insertmacro MUI_PAGE_INSTFILES

; Finish page
!define MUI_FINISHPAGE_RUN "$INSTDIR\${FULL_EXE_NAME}"
!insertmacro MUI_PAGE_FINISH

; Uninstaller pages
!insertmacro MUI_UNPAGE_INSTFILES

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
Name "${PRODUCT_NAME} ${PRODUCT_FULL_BUILD_NAME}"
;Icon "${ICON}"

!define /date PRODUCT_DATE_HACKED "%Y-%m-%d-%H-%M-%S"

OutFile "${PRODUCT_NAME}-${PRODUCT_FULL_BUILD_NAME}.${PRODUCT_EXPORT_CONTROL}_${PRODUCT_DATE_HACKED}_Setup.exe"
InstallDir "$PROGRAMFILES64\${PRODUCT_PUBLISHER}\${PRODUCT_NAME}\${PRODUCT_FULL_BUILD_NAME}"
ShowInstDetails hide
ShowUnInstDetails hide

Function .onInit
	!insertmacro MUI_LANGDLL_DISPLAY

	;Test de la version de Windows
	Push $R0
	Push $R1

	ClearErrors
	
	SetRegView 64

	ReadRegStr $R0 HKLM \
	"SOFTWARE\Microsoft\Windows NT\CurrentVersion" CurrentVersion

	IfErrors 0 lbl_winnt

	; we are not NT
	ReadRegStr $R0 HKLM \
	"SOFTWARE\Microsoft\Windows\CurrentVersion" VersionNumber

	StrCpy $R1 $R0 1
	StrCmp $R1 '4' 0 lbl_error

	StrCpy $R1 $R0 3

	StrCmp $R1 '4.0' lbl_win32_95
	StrCmp $R1 '4.9' lbl_win32_ME lbl_win32_98

	lbl_win32_95:
	StrCpy $R0 '95'
	Goto lbl_done

	lbl_win32_98:
	StrCpy $R0 '98'
	Goto lbl_done

	lbl_win32_ME:
	StrCpy $R0 'ME'
	Goto lbl_done

	lbl_winnt:

	StrCpy $R1 $R0 1

	StrCmp $R1 '3' lbl_winnt_x
	StrCmp $R1 '4' lbl_winnt_x

	StrCpy $R1 $R0 3

	StrCmp $R1 '5.0' lbl_winnt_2000
	StrCmp $R1 '5.1' lbl_winnt_XP
	StrCmp $R1 '5.2' lbl_winnt_2003
	StrCmp $R1 '6.0' lbl_winnt_vista
	StrCmp $R1 '6.1' lbl_winnt_7 lbl_error

	lbl_winnt_x:
	StrCpy $R0 "NT $R0" 6

	Goto lbl_done

	lbl_winnt_2000:
	Strcpy $R0 '2000'
	Goto lbl_done

	lbl_winnt_XP:
	Strcpy $R0 'XP'
	Goto lbl_done

	lbl_winnt_2003:
	Strcpy $R0 '2003'
	Goto lbl_done

	lbl_winnt_vista:
	Strcpy $R0 'Vista'
	Goto lbl_done

	lbl_winnt_7:
	Strcpy $R0 '7'
	Goto lbl_done

	lbl_error:
	Strcpy $R0 ''
	lbl_done:

	Pop $R1

	!insertmacro VerifyUserIsAdmin
FunctionEnd

; Request application privileges for Windows Vista
RequestExecutionLevel admin
ShowInstDetails hide

;--------------------------------------------
; Section Principales (Fichiers & raccourcis)
;--------------------------------------------
Section ${PRODUCT_NAME} SEC_PRODUCT
	SectionIn RO
	SetOutPath "$INSTDIR"
	SetOverwrite on
	 
	File ${RELATIVE_SOURCE_PATH}\${FULL_EXE_NAME}

	!include _files.nsh

	AccessControl::GrantOnFile "$INSTDIR" "(S-1-5-32-545)" "FullAccess"
SectionEnd


;--------------------------------------------
; Section Visual C++ Redistributable
;--------------------------------------------
Section "Visual C++ Redistributable" SEC_REDIST
	ExecWait '$INSTDIR\vcredist_x64.exe /install /passive /norestart'
SectionEnd

;--------------------------------------------
; Section RTE GEMALTO
;--------------------------------------------
Section "Gemalto RTE" SEC_GEMALTORTE
	ExecWait '$INSTDIR\haspdinst.exe -i -criticalmsg'
SectionEnd

;--------------------------------
;Descriptions

LangString DESC_SEC_PRODUCT ${LANG_ENGLISH} "Product Installation"
LangString DESC_SEC_PRODUCT ${LANG_FRENCH} "Installation du logiciel"
LangString DESC_SEC_REDIST ${LANG_ENGLISH} "Visual C++ Redistributable Installation"
LangString DESC_SEC_REDIST ${LANG_FRENCH} "Installation du Redistribuable Visual C++"
LangString DESC_SEC_GEMALTORTE ${LANG_ENGLISH} "Gemalto RTE Installation"
LangString DESC_SEC_GEMALTORTE ${LANG_FRENCH} "Installation Gemalto RTE"

;Assign language strings to sections
!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
!insertmacro MUI_DESCRIPTION_TEXT ${SEC_PRODUCT} $(DESC_SEC_PRODUCT)
!insertmacro MUI_DESCRIPTION_TEXT ${SEC_REDIST} $(DESC_SEC_REDIST)
!insertmacro MUI_DESCRIPTION_TEXT ${SEC_GEMALTORTE} $(DESC_SEC_GEMALTORTE)
!insertmacro MUI_FUNCTION_DESCRIPTION_END

;--------------------------------------------
; Section AdditionalIcons
;--------------------------------------------
Section -AdditionalIcons
	;on fait de sorte a installer les raccourcis pour tous les utilisateurs
	SetShellVarContext all
	CreateDirectory "$SMPROGRAMS\${PRODUCT_PUBLISHER}\${PRODUCT_NAME}\${PRODUCT_FULL_BUILD_NAME}"  
	CreateShortCut "$SMPROGRAMS\${PRODUCT_PUBLISHER}\${PRODUCT_NAME}\${PRODUCT_FULL_BUILD_NAME}\${PRODUCT_NAME} ${PRODUCT_FULL_BUILD_NAME}.lnk" "$INSTDIR\${FULL_EXE_NAME}"
	CreateShortCut "$DESKTOP\${PRODUCT_NAME} ${PRODUCT_FULL_BUILD_NAME}.lnk" "$INSTDIR\${FULL_EXE_NAME}"
	CreateShortCut "$SMPROGRAMS\${PRODUCT_PUBLISHER}\${PRODUCT_NAME}\${PRODUCT_FULL_BUILD_NAME}\Uninstall.lnk" "$INSTDIR\uninst.exe"
SectionEnd

;--------------------------------------------
; Post
;--------------------------------------------
Section -Post
	SetRegView 64
	WriteUninstaller "$INSTDIR\uninst.exe"
	WriteRegStr HKLM "${PRODUCT_UNINST_KEY}" "DisplayName" "${PRODUCT_NAME} ${PRODUCT_FULL_BUILD_NAME}"
	WriteRegStr HKLM "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\uninst.exe"
	WriteRegStr HKLM "${PRODUCT_UNINST_KEY}" "DisplayIcon" "$INSTDIR\${FULL_EXE_NAME}"
	WriteRegStr HKLM "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_FULL_BUILD_NAME}"
	WriteRegStr HKLM "${PRODUCT_UNINST_KEY}" "URLInfoAbout" "${PRODUCT_WEB_SITE}"
	WriteRegStr HKLM "${PRODUCT_UNINST_KEY}" "Publisher" "${PRODUCT_PUBLISHER}"
SectionEnd

;--------------------------------------------
; on success
;--------------------------------------------
Function un.onUninstSuccess
	HideWindow
	${Select} $LANGUAGE
	${case} ${LANG_ENGLISH}
		MessageBox MB_ICONINFORMATION|MB_OK "$(^Name) was successfully removed from your computer." /SD IDOK
	${case} ${LANG_FRENCH}
		MessageBox MB_ICONINFORMATION|MB_OK "$(^Name) a été désinstallé avec succès de votre ordinateur." /SD IDOK
	${EndSelect}	
FunctionEnd

;--------------------------------------------
; on init
;--------------------------------------------
Function un.onInit
	!insertmacro MUI_UNGETLANGUAGE
	${Select} $LANGUAGE
	${case} ${LANG_ENGLISH}
		MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "Are you sure you want to completely remove $(^Name) and all of its components? Please stop the program before." /SD IDYES IDYES +2
		Abort
	${case} ${LANG_FRENCH}
		MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "Êtes-vous certains de vouloir désinstaller totalement $(^Name) et tous ses composants ? Veuillez arrêter le programme avant de poursuivre." /SD IDYES IDYES +2
		Abort
	${EndSelect}	
	!insertmacro FindProc "${FULL_EXE_NAME}"
FunctionEnd

;--------------------------------------------
; Uninstall
;--------------------------------------------

Function un.RMDirUP
	!define RMDirUP '!insertmacro RMDirUPCall'
	!macro RMDirUPCall _PATH
		push '${_PATH}'
		Call un.RMDirUP
	!macroend
	; $0 - current folder
	ClearErrors
	Exch $0
	;DetailPrint "ASDF - $0\.."
	RMDir "$0\.."       
	IfErrors Skip
	${RMDirUP} "$0\.."
	Skip:        
	Pop $0
FunctionEnd

Section Uninstall
	Delete "$INSTDIR\*"

	;on fait de sorte a installer les raccourcis pour tous les utilisateurs
	SetShellVarContext all

	Delete "$SMPROGRAMS\${PRODUCT_PUBLISHER}\${PRODUCT_NAME}\${PRODUCT_FULL_BUILD_NAME}\Uninstall.lnk"
	Delete "$DESKTOP\${PRODUCT_NAME} ${PRODUCT_FULL_BUILD_NAME}.lnk"
	Delete "$SMPROGRAMS\${PRODUCT_PUBLISHER}\${PRODUCT_NAME}\${PRODUCT_VERSION}\${PRODUCT_NAME} ${PRODUCT_FULL_BUILD_NAME}.lnk"

	RMDir "$SMPROGRAMS\${PRODUCT_PUBLISHER}\${PRODUCT_NAME}\${PRODUCT_FULL_BUILD_NAME}"
	RMDir "$SMPROGRAMS\${PRODUCT_PUBLISHER}\${PRODUCT_NAME}"

	RMDir /r "$INSTDIR"
	${RMDirUP} "$INSTDIR"

	SetRegView 64
	DeleteRegKey HKLM "${PRODUCT_UNINST_KEY}"
	SetAutoClose true
SectionEnd
