/**
@file DataTestCodecFactory.h
@brief 
@author F.Bighelli
 */

#ifndef _DATATESTCODECFACTORY_H
#define _DATATESTCODECFACTORY_H

#include "ISwRecordDataCodecFactory.h"

using namespace StreamWork::SwRecord;

/**
@class DataTestCodecFactory
@brief 
*/
class DataTestCodecFactory : public ISwRecordDataCodecFactory{

public:
	/** @brief Constructor */
	DataTestCodecFactory();
    /* @brief nom du type de données generé */
    virtual QString generatedDataType();
    /* @brief nom des types de données pris en charge */
    virtual QStringList supportedDataTypes();
    /* @brief construction d'un codec */
    virtual ISwRecordDataCodec * buildCodec();

};

#endif
