/**
@file _SwRecordDataCodecDefault.h
@brief Implementation de base d'un codec pour SwData
@author F.Bighelli
 */

#ifndef __SWRECORDDATACODECDEFAULT_H
#define __SWRECORDDATACODECDEFAULT_H

#include "SwRecordDataCodecAdapter.h"

using namespace StreamWork::SwRecord;
/**
@class _SwRecordDataCodecDefault
@brief Implementation de base d'un codec pour SwData
*/
class _SwRecordDataCodecDefault : public SwRecordDataCodecAdapter{

public:
	/** @brief Constructor */
	_SwRecordDataCodecDefault();
    /* @brief encodage des données */
    void encode(QXmlStreamWriter * writer,void * data);
    /* @brief decodage des données */
    void * decode(QXmlStreamReader * reader,void * data);

};

#endif
