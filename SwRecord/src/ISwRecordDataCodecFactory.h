/**
@file ISwRecordDataCodecFactory.h
@brief Service d'enregistrement
@author F.Bighelli
 */

#ifndef _STREAMWORK_SWRECORD_ISWRECORDDATACODECFACTORY_H
#define _STREAMWORK_SWRECORD_ISWRECORDDATACODECFACTORY_H

#include <QtCore>
#include "ISwRecordDataCodec.h"

namespace StreamWork {

    namespace SwRecord {
    
        /**
        @class ISwRecordDataCodecFactory
        @brief Service d'enregistrement
        */
        class ISwRecordDataCodecFactory {
        public:
            /* @brief nom du type de données generé */
            virtual QString generatedDataType()=0;
            /* @brief nom des types de données pris en charge */
            virtual QStringList supportedDataTypes()=0;
            /* @brief construction d'un codec */
            virtual ISwRecordDataCodec * buildCodec()=0;

        };

    }
    
}
#endif
