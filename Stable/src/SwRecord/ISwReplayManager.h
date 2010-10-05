/**
@file ISwReplayManager.h
@brief Interface du replay manager
@author F.Bighelli
 */

#ifndef _STREAMWORK_SWRECORD_ISWREPLAYMANAGER_H
#define _STREAMWORK_SWRECORD_ISWREPLAYMANAGER_H

#include <ISwExecution_Service.h>
#include "ISwReplayManagerListener.h"

namespace StreamWork {

    namespace SwRecord {
    
        /**
        @class ISwReplayManager
        @brief Interface du replay manager

        utilisation:
        setter le repertoire d'enregistrement
        getter le fichier de configuration
        ...
        startReplay pour demarrer le rejeu
        ...
        stopReplay pour arreter le rejeu
        */
        class ISwReplayManager {

        public:
            /*@brief definition du repertoire d'enregistrement */
            virtual void setRecordDirectory(QString directoryName)=0;
            /*@brief acces au fichier de configuration */
            virtual QString getConfigurationFile()=0;
            /*@brief demarrage du rejeu avec l'executeur de pilotage*/
            virtual void startReplay(StreamWork::SwExecution::ISwExecution_Service * exeService)=0;
            /*@brief arret du rejeu */
            virtual void stopReplay()=0;
            /*@brief ajout de listener*/
            virtual void addReplayManagerListener(ISwReplayManagerListener * listener)=0;
            /*@brief suppression de listener*/
            virtual void removeReplayManagerListener(ISwReplayManagerListener * listener)=0;

        };

    }
    
}
#endif
