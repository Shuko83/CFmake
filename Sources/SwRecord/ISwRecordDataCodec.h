/**
@file ISwRecordDataCodec.h
@brief Service d'enregistrement
@author F.Bighelli
 */

#ifndef _STREAMWORK_SWRECORD_ISWRECORDDATACODEC_H
#define _STREAMWORK_SWRECORD_ISWRECORDDATACODEC_H

#include <QXmlStreamWriter>
#include <QXmlStreamReader>

namespace StreamWork {

    namespace SwRecord {
    
        /**
        @class ISwRecordDataCodec
        @brief Service d'enregistrement
        */
        class ISwRecordDataCodec {
        public:
            /* @brief ajoute un sous codec */
            virtual void addCodec(ISwRecordDataCodec * codec)=0;
            /* @brief destruction du codec */
            virtual void destroy()=0;
            /* @brief encodage des données */
            virtual void encode(QXmlStreamWriter * writer,void * data)=0;
            /* @brief decodage des données 
               @warning si la donnée est reference compté, ajouter une reference avant renvoi */
            virtual void * decode(QXmlStreamReader * reader,void * data)=0;
            /* @brief renvoie le nom du header xml */
            virtual QString getXmlName()=0;
        };

    }
    
}
#endif
