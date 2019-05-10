/**
@file ISwServiceParameters.h
@brief Parameters service
@author Nicolas Duboys
 */

#ifndef _STREAMWORK_SWCORE_ISWSERVICEPARAMETERS_H
#define _STREAMWORK_SWCORE_ISWSERVICEPARAMETERS_H

#include "ISwService.h"

//Nom du service
#define CG_SW_SERVICE_PARAMETER_MANAGER "ServiceParameters"

#pragma warning(disable:4101)

namespace StreamWork 
{
    namespace SwCore 
    {
        /**
        @class ISwServiceParameters
        @brief Parameters service
        */
        class ISwServiceParameters  : public ISwService 
        {
        public:
            /*! \brief Permet d'enregistrer un parametre avec sa valeur */
            virtual void registerParameter (QString parameter, QString value) = 0;
            /*! \brief Permet de savoir si un parametre est enregistre et récupérer sa valeur le cas echeant */
            virtual bool getParameter (QString parameter, QString & value) = 0;
            
            //---------------------------------------------------------------------
            // Interface ISwService
            //---------------------------------------------------------------------
            /*! \brief Est appele uniquement par le service manager aupres duquel le service est enregistré
            lorsque ce premier se detruit ou une operation de desenregistrement du service est réalisée*/
            virtual void Liberate(){/*Rien a faire*/}   
            /*! \brief Renvoie le nom du service
            \return le nom du service */
            QString GetServiceName(){return QString(CG_SW_SERVICE_PARAMETER_MANAGER); }
            /*! \brief Renvoie le nom du service reel (le nom de l'interface)
            \return le nom du service reel (le nom de l'interface)
            \note au niveau de l'interface (pas d'une de ses classes derivées) renvoyer typeid(*this).name()*/
            QString GetServiceRealName() {return QString(typeid(*this).name());}    
        };
    }
}

#endif
