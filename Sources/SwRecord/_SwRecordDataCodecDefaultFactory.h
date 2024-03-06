/**
@file _SwRecordDataCodecDefaultFactory.h
@brief Implementation de base d'une factory pour codec pour SwData
@author F.Bighelli
 */

#ifndef __SWRECORDDATACODECDEFAULTFACTORY_H
#define __SWRECORDDATACODECDEFAULTFACTORY_H

#include "ISwRecordDataCodecFactory.h"

using namespace StreamWork::SwRecord;

/**
@class _SwRecordDataCodecDefaultFactory
@brief Implementation de base d'une factory pour codec pour SwData
*/
class _SwRecordDataCodecDefaultFactory : public ISwRecordDataCodecFactory{

public:
	/** @brief Constructor */
	_SwRecordDataCodecDefaultFactory();
    /* @brief nom du type de données generé */
    virtual QString generatedDataType();
    /* @brief nom des types de données pris en charge */
    virtual QStringList supportedDataTypes();
    /* @brief construction d'un codec */
    virtual ISwRecordDataCodec * buildCodec();

};

#endif
