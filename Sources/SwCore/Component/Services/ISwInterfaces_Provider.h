/*!
\file ISwInterfaces_Provider.h
\date 12/04/2006
\brief service fournisseur d'interfaces
\author  Big
\version 1.0
 */

#ifndef _ISwInterfaces_Provider_H
#define _ISwInterfaces_Provider_H
/*
  * INCLUDES GLOBAUX
 */
#include <QString>
/*
  * INCLUDES LOCAUX
 */
#include "ISwInterfaces_Service.h"
#include "ISwHost.h"

//Nom du service
#define CG_SW_SERVICE_INTERFACES_PROVIDER "InterfacesProvider"


namespace StreamWork 
{
    namespace SwCore
    {
        class ISwInterfaces_Consumer;
        /*!
        @interface ISwInterfaces_Provider
        @brief service fournisseur d'interfaces
        @ingroup SwCoreGrp
        @swinterface
        */
        class ISwInterfaces_Provider : public ISwInterfaces_Service {
        public:
	        /*! \brief Enregistre une interface a fournir */
            template<typename T> inline void RegisterProvidedInterface(QString pinterface_name,T * handle_interface) {
                RegisterProvidedInterfaceWithType(pinterface_name, QStringLiteral("%1 *").arg(QString(typeid(T).name())),(void *)handle_interface);
            }
	        /*! \brief Enregistre une interface a fournir préférer le methode precedente*/
            virtual void RegisterProvidedInterfaceWithType(QString pinterface_name,QString pinterface_type,void * handle_interface)  throw(SwException)=0;
	        /*! \brief Desenregistre une interface fournie */
            virtual void UnregisterProvidedInterface(QString pinterface_name) throw(SwException)=0;
	        /*! \brief Rend disponible une interface */
            virtual void SetInterfaceAvailable(QString pinterface_name,void * new_handle=NULL) throw(SwException)=0;
	        /*! \brief Rend undisponible une interface */
            virtual void SetInterfaceUnavailable(QString pinterface_name) throw(SwException)=0;
 	        /*! \brief Remplacer une interface (DP strategie capability)*/
            template<typename T> void SubstituteInterface(QString pinterface_name,T * handle_interface) {
                SubstituteInterfaceWithType(pinterface_name,QString(typeid(handle_interface).name()),(void *)handle_interface);
            }
 	        /*! \brief Remplacer une interface (DP strategie capability) préférer le methode precedente*/
            virtual void SubstituteInterfaceWithType(QString pinterface_name,QString pinterface_type,void * handle_interface) throw(SwException)=0;

            /*! \brief Ajoute un consommateur a une interface donnée (est appelé par le consommateur)*/
	        virtual void AddConsumer(ISwInterfaces_Consumer * consumer,QString cinterface_name,QString pinterface_name)=0;
	        /*! \brief Supprime un consommateur d'une interface donnée (est appelé par le consommateur)*/
	        virtual void RemoveConsumer(ISwInterfaces_Consumer * consumer,QString cinterface_name,QString pinterface_name)=0;
	        /*! \brief Permet de recuperer le type de l'interface (via typeid)*/
	        virtual QString GetInterfaceType(QString)=0;
            /*! \brief Permet de savoir si une interface existe*/
            virtual bool InterfaceExist(QString)=0;
            /*! \brief Permet de connaitre la premiere interface( si retour isNull, plus d'interface) */
            virtual QString GetFirstInterface()=0;
            /*! \brief Permet de connaitre l'interface suivante ( si retour isNull, plus d'interface) */
            virtual QString GetNextInterface()=0;
             /*! \brief Permet de connaitre le premier consommateur et son interface consommé pour une interface( si retour NULL, plus de consommateur) */
            virtual ISwInterfaces_Consumer * GetFirstConsumer(QString pinterface_name,QString * pt_cinterfacename)=0;
             /*! \brief Permet de connaitre le consommateur suivant et son interface consommé pour une interface( si retour NULL, plus de consommateur) */
            virtual ISwInterfaces_Consumer * GetNextConsumer(QString * pt_cinterfacename)=0;
            //---------------------------------------------------------------------
            // Interface ISwService
            //---------------------------------------------------------------------
            /*! \brief Renvoie le nom du service
            \return le nom du service */
            QString GetServiceName(){return QString(CG_SW_SERVICE_INTERFACES_PROVIDER); }
            /*! \brief Renvoie le nom du service reel (le nom de l'interface)
            \return le nom du service reel (le nom de l'interface)
            \note au niveau de l'interface (pas d'une de ses classes derivées) renvoyer typeid(*this).name()*/
            QString GetServiceRealName() {return QString(typeid(*this).name());}    
            
        };
    }
}

#endif
