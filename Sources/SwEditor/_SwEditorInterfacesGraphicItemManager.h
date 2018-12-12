/*!
\file _SwEditorInterfacesGraphicItemManager.h
\date 02/01/2007
\brief implementation du service de gestion des items graphiques représentant les interfaces
\author  Big
\version 1.0
 */

#ifndef __SwEditorInterfacesGraphicItemManager_H
#define __SwEditorInterfacesGraphicItemManager_H
/*
  * INCLUDES GLOBAUX
 */
#include <QString>
#include <QMap>
#include <QGraphicsItem>
/*
  * INCLUDES LOCAUX
 */
#include <ISwService.h>
#include <ISwInterfaces_Service.h>
#include <ISwInterfaces_Provider.h>
#include <ISwInterfaces_Consumer.h>
#include <ISwHost.h>
#include <ISwPersistent.h>
#include <ISwFinalizer.h>
#include <ISwInterfaces_ServicesListener.h>

#include "_SwEditorGraphicItem.h"
#include "_SwEditorInterfaceGraphicItem.h"
#include "ISwEditorInterfacesGraphicItemManager.h"
#include "_SwEditorInterfacesLayerManager.h"


namespace StreamWork
{
    namespace SwEditor
    {
        /*!
        \class _SwEditorInterfacesGraphicItemManager
        \brief implementation du service de gestion des items graphiques représentant les interfaces
        */
        class _SwEditorInterfacesGraphicItemManager : public ISwEditorInterfacesGraphicItemManager, public SwCore::ISwInterfaces_ServicesListener {
        protected:
            /*! \brief Composant hote */
            SwCore::SwComponent_Class * _host_comp;
            /*! \brief GraphicsItem relatif au parent */
            _SwEditorGraphicItem * _host_item;
            /*! \brief Service fournisseur */
            SwCore::ISwInterfaces_Service * _provider_service;
            SwCore::ISwInterfaces_Provider * _provider;
            /*! \brief Service consommateur */
            SwCore::ISwInterfaces_Service * _consumer_service;
            SwCore::ISwInterfaces_Consumer * _consumer;
            /*! \brief liste des item interfaces consommes */
            QMap<QString,_SwEditorInterfaceGraphicItem *> _consumed_gitems;
            /*! \brief liste des item interfaces produites */
            QMap<QString,_SwEditorInterfaceGraphicItem *> _provided_gitems;
            /*! \brief Offset de generation des positions linaires automatiques */
            double _current_linear_position;
            /*! \brief Acces a l'interface layer manager*/
            _SwEditorInterfacesLayerManager * _il_manager;
            /*! \brief Definition de l'index d'historique*/
            quint64 _history_index;
            /*! \brief Structure de stockage des données temporaires des graphics items*/
            class _TmpGIData {
            public:
                double ppos;
                QPointF epos;
                _TmpGIData(double rppos,QPointF repos=QPointF()) {ppos=rppos;epos=repos;}
            };
            /*! \brief liste des info temporaires des items interfaces consommes */
            QMap<QString,_TmpGIData *> _consumed_tmp_items;
            /*! \brief liste des info temporaires  des item interfaces produites */
            QMap<QString,_TmpGIData *> _provided_tmp_items;
        public:
            /*! \brief Constructeur */
            _SwEditorInterfacesGraphicItemManager();
            /*! \brief Destructeur */
            virtual ~_SwEditorInterfacesGraphicItemManager();
            /*! \brief Initialisation */
            void Initialize(SwCore::SwComponent_Class * host_comp,_SwEditorGraphicItem * host_item,_SwEditorInterfacesLayerManager * il_manager);
            /*! \brief Acces a l'index d'historique */
            quint64 GetHistoryIndex();
            //---------------------------------------------------------------------
            // Interface ISwEditorInterfacesGraphicItemManager
            //---------------------------------------------------------------------
            /*! \brief Renvoie l'item graphique relatif a une interface fournit */
            virtual _SwEditorInterfaceGraphicItem * GetItemForProvidedInterface(QString name);
            //---------------------------------------------------------------------
            // Interface ISwInterfaces_ServicesListener
            //---------------------------------------------------------------------
	        /*! \brief Sur ajout d'une nouvelle interface */
	        void OnAddInterface(SwCore::ISwInterfaces_Service * source,QString interface_name);
	        /*! \brief Sur suppression d'une  interface */
	        void OnRemoveInterface(SwCore::ISwInterfaces_Service * source,QString interface_name);
	        /*! \brief Sur connection d'une interface */
	        void OnConnectInterface(SwCore::ISwInterfaces_Service * source,QString interface_name,SwCore::ISwInterfaces_Service * remote_source,QString remote_interface_name);
	        /*! \brief Sur deconnection d'une interface */
	        void OnDisconnectInterface(SwCore::ISwInterfaces_Service * source,QString interface_name,SwCore::ISwInterfaces_Service * remote_source,QString remote_interface_name);
            /*! \brief Nettoyage du manager*/
            void Liberate();
            //---------------------------------------------------------------------
            // Persistent aspect
            //---------------------------------------------------------------------
	        /*! \brief methode permettant de charger des donnees
            \return la valeur de finalisation attendu*/
	        quint64 Load(QDomElement & elt);
	        /*! \brief methode permettant de sauver des donnees */
			void Save(QXmlStreamWriter& writer);
	        /*! \brief Finalisation */
	        void Finalize();
        };
    }
}

#endif
