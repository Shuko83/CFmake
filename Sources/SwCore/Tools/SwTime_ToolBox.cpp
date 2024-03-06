/*!
 \file SwTime_ToolBox.cpp
 \brief Implementation of the Class SwAddress_Class
 \date 24-ao�t-2006 12:21:05
 \author F.Bighelli
*/
/*
 * INCLUDES GLOBAUX
 */
#include <QtGlobal>
//#include <unistd.h>
#include <time.h>
//include linux
#ifdef Q_OS_LINUX
#include <sys/time.h>
#endif

//include winNt
#ifdef Q_OS_WIN
#include <windows.h>
#define CL_SYS_1_JANVIER_1970   11644473600000LL       //1 janvier 1970 ?00h00mn00 en ms
#endif
/*
 * INCLUDES LOCAUX
 */
#include "Tools/SwTime_ToolBox.h"

using namespace StreamWork::SwCore;

bool _isInitialized=false;
double _startTime;
ULONGLONG _startTickCount;


/*! \brief Permet de recuperer le nombre de seconde depuis le 1 janvier 1970*/
double SwTime_ToolBox::GetTime() {
#ifdef Q_OS_WIN
    if (!_isInitialized) {
        SYSTEMTIME st;
        FILETIME fTime;
        unsigned __int64 timeToGet;
        double t;

        //recup time (100/m de ns) + convert en ms
        GetSystemTime(&st);
        SystemTimeToFileTime (&st,&fTime);
        timeToGet=0;
        timeToGet=(unsigned __int64)(fTime.dwHighDateTime);
        timeToGet=timeToGet << 32;
        timeToGet=timeToGet | (unsigned __int64)(fTime.dwLowDateTime);
        timeToGet=timeToGet / (10000LL);

        //conversion en relatif / 1 janvier 1970 en ms
        timeToGet = (timeToGet - CL_SYS_1_JANVIER_1970);

        //conversion en  GmtTime
        t = (double)(timeToGet);
        t = t / 1000;
        
        _startTime=t;
        _startTickCount=GetTickCount();
        _isInitialized=true;
        return t;
    } else {
        //ne pas utiliser GetTickCount64 car il n'est pas supporte sur Win XP !
        ULONGLONG elapsedms=GetTickCount()-_startTickCount; 
        double elapsedsec=((double)elapsedms)/1000.0;
        return _startTime+elapsedsec;
    }
#endif
#ifdef Q_OS_LINUX
        struct timeval tv;
        double t;

        gettimeofday(&tv,NULL);
        t=tv.tv_sec+((float)tv.tv_usec/1000000.0);
        return(t);
#endif
}
/*! \brief Permet d'attendre un certain nombre de millisecondes*/
void SwTime_ToolBox::InternalSleep(unsigned int time_ms) {
#ifdef Q_OS_WIN
        Sleep(time_ms);
#endif
#ifdef Q_OS_LINUX
        usleep((unsigned long)(time_ms*1000));
#endif
}
