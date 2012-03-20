/**
@file SwModelsListAccess.h
@brief Simgleton d'access a la liste des models
@author F.Bighelli
 */

#ifndef _STREAMWORK_SWMODEL_SWMODELSLISTACCESS_H
#define _STREAMWORK_SWMODEL_SWMODELSLISTACCESS_H

#include "SwModel2Constantes.h"

class _SwModelsList;

namespace StreamWork {

    namespace SwModel {
    
        /**
        @class SwModelsListAccess
        @brief Simgleton d'access a la liste des models
        */
        class SWMODEL2_EXPORT SwModelsListAccess {
        public:
            /** @brief instance access */
            static SwModelsListAccess * getInstance();
       
            /** @brief internal access (backup compatibility) */
            _SwModelsList * getInternal();
            
            /** @brief internal access (backup compatibility) */
            QString getModelsDirectory();
            /** @brief add model */
            void addModel(QString hostComponent,QString modelName);
        private:            
            /** @brief Constructor */
            SwModelsListAccess();
            /** @brief Destructor */
            virtual ~SwModelsListAccess();
            
        };
    
    }

}

#endif
