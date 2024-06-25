/*!
\file SwPins_Manager_Class.h
\date 12/04/2006
\brief implementation du service  de gestion des pins
\author  Big
\version 1.0
 */

#ifndef _SwPins_Manager_Class_H
#define _SwPins_Manager_Class_H

/*
  * INCLUDES GLOBAUX
 */
#include <QString>
#include <QSet>
#include <QMap>
/*
  * INCLUDES LOCAUX
 */
#include "SwCoreConstantes.h"
#include "Main/SwMacros.h"
#include "Component/Services/ISwPins_Manager.h"
#include "Component/Interfaces/ISwFinalizer.h"

namespace StreamWork
{
    namespace SwCore
    {
		/*!
        \class SwPins_Manager_Class
        \brief implementation du service  de gestion des pins
        @ingroup SwCoreGrp
        */
        class BUILD_SWCORE SwPins_Manager_Class : public ISwPins_Manager, public ISwFinalizer {
		protected:
			/*! \brief class de stockage pour la finalisation */
            class _SwPinDesc {
            public:
                /*! \brief source */
                QString _src;
                /*! \brief target */
                QString _tgt;
                /*! \brief path */
                QString _path;
            };
			/*! \brief composant hote */
            SwComponent_Class * _host_component;
            /*! \brief liste des observers  */
            QSet<ISwPins_ManagerListener *> _observers;
            /*! \brief liste des observers  */
            QMap<QString,SwPin *> _pins;
            /*! \brief banque des connection a finalize (pour le load) */
            QMap<quint64,_SwPinDesc *> _finalize_connections;

        	quint64 _historic_index;
		public:
	        /*! \brief Construsteur */
	        SwPins_Manager_Class(SwComponent_Class * host);
	        /*! \brief Destrusteur */
	        virtual ~SwPins_Manager_Class();
	        /*! \brief Enregistre un nouveau pin*/
	        void RegisterPin(SwPin * pin);
	        /*! \brief Desenregistre un pin existant*/
	        void UnregisterPin(SwPin *pin);
	        /*! \brief Connect un pin externe a un pin local*/
	        void ConnectRemotePinToLocalPin(QString local_pin,QString remote_pin,ISwPins_Manager * remote_manager);
	        /*! \brief Deconnecte un pin existant*/
	        void DisconnectPin(QString local_pin);
	        /*! \brief Enregistre un listener*/
	        void RegisterListener(ISwPins_ManagerListener * observer);
	        /*! \brief Desenregistre  un listener*/
	        void UnregisterListener(ISwPins_ManagerListener * observer);
	        /*! \brief Acces a la liste des plugins*/
	        QList<SwPin *> GetPinList();
	        /*! \brief Acces a la liste des pins par nom*/
	        SwPin * GetPinByName(QString name);
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
			void Save(QXmlStreamWriter& writer);
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
