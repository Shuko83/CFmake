/**
@file _SwServiceRecording.cpp
@brief Implementation du service de record
@author F.Bighelli
 */

#include "_SwServiceRecording.h"
#include "Main/SwApplication.h"
#include "Main/SwMacros.h"
#include <QDebug>

/** @brief Constructor */
_SwServiceRecording::_SwServiceRecording() {
	//A Completer
}
/** @brief Destructor */
_SwServiceRecording::~_SwServiceRecording(){
	QMap<QString,ISwRecordDataCodecFactory *>::iterator it=_factories.begin();
    while(it!=_factories.end()) {
        delete it.value();
		++it;
    }
    _factories.clear();
    _points.clear();
}
//---------------------------------------------------------------------
// Partie pour l'enregistrement des factories
//---------------------------------------------------------------------
/** 
* @brief enregistrement d'une fabrique de codecs
* @param codecFactory fabrique de codec
*/
void _SwServiceRecording::registerCodecFactory(ISwRecordDataCodecFactory * codecFactory){
    _factories.insert(codecFactory->generatedDataType(),codecFactory);
    SW_APP->Logger().Log(LogLvl_Info,QString("Registering codec for %1\n").arg(codecFactory->generatedDataType()));
}
/** 
* @brief enregistrement d'une fabrique de codecs
* @param codecFactory fabrique de codec
*/
void _SwServiceRecording::unregisterCodecFactory(ISwRecordDataCodecFactory * codecFactory){
    //A revoir
    //QMap<QString,ISwRecordDataCodecFactory *>::iterator it=_factories.find(codecFactory->generatedDataType());
    //if (it!=_factories.end()) {
    //    _factories.erase(it);
    //} 
}
//---------------------------------------------------------------------
// Partie pour l'enregistrement des point d'enregistrements
//---------------------------------------------------------------------
/** 
* @brief enregistrement d'un point d'enregistrement
* @param recordPoint point d'enregistrement
*/
void _SwServiceRecording::registerRecordPoint(ISwRecordPoint * recordPoint){
    _points.push_back(recordPoint);
}
/** 
* @brief desenregistrement d'un point d'enregistrement
* @param recordPoint recordPoint
*/
void _SwServiceRecording::unregisterRecordPoint(ISwRecordPoint * recordPoint){
    _points.removeOne(recordPoint);
}
//---------------------------------------------------------------------
// Acces aux codecs disponibles et creation d'un codec
//---------------------------------------------------------------------
/** @brief acces au codec disponible pour les SwData */
QStringList _SwServiceRecording::getSwDataCodecs() {
    QSet<SwUUID> pin_types;
    QSet<SwUUID>::iterator it;
    QStringList liste;

    pin_types=SW_FACTORIES.GetDataList();
    it=pin_types.begin();
    while (it!=pin_types.end()) {
        //Recuperation d'un type de donnée
        QString swdataName=SW_FACTORIES.GetDataTypeNameFromDataTypeId(*it);
        //Recherche de codec
        QMap<QString,ISwRecordDataCodecFactory *>::iterator itf=_factories.find(swdataName);
        if(itf!=_factories.end()) {
            liste.push_back(swdataName);   
        }
        it++;
    }
    return liste;
}
/** @brief Construction d'un codec */
ISwRecordDataCodec * _SwServiceRecording::buildCodec(QString swDataName) {
    QMap<QString,ISwRecordDataCodecFactory *>::iterator itf=_factories.find(swDataName);
    if(itf!=_factories.end()) {
        //Construction du codec
        ISwRecordDataCodec * codec=itf.value()->buildCodec();
        QStringList liste=itf.value()->supportedDataTypes();
        //Ajout des sous codecs
        for(int i=0;i<liste.count();i++) {
            QList<ISwRecordDataCodec *> subcodecs=buildCodecs(liste[i]);
            foreach(ISwRecordDataCodec* subcodec,subcodecs) {
                codec->addCodec(subcodec);
            }
        }
        return codec;
    }
    return 0;
}
/** @brief Construction d'une liste de codec */
QList<ISwRecordDataCodec *> _SwServiceRecording::buildCodecs(QString swDataName) {
    QList<ISwRecordDataCodec *> returnList;
    QList<ISwRecordDataCodecFactory *> fs=_factories.values(swDataName);
    for(ISwRecordDataCodecFactory * f :fs) {
        //Construction du codec
        ISwRecordDataCodec * codec=f->buildCodec();
        QStringList liste=f->supportedDataTypes();
        //Ajout des sous codecs
        for(int i=0;i<liste.count();i++) {
            QList<ISwRecordDataCodec *> subcodecs=buildCodecs(liste[i]);
            for(ISwRecordDataCodec* subcodec : subcodecs) {
                codec->addCodec(subcodec);
            }
        }
        //Enregistrement
        returnList.push_back(codec);
    }
    return returnList;
}

//---------------------------------------------------------------------
// Interface ISwService
//---------------------------------------------------------------------
/*! \brief Est appele uniquement par le service manager aupres duquel le service est enregistré
lorsque ce premier se detruit ou une operation de desenregistrement du service est réalisée*/
void _SwServiceRecording::Liberate(){
    delete this;
}   
//---------------------------------------------------------------------
// Acces aux points d'enregitrement
//---------------------------------------------------------------------
/** @brief Acces au points d'enregistrements */
QList<ISwRecordPoint *> _SwServiceRecording::getRecordPoints() {
    return _points;
}
/** @brief Acces au points d'enregistrements */
ISwRecordPoint * _SwServiceRecording::getRecordPoint(QString name) {
    foreach(ISwRecordPoint * point,_points) 
	{
		//qDebug()<<point->getRecordName();
		//qDebug()<<name;
        if (point->getRecordName()==name) 
		{
            return point;
        }
    }
    return 0;
}
