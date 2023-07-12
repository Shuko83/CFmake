/*!
 \file SwServicesManager_Class.h
 \brief Implementation of the Class SwServicesManager_Class
 \version 1.0
 \date 23-août-2006 16:04:57
 \author F.Bighelli
*/

#ifndef _SwServicesManager_Class_H
#define _SwServicesManager_Class_H

/*
  * INCLUDES GLOBAUX
  */
#include <QString>
#include <QList>
#include <QMap>
/*
  * INCLUDES LOCAUX
  */
#include "SwException.h"
#include "ISwService.h"
#include "ISwServicesManager_Listener.h"

namespace StreamWork
{
	namespace SwCore
	{
        /*!
		 \class SwServicesManager_Class 
		 \brief Manager de service
        @ingroup SwCoreGrp

        Permet d'enregistrer des services auprès de l'entité implémentant cette classe
        Cette classe implemente une collection de services.

        Les methodes de la classe permettent l'exploitation de la collection

        Pour enregistrer un nouveau service:
        \code    
        try {
            RegisterService(my_service);
        } catch (SwException &e) {
            //Si on est la c'est qu'un service avec le même nom est deja enregistrer
        }
        \endcode
        Pour desenregistrer un service existant:
        \code    
        try {
            UnregisterService(my_service_name);
        } catch (SwException &e) {
            //Si on est la c'est qu'aucun service enregistré a le nom passé en paramètre
        }
        \endcode
        
        Pour acceder a la liste des noms (renvoyer par GetServiceName()) des services:
        \code
        QList<QString> services_list=GetServicesList();
        \endcode

        Pour acceder a un service dont on connait le nom:
        \code
        ISwService * my_service=QueryService(my_service_name);
        if (my_service==NULL) {
            //Le service dont le nom est passé en paramètre n'existe pas
        }
        \endcode

        Pour liberer tous les services:
        \code
        LiberateAllServices();
        \endcode
        Est theoriquement appelé dans le destructeur de l'entité implementant cette classe

		*/
		class BUILD_SWCORE SwServicesManager_Class
		{
        private:
            /*! \brief QMap des services */
            QMap<QString,ISwService *> _services;
            /*! \brief fifo des services */
            QList<QString> _ordered_services;
            /*! \brief liste des observers */
            QList<ISwServicesManager_Listener *> _servicesObservers;
            /*! \brief Set de set permettant de garder en mémoire les observers delete par la méthode
            onUnregisterService. Le set de set permet une méthode réentrante.*/
            QSet < QSet<ISwServicesManager_Listener *>* > _deletedObservers;
		public:
            /*! \brief Constructeur */
            SwServicesManager_Class();
            /*! \brief Destructeur */
            ~SwServicesManager_Class();
            /*! \brief Renvoie la liste des noms des services enregistrés */
			QList<QString> GetServicesList();
			/*! \brief Demande un service*/
			ISwService * QueryService(QString service_name);
			/*! \brief Enregistre un service*/
			void RegisterService(ISwService * service) throw(SwException);
			/*! \brief Desenregistre un service*/
			void UnregisterService(QString service_name) throw(SwException);
			/*! \brief Liberation de tous les services enregistres*/
			void LiberateAllServices();
            /*! \brief ajoute un observer */
            void AddServicesManagerObserver(ISwServicesManager_Listener * observer);
            /*! \brief supprime un observer */
            void RemoveServicesManagerObserver(ISwServicesManager_Listener * observer);

		protected:
			virtual void raiseQueryError(QString serviceName){};
		};
	}
}
#endif 
