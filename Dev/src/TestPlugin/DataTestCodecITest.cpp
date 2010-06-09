/**
@file DataTestCodecITest.cpp
@brief 
@author F.Bighelli
 */

#include "DataTestCodecITest.h"
#include "DataTest.h" 

/** @brief Constructor */
DataTestCodecITest::DataTestCodecITest():SwRecordDataCodecAdapter() {
	//A Completer
}
/* @brief encodage des données */
void DataTestCodecITest::encode(QXmlStreamWriter * writer,void * data) {
    ITest * itest=(ITest *)data;
    writer->writeStartElement("ITest");
    writer->writeAttribute("v",QString("%1").arg(itest->getVal()));
    writer->writeEndElement();
}
/* @brief decodage des données 
   @warning si la donnée est reference compté, ajouter une reference avant renvoi */
void * DataTestCodecITest::decode(QXmlStreamReader * reader,void * data) {
    ITest * itest=0;
    if (reader->isStartElement() && reader->name()=="ITest") {
        QString val=reader->attributes().value(QString(),"v").toString();
        itest=new TestImpl(val.toInt());
    }
    while(!reader->atEnd() && reader->readNext()==QXmlStreamReader::Characters); 
    if(!reader->isEndElement())  {//Fin key ou exe
        return itest;
    }
    return itest;
}
/* @brief renvoie le nom du header xml */
QString DataTestCodecITest::getXmlName() {
    return "ITest";
}