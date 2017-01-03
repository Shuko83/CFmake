!ifndef PRODUCT_NAME
  !define PRODUCT_NAME "StreamWorkEditor2"
!endif  

!ifndef PRODUCT_BUILD
	!define PRODUCT_BUILD "build"
!endif
!ifndef PRODUCT_DATE
	!define PRODUCT_DATE "date"
!endif
!ifndef PRODUCT_VERSION
	!define PRODUCT_VERSION "test"
!endif
!define PRODUCT_PUBLISHER "Diginext"
!define PRODUCT_WEB_SITE "http://www.diginext.fr"

!ifndef RELATIVE_SOURCE_PATH_RELEASE
  !define RELATIVE_SOURCE_PATH_RELEASE "..\bin\vc\release"
!endif

!ifndef EXE_NAME_RELEASE
    !define EXE_NAME_RELEASE "StreamWorkEditor2.exe"
!endif

!ifndef RELATIVE_SOURCE_PATH_DEBUG
  !define RELATIVE_SOURCE_PATH_DEBUG "..\bin\vc\debug"
!endif

!ifndef EXE_NAME_DEBUG
    !define EXE_NAME_DEBUG "StreamWorkEditor2d.exe"
!endif

!define FULL_EXE_NAME_RELEASE "StreamWorkEditor2.exe"
!define FULL_EXE_NAME_DEBUG "StreamWorkEditor2d.exe"

!define PRODUCT_UNINST_ROOT_KEY "HKLM"
!define D_HEADER_IMAGE "NewLogo.bmp" ; optional

!define INPUTDIR ${PRODUCT_VERSION}


!if ${PRODUCT_VERSION} == "test"
  !define PRODUCT_VERSION_TAG "${PRODUCT_VERSION}-build#${PRODUCT_BUILD}-${PRODUCT_DATE}" 
!else if ${PRODUCT_VERSION} == "Test"
  !define PRODUCT_VERSION_TAG "${PRODUCT_VERSION}-build#${PRODUCT_BUILD}-${PRODUCT_DATE}"  
!else if ${PRODUCT_VERSION} == "alpha"
  !define PRODUCT_VERSION_TAG "${PRODUCT_VERSION}-build#${PRODUCT_BUILD}-${PRODUCT_DATE}"  
!else if ${PRODUCT_VERSION} == "Alpha"
  !define PRODUCT_VERSION_TAG "${PRODUCT_VERSION}-build#${PRODUCT_BUILD}-${PRODUCT_DATE}" 
!else if ${PRODUCT_VERSION} == "beta"
  !define PRODUCT_VERSION_TAG "${PRODUCT_VERSION}-build#${PRODUCT_BUILD}-${PRODUCT_DATE}"   
!else if ${PRODUCT_VERSION} == "Beta"
  !define PRODUCT_VERSION_TAG "${PRODUCT_VERSION}-build#${PRODUCT_BUILD}-${PRODUCT_DATE}"  
!else 
  !define PRODUCT_VERSION_TAG "${PRODUCT_VERSION}"
!endif


