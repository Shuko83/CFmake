/**
 * @file SwRecordConstantes.h
 * @brief Declaration de constantes
 * @version 1.0
 * @date Tue Apr 28 15:55:16 CEST 2009
 * @author F.Bighelli
 */

#ifndef _SwRecordConstantes_H
#define _SwRecordConstantes_H

/*
  * INCLUDES LOCAUX
  */

#include <Qt/qglobal.h>
//Export via les DLL
#include <SwMacros.h>

#ifdef Q_CC_MSVC
#pragma warning(disable:4290)
#pragma warning(disable:4100)  
#pragma warning(disable:4250)
#endif


#ifdef SwRecord_LIB
#define SwRecord_EXPORT Q_DECL_EXPORT
#define SwRecord_DECLARE_UUID(name) SW_EXTERN_C Q_DECL_EXPORT const StreamWork::SwCore::SwUUID name;
#else
#define SwRecord_EXPORT Q_DECL_IMPORT
#define SwRecord_DECLARE_UUID(name) SW_EXTERN_C Q_DECL_IMPORT const StreamWork::SwCore::SwUUID name;
#endif

#define CG_RECORD_FILE "Record.xml"
#define CG_RECORD_CONFIGURATION_FILE "RecordConfiguration.xml"
#define CG_RECORD_CONFIGURATION_DIRECTORY "configurations"
#define CG_RECORD_FILE_DATA "RecordData_%1.xml"
#define CG_RECORD "Record"
#define CG_RECORD_DATA "RecordData"
#define CG_RECORD_KEY "Key"
#define CG_RECORD_EXE "E"
#define CG_RECORD_START "Start"
#define CG_RECORD_STOP "Stop"
#define CG_RECORD_STOP_DCOUNT "dataCount"
#define CG_RECORD_HEADER "Header"
#define CG_RECORD_BODY "Body"
#define CG_RECORD_RP "RecordPoint"
#define CG_RECORD_RP_ATT_NUMBER "index"
#define CG_RECORD_RP_ATT_NAME "rname"
#define CG_RECORD_TIME "t"

#endif
//------------------------------------------------------
//DO NOT WRITE ANY CODE AFTER THIS POINT
//------------------------------------------------------
