/**
@file SwRecordDataCodecAdapter.h
@brief Implementation par defaut d'un codec
@author F.Bighelli
 */

#ifndef _STREAMWORK_SWRECORD_SWRECORDDATACODECADAPTER_H
#define _STREAMWORK_SWRECORD_SWRECORDDATACODECADAPTER_H

#include "ISwRecordDataCodec.h"
#include "SwRecordConstantes.h"
#include <QList>

namespace StreamWork {

    namespace SwRecord {
    
        /**
        @class SwRecordDataCodecAdapter
        @brief Implementation par defaut d'un codec
        */
        class SwRecord_EXPORT SwRecordDataCodecAdapter : public ISwRecordDataCodec {
        protected:
            /** @brief liste des sous codec */
            QList<ISwRecordDataCodec *> _codecs;
        public:
	        /** @brief Constructor */
	        SwRecordDataCodecAdapter();
	        /** @brief Destructor */
	        virtual ~SwRecordDataCodecAdapter();
            /* @brief ajoute un sous codec */
            virtual void addCodec(ISwRecordDataCodec * codec);
            /* @brief destruction du codec */
            virtual void destroy();
            /* @brief renvoie le nom du header xml */
            virtual QString getXmlName();
        };

    }
    
}
#endif
