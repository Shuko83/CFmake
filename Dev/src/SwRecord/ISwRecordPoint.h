/**
@file ISwRecordPoint.h
@brief Point d'enregistrement
@author F.Bighelli
 */

#ifndef _STREAMWORK_SWRECORD_ISWRECORDPOINT_H
#define _STREAMWORK_SWRECORD_ISWRECORDPOINT_H

#include <QtCore>
#include <SwUUID.h>
#include <ISwRecordManager.h>
#include <QXmlStreamReader>

namespace StreamWork {

    namespace SwRecord {
    
        /**
        @class ISwRecordPoint
        @brief Point d'enregistrement
        */
        class ISwRecordPoint {
        public:
            /* @brief identifiant */
            virtual SwCore::SwUUID getRecordIdentifier()=0;
            /* @brief name */
            virtual QString getRecordName()=0;
            /* @brief assignation du manager d'enregistrement */
            virtual void setRecordManager(ISwRecordManager * recordManager)=0;
            /* @brief construction d'une clef */
            virtual bool buildKey(QXmlStreamReader * reader)=0;
            /* @brief soumission d'une clef pour l'emission*/
            virtual void submitKey()=0;
            /* @brief clean des clefs existantes*/
            virtual void cleanKeys()=0;

        };

    }
    
}
#endif
