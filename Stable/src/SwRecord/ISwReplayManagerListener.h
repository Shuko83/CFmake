/**
@file ISwReplayManagerListener.h
@brief Interface listener du replay manager
@author F.Bighelli
 */

#ifndef _STREAMWORK_SWRECORD_ISWREPLAYMANAGERLISTENER_H
#define _STREAMWORK_SWRECORD_ISWREPLAYMANAGERLISTENER_H


namespace StreamWork {

    namespace SwRecord {
    
        /**
        @class ISwReplayManagerListener
        @brief Interface listener du replay manager

        utilisation: Vue du replay manager
        */
        class ISwReplayManagerListener {

        public:
            /** @brief ndique un rejeu en cours si true et false sinon */
            virtual void setEnableReplayInformation(bool enable)=0;
            /** @brief definit le temps de debut du rejeu en cours  */
            virtual void setStartTime(double vtime)=0;
            /** @brief definit le temps de fin du rejeu en cours */
            virtual void setStopTime(double vtime)=0;
            /** @brief definit le temps courant du rejeu en cours */
            virtual void setCurrentTime(double vtime)=0;
            /** @brief definit le temps courant du cache du rejeu en cours */
            virtual void setCacheTime(double vtime)=0;
            /** @brief renvoie l'etat de la pause */
            virtual bool getPauseState()=0;
            /** @brief force l'etat de la pause */
            virtual void ForcePauseState()=0;

        };

    }
    
}
#endif
