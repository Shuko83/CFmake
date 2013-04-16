/**
@file DataTestCodecITest.h
@brief 
@author F.Bighelli
 */

#ifndef _DATATESTCODECITEST_H
#define _DATATESTCODECITEST_H

#include "SwRecordDataCodecAdapter.h"

using namespace StreamWork::SwRecord;

/**
@class DataTestCodecITest
@brief 
*/
class DataTestCodecITest : public SwRecordDataCodecAdapter {

public:
    /** @brief Constructor */
    DataTestCodecITest();
    /* @brief encodage des données */
    void encode(QXmlStreamWriter * writer,void * data);
    /* @brief decodage des données 
       @warning si la donnée est reference compté, ajouter une reference avant renvoi */
    virtual void * decode(QXmlStreamReader * reader,void * data);
    /* @brief renvoie le nom du header xml */
    virtual QString getXmlName();

};

#endif
