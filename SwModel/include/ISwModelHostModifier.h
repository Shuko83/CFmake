/**
@file ISwModelHostModifier.h
@brief Interface de modification du model host
@author F.Bighelli
 */

#ifndef _STREAMWORK_SWMODEL_ISWMODELHOSTMODIFIER_H
#define _STREAMWORK_SWMODEL_ISWMODELHOSTMODIFIER_H

#include <QtCore>
#include "Component/Base/SwComponent_Class.h"


namespace StreamWork {

    namespace SwModel {
    
        /**
        @class ISwModelHostModifier
        @brief Interface de modification du model host
        */
        class ISwModelHostModifier {
        public:
            /*! \brief Add provider interface*/
            virtual QString addProviderInterface(QString name,SwCore::SwComponent_Class * component,QString itype)=0;        
            /*! \brief Add consumer interface*/
            virtual QString addConsumerInterface(QString name,SwCore::SwComponent_Class * component,QString itype)=0;      
            /*! \brief Add connector*/
            virtual QString addConnector(QString name,SwCore::SwComponent_Class * component,QString itype)=0;     
            /*! \brief update model host*/
            virtual void updateModelHost()=0;     

        };
    
    }

}

#endif
