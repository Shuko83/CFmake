/**
@file DataTestCodec.cpp
@brief 
@author F.Bighelli
 */

#include "DataTestCodec.h"
#include "DataTest.h"

using namespace StreamWork::SwCore; 

/** @brief Constructor */
DataTestCodec::DataTestCodec():SwRecordDataCodecAdapter() {
	//A Completer
}
/* @brief encodage des données */
void DataTestCodec::encode(QXmlStreamWriter * writer,void * data) {
    DataTest * ldata=(DataTest *)data;
    writer->writeStartElement("DataTest");
    writer->writeAttribute("cnl",QString("%1").arg(ldata->Channel));
    writer->writeAttribute("pri",QString("%1").arg(ldata->Priority));
    writer->writeAttribute("time",QString("%1").arg(ldata->Time,0,'f',3));
    writer->writeAttribute("content",ldata->GetContent());
    foreach(ISwRecordDataCodec * codec,_codecs) {
        if (ldata->_itest!=0) 
            codec->encode(writer,(void *)ldata->_itest);
    }
    writer->writeEndElement();

}
/* @brief decodage des données */
void * DataTestCodec::decode(QXmlStreamReader * reader,void * data) {
    ((SwData_Class *)data)->_release();
    DataTest * ldata=new DataTest;
    ldata->_addRef();
    if (reader->isStartElement() && reader->name()=="DataTest") {
        ldata=new DataTest();
        QString val=reader->attributes().value(QString(),"cnl").toString();
        ldata->Channel=val.toInt();
        val=reader->attributes().value(QString(),"pri").toString();
        ldata->Priority=val.toInt();
        val=reader->attributes().value(QString(),"time").toString();
        ldata->Time=val.toDouble();
        ldata->SetContent(reader->attributes().value(QString(),"content").toString());

        ldata->_addRef();
    }
    while(!reader->atEnd() && reader->readNext()==QXmlStreamReader::Characters); 
    if (reader->isStartElement()) {
        foreach(ISwRecordDataCodec * codec,_codecs) {
            if (codec->getXmlName()==reader->name()) 
                ldata->_itest=(ITest *)codec->decode(reader,ldata->_itest);
        }
        while(!reader->atEnd() && reader->readNext()==QXmlStreamReader::Characters); 
    } 
    if(!reader->isEndElement())  {//Fin SwData
        return ldata;
    }
    return ldata;
}
/* @brief renvoie le nom du header xml */
QString DataTestCodec::getXmlName() {
    return "DataTest";
}
