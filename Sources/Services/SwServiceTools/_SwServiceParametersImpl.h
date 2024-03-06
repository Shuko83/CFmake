/**
@file _SwServiceParametersImpl.h
@brief Parameters service implementation
@author Nicolas Dubooys
*/

#ifndef _STREAMWORK_SWCORE__SWSERVICEPARAMETERSIMPL_H
#define _STREAMWORK_SWCORE__SWSERVICEPARAMETERSIMPL_H

#include "ISwServiceParameters.h"
#include <QMap>

namespace StreamWork 
{
    namespace SwCore 
    {
        /**
        @class _SwServiceParametersImpl
        @brief Parameters service implementation
        */
        class _SwServiceParametersImpl : public ISwServiceParameters 
        {
        public:
            /** @brief Constructor */
            _SwServiceParametersImpl();
            /** @brief Destructor */
            virtual ~_SwServiceParametersImpl();
            /*! \brief Permet d'enregistrer un parametre avec sa valeur */
            void registerParameter (QString parameter, QString value);            
            /*! \brief Permet de savoir si un parametre est enregistre et récupérer sa valeur le cas echeant */
            bool getParameter (QString parameter, QString & value);

        private:
            QMap<QString, QString> _parameters;        
        };
    }
}

#endif
