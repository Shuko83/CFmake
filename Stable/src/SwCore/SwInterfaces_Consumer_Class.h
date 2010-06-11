/*!
\file SwInterfaces_Consumer_Class.h
\date 12/04/2006
\brief implementation du service consommateur d'interfaces
\author  Big
\version 1.0
 */

#ifndef _SwInterfaces_Consumer_Class_H
#define _SwInterfaces_Consumer_Class_H
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
#include "ISwInterfaces_Consumer.h"
#include "ISwInterfaces_ConsumerObserver.h"
#include "ISwFinalizer.h"

namespace StreamWork
{
    namespace SwCore
    {
        class _SwConsumedInterfaceContainer_Class;
        /*!
        \class SwInterfaces_Consumer_Class
        \brief implementation du service consommateur d'interfaces
        */
        class BUILD_SWCORE SwInterfaces_Consumer_Class : public ISwInterfaces_Consumer , public ISwFinalizer{
        private:
            /*! \brief composant hote */
            SwComponent_Class * _host_component;
            /*! \brief banque des interfaces consommées */
            QMap<QString,_SwConsumedInterfaceContainer_Class *> _interfaces;
            /*! \brief iterateur courant */
            QMap<QString,_SwConsumedInterfaceContainer_Class *>::iterator _current_interface;
            /*! \brief banque des interfaces a finalize (pour le load) */
            QMap<quint64,_SwConsumedInterfaceContainer_Class *> _finalize_interfaces;
            /*! \brief liste des observers du services de consommation*/
            QSet<ISwInterfaces_ConsumerObserver *> _observers;
            /*! \brief liste des observers des services d'interface */
            QSet<ISwInterfaces_ServicesListener *> _services_observers;
        public:
	        /*! \brief Construsteur */
	        SwInterfaces_Consumer_Class(SwComponent_Class * host);
	        /*! \brief Destrusteur */
	        virtual ~SwInterfaces_Consumer_Class();
	        /*! \brief Enregistre une interface a fournir */
            template<typename T> inline void RegisterConsumedInterface(QString cinterface_name,T ** handle_handle_interface) {
                T * named_handle;
				named_handle=0;
                RegisterConsumedInterfaceWithType(cinterface_name,QString(typeid(named_handle).name()),(void **)handle_handle_interface);
            }
	        /*! \brief Enregistre une interface a fournir (utliser de préférence la methode précédente)*/
            void RegisterConsumedInterfaceWithType(QString cinterface_name,QString cinterface_type,void ** handle_handle_interface);
	        /*! \brief Desenregistre une interface fournie */
            void UnregisterConsumedInterface(QString cinterface_name);
	        /*! \brief Permet de savoir si une interface est disponible */
            bool IsInterfaceAvailable(QString cinterface_name);
	        /*! \brief Attache un observateur du consommateur d'interface */
            void AttachInterfacesConsumerObserver(ISwInterfaces_ConsumerObserver * observer);
	        /*! \brief Detache un observateur du consommateur d'interface */
            void DetachInterfacesConsumerObserver(ISwInterfaces_ConsumerObserver * observer);
            //---------------------------------------------------------------------
            // Interface ISwInterfaces_Consumer
            //---------------------------------------------------------------------
	        /*! \brief Attache un fournisseur a une interface donnée */
	        void AttachProvider(ISwInterfaces_Provider * provider,QString cinterface_name,QString pinterface_name);
	        /*! \brief Detache un fournisseur d'une interface donnée */
	        void DetachProvider(QString cinterface_name);
	        /*! \brief Fournit l'interface (est appelé par le fournisseur)*/
	        void ProvideInterface(QString cinterface_name,void * handle_interface);
	        /*! \brief Ne fournit plus l'interface (est appelé par le fournisseur)*/
	        void UnprovideInterface(QString cinterface_name);
	        /*! \brief Permet d'acceder a la premier interface consommé
            (renvoie aussi le type, le fournisseur et le nom de l'interface fournie)
            Quand la chaine renvoye Null plus d'interface)*/
	        QString GetFirstInterface(QString * nametype,ISwInterfaces_Provider ** pt_provider,QString * pt_pinterface_name);
	        /*! \brief Permet d'acceder a l'interface consommé suivante
            (renvoie aussi le type, le fournisseur et le nom de l'interface fournie)
            Quand la chaine renvoye Null plus d'interface)*/
	        QString GetNextInterface(QString * nametype,ISwInterfaces_Provider ** pt_provider,QString * pt_pinterface_name);
            //---------------------------------------------------------------------
            // Interface ISwInterfaces_Service
            //---------------------------------------------------------------------
	        /*! \brief Ajout d'un listener */
	        virtual void AttachInterfacesServices_Listener(ISwInterfaces_ServicesListener * listener);
	        /*! \brief Retrait d'un listener */
	        virtual void DetachInterfacesServices_Listener(ISwInterfaces_ServicesListener * listener);
	        /*! \brief Permet de recuperer le type de l'interface consomme(via typeid)*/
	        QString GetInterfaceType(QString interface_name);
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
            //---------------------------------------------------------------------
            // Interface ISwpersistent
            //---------------------------------------------------------------------
 	        /*! \brief methode permettant de charger des donnees */
	        void Load(QDomElement &elt,ISwFinalizerManager & finalizer_manager);
	        /*! \brief methode permettant de sauver des donnees */
	        void Save(QDomElement &elt,QDomDocument &doc);
             //---------------------------------------------------------------------
            // Interface ISwFinalizer
            //---------------------------------------------------------------------
             /*! \brief finalize l'operation correspondant a l'index d'historique
            \return false si la finalisation n'a pas eu lieu et true si ok*/
	        bool Finalize(quint64 historic_index);
     };
    }
}

#endif
