/**
@file ISwRecordManager.h
@brief Interface du record manager
@author F.Bighelli
 */

#ifndef _STREAMWORK_SWRECORD_ISWRECORDMANAGER_H
#define _STREAMWORK_SWRECORD_ISWRECORDMANAGER_H

#include <QXmlStreamWriter>
#include <ISwRecordManagerListener.h>

namespace StreamWork 
{

    namespace SwRecord 
	{
    
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
        class ISwRecordManager 
		{

        public:

            /**
             * @brief    : Definition du repertoire d'enregistrement
             * @param	 : QString directoryName - Path du répertoire
             */
            virtual void setRecordDirectory(QString directoryName)=0;
			
			/**
             * @brief    : Definition du repertoire contenant tout les enregistrements
             * @param	 : QString directoryName - Path du répertoire
             */
            virtual void setMainDir(QString directoryName)=0;

            /**
             * @brief    : Démarre l'enregistrement
             */
            virtual void startRecording()=0;

            /**
             * @brief    : Arret de l'enregistrement
             */
            virtual void stopRecording()=0;

            /**
             * @brief    : Demande de creation de clef d'enregistrement
             * @return   : QXmlStreamWriter * - Permet d'écrire dans ce writer
             * @param	 : ISwRecordPoint * recordPoint - Pointeru vers le recordPoint qui veut la clef
             * @param	 : double currentTime - Le temps courant au moment de la demande
             */
            virtual QXmlStreamWriter *queryRecordKey(ISwRecordPoint * recordPoint,double currentTime)=0;

            /**
             * @brief    : Finalisation de la clef d'enregistrement
             * @return   : void
             */
            virtual void finalizeRecordKey()=0;


			/**
             * @brief    : Demande de creation de clef d'enregistrement pour les propriétés
             * @return   : QXmlStreamWriter * - Permet d'écrire dans ce writer
             * @param	 : ISwRecordPoint * recordPoint - Pointeur vers le recordPoint qui veut la clef
             * @param	 : double currentTime - Le temps courant au moment de la demande
             */
            virtual QXmlStreamWriter *queryPropertyKey(ISwRecordPoint * recordPoint,double currentTime)=0;

            /**
             * @brief    : Finalisation de la clef d'enregistrement
             * @return   : void
             */
            virtual void finalizePropertyKey()=0;

            /**
             * @brief    : Ajout de listener
             * @param	 : ISwRecordManagerListener * listener - Pointeur sur un listener
             */
            virtual void addRecordManagerListener(ISwRecordManagerListener * listener)=0;

            /**
             * @brief    : Suppression de listener
             * @param	 : ISwRecordManagerListener * listener -  Pointeur sur un listener
             */
            virtual void removeRecordManagerListener(ISwRecordManagerListener * listener)=0;

			/**
			 * @brief    : Permet de recupérer l'état du recordManager
			 * @return   : bool true si l'enregistrement est en cours
			 */
			virtual bool isRecording()=0;
        };

    }
    
}
#endif
