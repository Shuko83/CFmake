/**
@file ISwRecordManagerListener.h
@brief Interface listener du record manager
@author F.Bighelli
 */

#ifndef _STREAMWORK_SWRECORD_ISWRECORDMANAGERLISTENER_H
#define _STREAMWORK_SWRECORD_ISWRECORDMANAGERLISTENER_H

#define SEP_REPLAY "#¤$"

namespace StreamWork {

    namespace SwRecord {
    
        /**
        @class ISwRecordManagerListener
        @brief Interface listener du record manager

        utilisation: Vue du record manager
        */
        class ISwRecordManagerListener 
		{

        public:
            /**
             * @brief    : Indique un record en cours si true et false sinon
             * @param	 : bool enable - True si on record
             */
            virtual void setRecording(bool enable)=0;

            /**
             * @brief    : Definit le temps de demarrage de l'enregistrement en cours
             * @param	 : double stime - temps en double
             */
            virtual void setStartRecTime(double stime)=0;

            /**
             * @brief    : Definit le temps courant de l'enregistrement en cours
             * @param	 : double ctime - temps en double
             */
            virtual void setCurrentRecTime(double ctime)=0;
            /**
             * @brief    : Definit la taille de l'enregistrement en cours en octet
             * @param	 : int size - la taille en octet
             */
            virtual void setDataSize(int size)=0;

			/**
			 * @brief    : Retourne la liste des records disponible pour le rejeu
			 * @param	 : QString list - list séparer par SEP_REPLAY
			 */
			virtual void setRecordList(QString list) = 0;
        };

    }
    
}
#endif