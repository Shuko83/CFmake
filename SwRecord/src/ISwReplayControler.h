/**
@file ISwReplayControler.h
@brief Interface du replay manager
@author F.Bighelli
 */

#ifndef _STREAMWORK_SWRECORD_ISWREPLAYCONTROLER_H
#define _STREAMWORK_SWRECORD_ISWREPLAYCONTROLER_H

#include "ISwExecution_Service.h"
#include "ISwReplayListener.h"

namespace StreamWork {

    namespace SwRecord {
    
        /**
        @class ISwReplayControler
        @brief Interface du replay manager

        utilisation:
        setter le repertoire d'enregistrement
        getter le fichier de configuration
        ...
        startReplay pour demarrer le rejeu
        ...
        stopReplay pour arreter le rejeu
        */
        class ISwReplayControler {

        public:
            /**
             * @brief    : definition du repertoire de rejeu
             * @param	 : QString directoryName - Chemin complet du répertoire
             */
            virtual void loadReplay(QString directoryName)=0;

            /**
             * @brief    : Retourne le fichier de configuration
             * @return   : QString - chemin complet 
             */
            virtual QString getConfigurationFile()=0;

            /**
             * @brief    : Demarrage du rejeu
             */
            virtual void startReplay()=0;

            /**
             * @brief    : Arret du rejeu
             */
            virtual void stopReplay()=0;

			/**
             * @brief    : Pause du rejeu
             */
            virtual void pauseReplay()=0;

            /**
             * @brief    : Definit la vitesse de rejeu (si la vitesse d'execution de l'excutor est 50ms, en vitesse 2 l'executor tournera a 25Ms)
             * @param	 : int val - speed (2 = vitesse *2 ) 
             */
            virtual void setSpeed(int val)=0;

            /**
             * @brief    : Ajout de listener
             * @param	 : ISwReplayManagerListener * listener - Pointeur sur un listener
             */
            virtual void addReplayManagerListener(ISwReplayListener * listener)=0;

            /**
             * @brief    : Suppression de listener
			 * @param	 : ISwReplayManagerListener * listener - Pointeur sur un listener
             */
            virtual void removeReplayManagerListener(ISwReplayListener * listener)=0;

        };

    }
    
}
#endif
