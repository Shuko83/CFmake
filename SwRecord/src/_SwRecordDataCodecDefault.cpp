/**
@file _SwRecordDataCodecDefault.cpp
@brief Implementation de base d'un codec pour SwData
@author F.Bighelli
 */

#include "_SwRecordDataCodecDefault.h"
 
#include "Component/Pin/SwData_Class.h"

using namespace StreamWork::SwCore;

/** @brief Constructor */
_SwRecordDataCodecDefault::_SwRecordDataCodecDefault():SwRecordDataCodecAdapter(){
	//A Completer
}
/* @brief encodage des données */
void _SwRecordDataCodecDefault::encode(QXmlStreamWriter * writer,void * data) {
    SwData_Class * ldata=(SwData_Class *)data;
    writer->writeStartElement("SwData");
    writer->writeAttribute("cnl",QString("%1").arg(ldata->Channel));
    writer->writeAttribute("pri",QString("%1").arg(ldata->Priority));
    writer->writeAttribute("time",QString("%1").arg(ldata->Time,20,'f',3));
    writer->writeEndElement();
}
/* @brief decodage des données */
void * _SwRecordDataCodecDefault::decode(QXmlStreamReader * reader,void * data) {
    SwData_Class * ldata=(SwData_Class *)data;
    if (reader->isStartElement() && reader->name()=="SwData") {
        QString val=reader->attributes().value(QString(),"cnl").toString();
        ldata->Channel=val.toInt();
        val=reader->attributes().value(QString(),"pri").toString();
        ldata->Priority=val.toInt();
        val=reader->attributes().value(QString(),"time").toString();
        ldata->Time=val.toDouble();
        ldata->_addRef();
    }
    while(!reader->atEnd() && reader->readNext()==QXmlStreamReader::Characters); 
    if(!reader->isEndElement())  {//Fin key ou exe
        return data;
    }
    return data;
}
