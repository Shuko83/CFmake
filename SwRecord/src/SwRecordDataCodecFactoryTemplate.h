/**
@file SwRecordDataCodecFactoryTemplate.h
@brief Template de factory de codec simple
@author F.Bighelli
 */

#ifndef _STREAMWORK_SWRECORD_SWRECORDDATACODECFACTORYTEMPLATE_H
#define _STREAMWORK_SWRECORD_SWRECORDDATACODECFACTORYTEMPLATE_H

#include "ISwRecordDataCodecFactory.h"

namespace StreamWork {

    namespace SwRecord {
    
        /**
        @class SwRecordDataCodecFactoryTemplate
        @brief Template de factory de codec simple
        */
        template <class DATACODEC_CLASS> class SwRecordDataCodecFactoryTemplate : public ISwRecordDataCodecFactory{
        private:
            QString _dataTypeName;
        public:
	        /** @brief Constructor */
            SwRecordDataCodecFactoryTemplate(QString name) {_dataTypeName=name;}
            /* @brief nom du type de données generé */
            virtual QString generatedDataType() {return _dataTypeName;}
            /* @brief nom des types de données pris en charge */
            virtual QStringList supportedDataTypes() {return QStringList();}
            /* @brief construction d'un codec */
            virtual ISwRecordDataCodec * buildCodec() {return new DATACODEC_CLASS;}
        };

    }
    
}
#endif
