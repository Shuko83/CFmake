/**
@file DataTestCodec.h
@brief 
@author F.Bighelli
 */

#ifndef _DATATESTCODEC_H
#define _DATATESTCODEC_H

#include "SwRecordDataCodecAdapter.h"

using namespace StreamWork::SwRecord;

/**
@class DataTestCodec
@brief 
*/
class DataTestCodec : public SwRecordDataCodecAdapter{

public:
	/** @brief Constructor */
	DataTestCodec();
    /* @brief encodage des données */
    void encode(QXmlStreamWriter * writer,void * data);
    /* @brief decodage des données */
    void * decode(QXmlStreamReader * reader,void * data);
    /* @brief renvoie le nom du header xml */
    virtual QString getXmlName();

};

#endif
