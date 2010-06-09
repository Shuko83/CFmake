/*!
\file SwInterfaces_Provider_Class.h
\date 12/04/2006
\brief implementation du service fournisseur d'interfaces
\author  Big
\version 1.0
 */

#ifndef _SwInterfaces_Provider_Class_H
#define _SwInterfaces_Provider_Class_H
/*
  * INCLUDES GLOBAUX
 */
#include <QString>
#include <QMap>
#include <QSet>
/*
  * INCLUDES LOCAUX
 */
#include "SwCoreConstantes.h"
#include "SwException.h"
#include "SwMacros.h"
#include "ISwInterfaces_Provider.h"

namespace StreamWork
{
    namespace SwCore
    {
        class _SwProvidedInterfaceContainer_Class;
        /*!
        \class ISwInterfaces_Provider
        \brief implementation du service fournisseur d'interfaces
        */
        class BUILD_SWCORE SwInterfaces_Provider_Class : public ISwInterfaces_Provider {
        private:
            /*! \brief composant hote */
            SwComponent_Class * _host_component;
            /*! \brief banque des interfaces fournit */
            QMap<QString,_SwProvidedInterfaceContainer_Class *> _interfaces;
            /*! \brief iterateur de parcours */
            QMap<QString,_SwProvidedInterfaceContainer_Class *>::iterator _current_interface;
            /*! \brief iterateur de parcours */
            _SwProvidedInterfaceContainer_Class * _current_interface_container;
            /*! \brief liste des observers des services d'interface */
            QSet<ISwInterfaces_ServicesListener *> _services_observers;
        public:
	        /*! \brief Construsteur */
	        SwInterfaces_Provider_Class(SwComponent_Class * host);
	        /*! \brief Destrusteur */
	        virtual ~SwInterfaces_Provider_Class();
	        /*! \brief Enregistre une interface a fournir */
            template<typename T> inline void RegisterProvidedInterface(QString pinterface_name,T * handle_interface) {
                RegisterProvidedInterfaceWithType(pinterface_name,QString(typeid(handle_interface).name()),(void *)handle_interface);
            }
	        /*! \brief Enregistre une interface a fournir préférer le methode precedente*/
            void RegisterProvidedInterfaceWithType(QString pinterface_name,QString pinterface_type,void * handle_interface)  throw(SwException);
	        /*! \brief Desenregistre une interface fournie */
            void UnregisterProvidedInterface(QString pinterface_name) throw(SwException);
	        /*! \brief Rend disponible une interface */
            void SetInterfaceAvailable(QString pinterface_name,void * new_handle=NULL) throw(SwException);
	        /*! \brief Rend undisponible une interface */
            void SetInterfaceUnavailable(QString pinterface_name) throw(SwException);
 	        /*! \brief Remplacer une interface (DP strategie capability)*/
            template<typename T> void SubstituteInterface(QString pinterface_name,T * handle_interface) {
                SubstituteInterfaceWithType(pinterface_name,QString(typeid(handle_interface).name()),(void *)handle_interface);
            }
 	        /*! \brief Remplacer une interface (DP strategie capability) préférer le methode precedente*/
            void SubstituteInterfaceWithType(QString pinterface_name,QString pinterface_type,void * handle_interface) throw(SwException);
           //---------------------------------------------------------------------
            // Interface ISwInterfaces_Provider
            //---------------------------------------------------------------------
	        /*! \brief Ajoute un consommateur a une interface donnée */
	        void AddConsumer(ISwInterfaces_Consumer * consumer,QString cinterface_name,QString pinterface_name);
	        /*! \brief Supprime un consommateur d'une interface donnée */
	        void RemoveConsumer(ISwInterfaces_Consumer * consumer,QString cinterface_name,QString pinterface_name);
            /*! \brief Permet de savoir si une interface existe*/
            bool InterfaceExist(QString pinterface_name);
             /*! \brief Permet de connaitre la premiere interface( si retour isNull, plus d'interface) */
            QString GetFirstInterface();
            /*! \brief Permet de connaitre l'interface suivante ( si retour isNull, plus d'interface) */
            QString GetNextInterface();
             /*! \brief Permet de connaitre le premier consommateur et son interface consommé pour une interface( si retour NULL, plus de consommateur) */
            ISwInterfaces_Consumer * GetFirstConsumer(QString pinterface_name,QString * pt_cinterfacename);
             /*! \brief Permet de connaitre le consommateur suivant et son interface consommé pour une interface( si retour NULL, plus de consommateur) */
            ISwInterfaces_Consumer * GetNextConsumer(QString * pt_cinterfacename);
            //---------------------------------------------------------------------
            // Interface ISwInterfaces_Service
            //---------------------------------------------------------------------
	        /*! \brief Ajout d'un listener */
	        virtual void AttachInterfacesServices_Listener(ISwInterfaces_ServicesListener * listener);
	        /*! \brief Retrait d'un listener */
	        virtual void DetachInterfacesServices_Listener(ISwInterfaces_ServicesListener * listener);
	        /*! \brief Permet de recuperer le type de l'interface (via typeid)*/
	        QString GetInterfaceType(QString pinterface_name);
          //---------------------------------------------------------------------
            // Interface ISwService
            //---------------------------------------------------------------------
            /*! \brief Est appele uniquement par le service manager aupres duquel le service est enregistré
            lorsque ce premier se detruit ou une operation de desenregistrement du service est réalisée*/
            void Liberate();
            //---------------------------------------------------------------------
            // Interface ISwHost
            //---------------------------------------------------------------------
	        /*! \brief acces a son composant hote */
	        SwComponent_Class * GetHostComponent();
        };
    }
}

#endif
