/**
@file _SwRecordDataCodecDefaultFactory.cpp
@brief Implementation de base d'une factory pour codec pour SwData
@author F.Bighelli
 */

#include "_SwRecordDataCodecDefaultFactory.h"
#include "_SwRecordDataCodecDefault.h" 
#include "SwData_Class.h" 

using namespace StreamWork::SwCore;

/** @brief Constructor */
_SwRecordDataCodecDefaultFactory::_SwRecordDataCodecDefaultFactory() {
	//A Completer
}
/* @brief nom du type de données generé */
QString _SwRecordDataCodecDefaultFactory::generatedDataType() {
    return CG_SW_DEFAULT_TYPE;
}
/* @brief nom des types de données pris en charge */
QStringList _SwRecordDataCodecDefaultFactory::supportedDataTypes(){
    return QStringList();
}
/* @brief construction d'un codec */
ISwRecordDataCodec * _SwRecordDataCodecDefaultFactory::buildCodec(){
    return new _SwRecordDataCodecDefault();
}
