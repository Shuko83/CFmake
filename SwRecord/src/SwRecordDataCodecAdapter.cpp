/**
@file SwRecordDataCodecAdapter.cpp
@brief Implementation par defaut d'un codec
@author F.Bighelli
 */

#include "SwRecordDataCodecAdapter.h"
 
using namespace StreamWork::SwRecord;

/** @brief Constructor */
SwRecordDataCodecAdapter::SwRecordDataCodecAdapter() {
	//A Completer
}
/** @brief Destructor */
SwRecordDataCodecAdapter::~SwRecordDataCodecAdapter() {
    for(int i=0;i<_codecs.count();i++) {
        _codecs[i]->destroy();
    }
}
/* @brief ajoute un sous codec */
void SwRecordDataCodecAdapter::addCodec(ISwRecordDataCodec * codec) {
    _codecs.push_back(codec);
}
/* @brief destruction du codec */
void SwRecordDataCodecAdapter::destroy() {
    delete this;
}
/* @brief renvoie le nom du header xml */
QString SwRecordDataCodecAdapter::getXmlName() {
    return "???";
}
