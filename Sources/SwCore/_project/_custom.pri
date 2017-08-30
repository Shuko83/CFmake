win32:{
    LIBS += -lPsapi -limagehlp
} 
PRECOMPILED_HEADER = ../pch/StdAfx.h

HEADERS -= ../pch/StdAfx.h
SOURCES -= ../pch/StdAfx.h.cpp

DEFINES += UNICODE 

DEFINES+= _CRT_SECURE_NO_WARNINGS