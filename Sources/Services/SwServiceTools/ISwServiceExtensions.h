/**
@file ISwServiceExtensions.h
@brief Extensions service
@author Big
 */

#ifndef _STREAMWORK_SWCORE_ISWSERVICEEXTENSIONS_H
#define _STREAMWORK_SWCORE_ISWSERVICEEXTENSIONS_H

#include "ISwService.h"
#include "ISwExtension.h"

//Nom du service
#define CG_SW_SERVICE_EXTENSION_MANAGER "ServiceExtensions"

#pragma warning(disable:4101)

namespace StreamWork {

    namespace SwCore {
    
        /**
        @class ISwServiceExtensions
        @brief Extensions service
        */
        class ISwServiceExtensions  : public ISwService {
        public:

            /*! \brief Permet d'enregistrer une extension*/
            template<typename T> inline void registerExtension(QString extension_name,QString component_name){
                T * handle;
                registerExtensionWithType(QString(typeid(handle).name()),extension_name,component_name);
            }
            
            /*! \brief Permet d'avoir une liste d'extension pourt un type donne*/
            template<typename T> inline QList<ISwExtension *> getExtensions(){
                T * handle;
                return getExtensionWithType(QString(typeid(handle).name()));
            }
            /*! \brief Permet d'avoir une liste d'extension pourt un type donne (utliser la methode precedente)*/
            virtual QList<ISwExtension *> getExtensionWithType(QString extension_type_name)=0;
           
            
            //---------------------------------------------------------------------
            // Interface ISwService
            //---------------------------------------------------------------------
            /*! \brief Est appele uniquement par le service manager aupres duquel le service est enregistré
            lorsque ce premier se detruit ou une operation de desenregistrement du service est réalisée*/
            virtual void Liberate(){/*Rien a faire*/}   
            /*! \brief Renvoie le nom du service
            \return le nom du service */
            QString GetServiceName(){return QString(CG_SW_SERVICE_EXTENSION_MANAGER); }
            /*! \brief Renvoie le nom du service reel (le nom de l'interface)
            \return le nom du service reel (le nom de l'interface)
            \note au niveau de l'interface (pas d'une de ses classes derivées) renvoyer typeid(*this).name()*/
            QString GetServiceRealName() {return QString(typeid(*this).name());}    

        protected:
            /*! \brief Permet d'enregistrer une extension avec le type en nom (utliser la methode precedente)*/
            virtual void registerExtensionWithType(QString extension_type_name,QString extension_name,QString pluginName,QString component_name)=0;

        };
    
    }

}

#endif
