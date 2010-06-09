/**
@file _TimeLine.cpp
@brief Ligne de temps de l'enregistrement
@author F.Bighelli
 */

#include "_TimeLine.h"
#include "SwRecordConstantes.h"
#include <SwUUID.h>
 
using namespace StreamWork::SwCore;

typedef enum {StartDocElt,RecordElt,HeaderBeginElt,RecordPointBeginElt,RecordPointEndElt,HeaderEndElt,BodyElt} TElt;

/** @brief Constructor */
_TimeLine::_TimeLine() {
	_initTime=0;
	_startTime=0;
	_stopTime=0;
    _widget=0;
}
/** @brief destruction */
_TimeLine::~_TimeLine() {
    foreach( _ExecutionKey *ekey,_executionKeys) {
        delete ekey;
    }
    _executionKeys.clear();
}
void _TimeLine::reset() {
	_initTime=0;
	_startTime=0;
	_stopTime=0;
    _dataCounter=0;
    _executionKeyMaxSize=0;
    foreach( _ExecutionKey *ekey,_executionKeys) {
        delete ekey;
    }
    _rpMap.clear();
    _executionKeys.clear();
}
/** @brief definition du widget */
void _TimeLine::setWidget(_ReplayWidget * widget) {
    _widget=widget;
}
/** @brief Chargement du fichier d'enregistrement */
bool _TimeLine::loadRecordFile(QXmlStreamReader * reader,_SwServiceRecording * serviceRecord,QString repositoryName) {
    _repositoryName=repositoryName;
    reader->readNext();//Debug doc
    if(!reader->isStartDocument()) {  
        return false;
    }
    while(!reader->atEnd() && reader->readNext()==QXmlStreamReader::Characters); //Debug record
    if (!reader->isStartElement() || reader->name()!=CG_RECORD) { 
        return false;
    }
    //--------------------------------------------------------------------------
    // HEADER PART
    //--------------------------------------------------------------------------
    while(!reader->atEnd() && reader->readNext()==QXmlStreamReader::Characters); //Debug header
    if (!reader->isStartElement() || reader->name()!=CG_RECORD_HEADER) 
        return false;
    if (reader->attributes().hasAttribute(CG_RECORD_TIME)) {
        QString val=reader->attributes().value(QString(),CG_RECORD_TIME).toString();
        _initTime=val.toDouble();
    }
    while(!reader->atEnd() && reader->readNext()==QXmlStreamReader::Characters); //Debug record point
    while(reader->isStartElement() && reader->name()==CG_RECORD_RP) {
        if (reader->attributes().hasAttribute(CG_RECORD_RP_ATT_NUMBER) && 
            reader->attributes().hasAttribute(CG_RECORD_RP_ATT_NAME)) {
            QString val=reader->attributes().value(QString(),CG_RECORD_RP_ATT_NUMBER).toString();
            int index=val.toInt();
            SwUUID uuid;
            val=reader->attributes().value(QString(),CG_RECORD_RP_ATT_NAME).toString();
            ISwRecordPoint *rp=serviceRecord->getRecordPoint(val);
            if (rp!=0) {
                rp->cleanKeys();
                _rpMap.insert(index,rp);
            }
        } 
        while(!reader->atEnd() && reader->readNext()==QXmlStreamReader::Characters);
        if(!reader->isEndElement()) //Fin record point
            return false;
        while(!reader->atEnd() && reader->readNext()==QXmlStreamReader::Characters); //Debug record point || Fin header
    }
    if(!reader->isEndElement()) //Fin header
        return false;

    //--------------------------------------------------------------------------
    // BODY PART
    //--------------------------------------------------------------------------
    while(!reader->atEnd() && reader->readNext()==QXmlStreamReader::Characters); //Debug body
    if (!reader->isStartElement() || reader->name()!=CG_RECORD_BODY) 
        return false;

    while(!reader->atEnd() && reader->readNext()==QXmlStreamReader::Characters); //Debug start
    qDebug(reader->tokenString().toLatin1().data());
    if (!reader->isStartElement() || reader->name()!=CG_RECORD_START) 
        return false;
    if (reader->attributes().hasAttribute(CG_RECORD_TIME)) {
        QString val=reader->attributes().value(QString(),CG_RECORD_TIME).toString();
        _startTime=val.toDouble()+_initTime;
    }
    while(!reader->atEnd() && reader->readNext()==QXmlStreamReader::Characters); 
    if(!reader->isEndElement()) //Fin start
        return false;

    while(!reader->atEnd() && reader->readNext()==QXmlStreamReader::Characters);     

    if (reader->name()==CG_RECORD_STOP) {
        QString val=reader->attributes().value(QString(),CG_RECORD_TIME).toString();
        _stopTime=val.toDouble()+_initTime;
        val=reader->attributes().value(QString(),CG_RECORD_STOP_DCOUNT).toString();
        _maxDataCounter=val.toInt();
    }
    while(!reader->atEnd() && reader->readNext()==QXmlStreamReader::Characters); 
    if(!reader->isEndElement()) //Fin stop
        return false;
    while(!reader->atEnd() && reader->readNext()==QXmlStreamReader::Characters); 
    if(!reader->isEndElement()) //Fin body
        return false;
    while(!reader->atEnd() && reader->readNext()==QXmlStreamReader::Characters); 
    if(!reader->isEndElement()) //Fin record
        return false;
    while(!reader->atEnd() && reader->readNext()==QXmlStreamReader::Characters); 
    if(!reader->isEndDocument ()) //Fin docuement
        return false;

    if (reader->hasError()) {
        qDebug(reader->errorString().toLatin1().data());
        return false;
    }

    if (_widget!=0) {
        _widget->setStartTime(_initTime);   
        _widget->setStopTime(_stopTime);   
        _widget->setCurrentTime(_initTime);   
    }
    loadDataWriter();

    return true;
}


/*! \brief renvoie du temps pour l'initialisation */
double _TimeLine::queryInitTime() {
    if (_widget!=0) 
        _widget->setEnableReplayInformation(true);
    return _initTime;
}
/*! \brief renvoie du temps pour le demarrage */
double _TimeLine::queryStartTime() {
    return _startTime;
}
/*! \brief renvoie du temps pour l'execution */
double _TimeLine::queryExecuteTime(bool * isLast) {
    if (_executionKeyMaxSize/2>_executionKeys.count() && _dataCounter<_maxDataCounter) {
        loadDataWriter();
    }
    if (_executionKeys.isEmpty()) {
        *isLast=true;
        return _stopTime;
    }
    _ExecutionKey * ekey=_executionKeys.front();
    _executionKeys.pop_front();
    if (_executionKeys.isEmpty()) {
        *isLast=true;
        if (_widget!=0) {
            _widget->ForcePauseState();
        }
    }
    double t=ekey->_currentTime;
    QList<ISwRecordPoint *>::iterator itrp=ekey->_recordPoints.begin();
    QList<int>::iterator itrpcall=ekey->_recordPointsCall.begin();
    while(itrp!=ekey->_recordPoints.end()) {
        ISwRecordPoint * rp=(*itrp);
        int nbCall=(*itrpcall);
        for(int i=0;i<nbCall;i++) {
            rp->submitKey();
        }
        itrp++;
        itrpcall++;
    }
    delete ekey;
    if (_widget!=0)
        _widget->setCurrentTime(t);   
    return t;
}
/*! \brief renvoie du temps pour l'arret */
double _TimeLine::queryStopTime() {
    if (_widget!=0) 
        _widget->setEnableReplayInformation(false);
    return _stopTime;
}
/*! \brief pause demandé */
bool _TimeLine::queryPause() {
    if (_widget!=0) 
        return _widget->getPauseState();
    return false;
}
/*! \brief renvoie du temps pour l'arret */
bool _TimeLine::loadDataWriter() {
    QDir d=QDir(_repositoryName);
    QString name=QString(d.absolutePath()+QDir::separator()+CG_RECORD_FILE_DATA).arg(_dataCounter++);
    //Chargement du fichier d'enregistrement
    QFile *fileReader=new QFile(name);
    if (!fileReader->open(QIODevice::ReadOnly | QIODevice::Text)) {
        delete fileReader;
        fileReader=0;
        return false;
    }
    QXmlStreamReader * reader=new QXmlStreamReader(fileReader);

    reader->readNext();//Debug doc
    if(!reader->isStartDocument()) {  
        return false;
    }
    while(!reader->atEnd() && reader->readNext()==QXmlStreamReader::Characters); //Debug record
    if (!reader->isStartElement() || reader->name()!=CG_RECORD_DATA) { 
        return false;
    }
    while(!reader->atEnd() && reader->readNext()==QXmlStreamReader::Characters); 

    while(reader->isStartElement()) {      
        if (reader->name()==CG_RECORD_EXE) {
            QString val=reader->attributes().value(QString(),CG_RECORD_TIME).toString();
            _ExecutionKey * ekey=new _ExecutionKey();
            ekey->_currentTime=val.toDouble()+_initTime;
            _executionKeys.push_back(ekey);
            while(!reader->atEnd() && reader->readNext()==QXmlStreamReader::Characters); 
        } else if (reader->name()==CG_RECORD_KEY) {
            QString val=reader->attributes().value(QString(),CG_RECORD_RP_ATT_NUMBER).toString();
            int index=val.toInt();
            while(!reader->atEnd() && reader->readNext()==QXmlStreamReader::Characters); 
            QMap<int,ISwRecordPoint *>::iterator it=_rpMap.find(index);
            int nbCall=0;
            if (it!=_rpMap.end()) {
                while(reader->isStartElement()) {
                    QString node_name=reader->name().toString();
                    it.value()->buildKey(reader);
                    nbCall++;
                    while(!reader->atEnd() && reader->readNext()==QXmlStreamReader::Characters); 
                    if (reader->isEndElement() && reader->name()==node_name)
                        while(!reader->atEnd() && reader->readNext()==QXmlStreamReader::Characters); 
                }
            }
            _executionKeys.back()->_recordPoints.push_back(it.value());
            _executionKeys.back()->_recordPointsCall.push_back(nbCall);
        } else {
            return false;
        }
        if(!reader->isEndElement()) {
            //Fin key ou exe
            return false;
        }
        while(!reader->atEnd() && reader->readNext()==QXmlStreamReader::Characters); 
    }

    if(!reader->isEndElement()) //Fin record
        return false;
    while(!reader->atEnd() && reader->readNext()==QXmlStreamReader::Characters); 
    if(!reader->isEndDocument ()) //Fin docuement
        return false;

    if (reader->hasError()) {
        qDebug(reader->errorString().toLatin1().data());
        return false;
    }

    delete reader;
    fileReader->close();
    delete fileReader;

    if (_executionKeyMaxSize==0)
        _executionKeyMaxSize=_executionKeys.count();

    if (_widget!=0) 
        _widget->setCacheTime(_executionKeys.back()->_currentTime);   
    return true;
}
