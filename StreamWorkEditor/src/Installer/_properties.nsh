;-------------------------------------------------------------------------------
; PROJECTS PROPERTIES
;-------------------------------------------------------------------------------
!define RELATIVE_SOURCE "..\..\..\Delivery"
!define PRODUCT_PUBLISHER "CS GROUP"
!define PRODUCT_WEB_SITE "http://www.c-s.fr"

!ifndef PRODUCT_NAME
	!define PRODUCT_NAME "StreamWorkEditor"
!endif

!ifndef PRODUCT_VERSION
	!define PRODUCT_FULL_BUILD_NAME "Development"
!else ifndef PRODUCT_BUILD
	!define PRODUCT_FULL_BUILD_NAME "${PRODUCT_VERSION}"
!else
	!define PRODUCT_FULL_BUILD_NAME "${PRODUCT_VERSION}.${PRODUCT_BUILD}"
!endif

!ifndef PRODUCT_EXPORT_CONTROL
	!define PRODUCT_EXPORT_CONTROL "n"
!endif

!ifndef DELIVERY_VAR
	!define DELIVERY_VAR "win-msvc2015-x64-QT5.9.6"
!endif

!define RELATIVE_SOURCE_PATH "${RELATIVE_SOURCE}\${PRODUCT_NAME}\${DELIVERY_VAR}\release"

!define FULL_EXE_NAME "${PRODUCT_NAME}.exe"

!define D_HEADER_IMAGE "NewLogo.bmp"
