/**
@file ISwServiceRecording.h
@brief Service d'enregistrement
@author F.Bighelli
 */

#ifndef _STREAMWORK_SWRECORD_ISWSERVICERECORDING_H
#define _STREAMWORK_SWRECORD_ISWSERVICERECORDING_H

#include "Main/Services/Management/ISwService.h"
#include "ISwRecordDataCodecFactory.h"
#include "ISwRecordPoint.h"

//Nom du service
#define CG_SW_SERVICE_RECORDING "ServiceRecording"

namespace StreamWork {

    namespace SwRecord {
    
        /**
        @class ISwServiceRecording
        @brief Service d'enregistrement
        */
        class ISwServiceRecording : public SwCore::ISwService{

        public:
            //---------------------------------------------------------------------
            // Partie pour l'enregistrement des factories
            //---------------------------------------------------------------------
            /** 
            * @brief enregistrement d'une fabrique de codecs
            * @param codecFactory fabrique de codec
            */
            virtual void registerCodecFactory(ISwRecordDataCodecFactory * codecFactory)=0;
            /** 
            * @brief enregistrement d'une fabrique de codecs
            * @param codecFactory fabrique de codec
            */
            virtual void unregisterCodecFactory(ISwRecordDataCodecFactory * codecFactory)=0;
            //---------------------------------------------------------------------
            // Partie pour l'enregistrement des point d'enregistrements
            //---------------------------------------------------------------------
            /** 
            * @brief enregistrement d'un point d'enregistrement
            * @param recordPoint point d'enregistrement
            */
            virtual void registerRecordPoint(ISwRecordPoint * recordPoint)=0;
            /** 
            * @brief desenregistrement d'un point d'enregistrement
            * @param recordPoint recordPoint
            */
            virtual void unregisterRecordPoint(ISwRecordPoint * recordPoint)=0;
            //---------------------------------------------------------------------
            // Acces aux codecs disponibles et creation d'un codec
            //---------------------------------------------------------------------
            /** @brief acces au codec disponible pour les SwData */
            virtual QStringList getSwDataCodecs()=0;
            /** @brief Construction d'un codec */
            virtual ISwRecordDataCodec * buildCodec(QString swDataName)=0;
            //---------------------------------------------------------------------
            // Interface ISwService
            //---------------------------------------------------------------------
            /*! \brief Est appele uniquement par le service manager aupres duquel le service est enregistré
            lorsque ce premier se detruit ou une operation de desenregistrement du service est réalisée*/
            virtual void Liberate(){/*Rien a faire*/}   
            /*! \brief Renvoie le nom du service
            \return le nom du service */
            QString GetServiceName(){return QString(CG_SW_SERVICE_RECORDING); }
            /*! \brief Renvoie le nom du service reel (le nom de l'interface)
            \return le nom du service reel (le nom de l'interface)
            \note au niveau de l'interface (pas d'une de ses classes derivées) renvoyer typeid(this).name()*/
            QString GetServiceRealName() {return QString(typeid(this).name());}    
        };

    }
    
}
#endif
