/**
@file ISwRecordManagerListener.h
@brief Interface listener du record manager
@author F.Bighelli
 */

#ifndef _STREAMWORK_SWRECORD_ISWRECORDMANAGERLISTENER_H
#define _STREAMWORK_SWRECORD_ISWRECORDMANAGERLISTENER_H

namespace StreamWork {

    namespace SwRecord {
    
        /**
        @class ISwRecordManagerListener
        @brief Interface listener du record manager

        utilisation: Vue du record manager
        */
        class ISwRecordManagerListener {

        public:
            /*@brief indique un record en cours si true et false sinon */
            virtual void setEnableRecordInformation(bool enable)=0;
            /*@brief definit le temps de demarrage de l'enregistrement en cours */
            virtual void setStartTime(double stime)=0;
            /*@brief definit le temps courant de l'enregistrement en cours */
            virtual void setCurrentTime(double ctime)=0;
            /*@brief definit la taille de l'enregistrement en cours en octet */
            virtual void setDataSize(int size)=0;
        };

    }
    
}
#endif