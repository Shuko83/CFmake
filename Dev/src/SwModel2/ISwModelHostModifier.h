/**
@file ISwModelHostModifier.h
@brief Interface de modification du model host
@author F.Bighelli
 */

#ifndef _STREAMWORK_SWMODEL_ISWMODELHOSTMODIFIER_H
#define _STREAMWORK_SWMODEL_ISWMODELHOSTMODIFIER_H

#include <QtCore>
#include <SwComponent_Class.h>


namespace StreamWork {

    namespace SwModel {
    
        /**
        @class ISwModelHostModifier
        @brief Interface de modification du model host
        */
        class ISwModelHostModifier {
        public:
            /*! \brief Add provider interface*/
            virtual void addProviderInterface(QString name,SwCore::SwComponent_Class * component)=0;        
            /*! \brief Add consumer interface*/
            virtual void addConsumerInterface(QString name,SwCore::SwComponent_Class * component)=0;      
        };
    
    }

}

#endif
