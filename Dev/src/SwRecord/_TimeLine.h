/**
@file _TimeLine.h
@brief Ligne de temps de l'enregistrement
@author F.Bighelli
 */

#ifndef __TIMELINE_H
#define __TIMELINE_H

#include <QList>
#include <QXmlStreamReader>
#include "_ExecutionKey.h"
#include "_SwServiceRecording.h"
#include <ISwClockProvider.h>
#include "ISwReplayManagerListener.h"

using namespace StreamWork::SwRecord;
using namespace StreamWork::SwExecution;
/**
@class _TimeLine
@brief Ligne de temps de l'enregistrement
*/
class _TimeLine : public ISwClockProvider {
    /** @brief temps a l'init */
    double _initTime;
    /** @brief temps au start */
    double _startTime;
    /** @brief execution list*/
    QLinkedList<_ExecutionKey *> _executionKeys; 
    /** @brief temps a la fin */
    double _stopTime;
    /** @brief repertoire d'enregistrement */
    QString _repositoryName;
    /** @brief mapping index vers recordPoint */
    QMap<int,ISwRecordPoint *> _rpMap;
    /* @brief max data counter*/
    int _maxDataCounter;
    /* @brief data counter*/
    int _dataCounter;
    /* @brief executionKeyMaxSize*/
    int _executionKeyMaxSize;
    /* @brief Widget de rejeu */
    QList<ISwReplayManagerListener *> _listeners;
public:
	/** @brief Constructor */
	_TimeLine();
	/** @brief destruction */
	~_TimeLine();
	/** @brief reset */
	void reset();
    /** @brief Chargement du fichier d'enregistrement */
    bool loadRecordFile(QXmlStreamReader * reader,_SwServiceRecording * serviceRecord,QString repositoryName);
    /*! \brief renvoie du temps pour l'initialisation */
    virtual double queryInitTime();
    /*! \brief renvoie du temps pour le demarrage */
    virtual double queryStartTime();
    /*! \brief renvoie du temps pour l'execution */
    virtual double queryExecuteTime(bool * isLast);
    /*! \brief renvoie du temps pour l'arret */
    virtual double queryStopTime();
    /*! \brief pause demandé */
    virtual bool queryPause();
    /*@brief ajout de listener*/
    virtual void addReplayManagerListener(ISwReplayManagerListener * listener);
    /*@brief suppression de listener*/
    virtual void removeReplayManagerListener(ISwReplayManagerListener * listener);
protected:
    /*! \brief renvoie du temps pour l'arret */
    virtual bool loadDataWriter();
};

#endif
