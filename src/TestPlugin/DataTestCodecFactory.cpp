/**
@file DataTestCodecFactory.cpp
@brief 
@author F.Bighelli
 */

#include "DataTestCodecFactory.h"
#include "DataTestCodec.h"
#include "DataTest.h"
 

/** @brief Constructor */
DataTestCodecFactory::DataTestCodecFactory() {
	//A Completer
}
/* @brief nom du type de données generé */
QString DataTestCodecFactory::generatedDataType() {
    return CG_DATA_TEST;
}
/* @brief nom des types de données pris en charge */
QStringList DataTestCodecFactory::supportedDataTypes(){
    QStringList liste;
    liste.push_back(CG_DATA_TEST_ITEST);
    return liste;
}
/* @brief construction d'un codec */
ISwRecordDataCodec * DataTestCodecFactory::buildCodec(){
    return new DataTestCodec();
}

