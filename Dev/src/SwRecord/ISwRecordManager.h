/**
@file ISwRecordManager.h
@brief Interface du record manager
@author F.Bighelli
 */

#ifndef _STREAMWORK_SWRECORD_ISWRECORDMANAGER_H
#define _STREAMWORK_SWRECORD_ISWRECORDMANAGER_H

#include <QXmlStreamWriter>
#include <ISwConfigurationSaver.h>
#include <ISwRecordManagerListener.h>

namespace StreamWork {

    namespace SwRecord {
    
        class ISwRecordPoint;
        /**
        @class ISwRecordManager
        @brief Interface du record manager

        utilisation:
        mettre le record manager en premier dans la liste des executions
        setter le fichier de configuration
        setter le repertoire d'enregistrement
        ...
        appeler startRecording
        executer le stream
        ...
        arreter l'execution du stream
        appeler stopRecording
        */
        class ISwRecordManager {

        public:
            /*@brief definition du repertoire d'enregistrement */
            virtual void setRecordDirectory(QString directoryName)=0;
            /*@brief definition du fichier de configuration */
            virtual void setConfigurationFile(QString fileName)=0;
            /*@brief definition du fichier de configuration */
            virtual void setConfigurationSaver(StreamWork::SwConfiguration::ISwConfigurationSaver * configSaver)=0;
            /*@brief demarrage de l'enregistrement */
            virtual void startRecording()=0;
            /*@brief arret de l'enregistrement */
            virtual void stopRecording()=0;
            /*@brief demande de creation de clef d'enregistrement*/
            virtual QXmlStreamWriter *queryRecordKey(ISwRecordPoint * recordPoint,double currentTime)=0;
            /*@brief finalisation de la clef d'enregistrement*/
            virtual void finalizeRecordKey()=0;
            /*@brief ajout de listener*/
            virtual void addRecordManagerListener(ISwRecordManagerListener * listener)=0;
            /*@brief suppression de listener*/
            virtual void removeRecordManagerListener(ISwRecordManagerListener * listener)=0;
        };

    }
    
}
#endif
