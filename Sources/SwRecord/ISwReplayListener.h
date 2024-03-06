/**
@file ISwReplayListener.h
@brief Interface listener du replay manager
@author F.Bighelli
 */

#ifndef _STREAMWORK_SWRECORD_ISWREPLAYLISTENER_H
#define _STREAMWORK_SWRECORD_ISWREPLAYLISTENER_H

#define STATE_STARTED "Started"
#define STATE_STOPPED "Stopped"
#define STATE_PAUSED "Paused"

namespace StreamWork {

    namespace SwRecord {
    
        /**
        @class ISwReplayManagerListener
        @brief Interface listener du replay manager

        utilisation: Vue du replay manager
        */
        class ISwReplayListener {

        public:
            /** @brief definit l'état du rejeu */
            virtual void setState(QString val)=0;
            /** @brief definit le temps de debut du rejeu en cours  */
            virtual void setStartTime(double vtime)=0;
            /** @brief definit le temps de fin du rejeu en cours */
            virtual void setStopTime(double vtime)=0;
            /** @brief definit le temps courant du rejeu en cours */
            virtual void setCurrentTime(double vtime)=0;
            /** @brief definit le temps courant du cache du rejeu en cours */
            virtual void setCacheTime(double vtime)=0;
			/** @brief definit le répertoire de rejeu*/
			virtual void setCurrentDirectory(QString dir)=0;
			/** @brief definit la vitesse de rejeu */
			virtual void setSpeed(int speed)=0;
        };

    }
    
}
#endif
