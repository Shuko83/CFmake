/*!
\file _SwEditorPinGraphicItemManager.h
\date 02/01/2007
\brief implementation du service de gestion des items graphiques représentant les interfaces
\author  Big
\version 1.0
 */

#ifndef __SwEditorPinGraphicItemManager_H
#define __SwEditorPinGraphicItemManager_H
/*
  * INCLUDES GLOBAUX
 */
#include <QString>
#include <QMap>
#include <QGraphicsItem>
#include <QPolygonF>
/*
  * INCLUDES LOCAUX
 */
#include <ISwService.h>
#include <ISwPins_Manager.h>
#include <ISwHost.h>
#include <ISwPersistent.h>
#include <ISwFinalizer.h>
#include <ISwPins_ManagerListener.h>

#include "_SwEditorGraphicItem.h"
#include "_SwEditorPinGraphicItem.h"
#include "ISwEditorPinGraphicItemManager.h"
#include "_SwEditorPinsLayerManager.h"


namespace StreamWork
{
    namespace SwEditor
    {
        /*!
        \class _SwEditorPinGraphicItemManager
        \brief implementation du service de gestion des items graphiques représentant les interfaces
        */
        class _SwEditorPinGraphicItemManager : public ISwEditorPinGraphicItemManager, public SwCore::ISwPins_ManagerListener {
        protected:
            /*! \brief Composant hote */
            SwCore::SwComponent_Class * _host_comp;
            /*! \brief GraphicsItem relatif au parent */
            _SwEditorGraphicItem * _host_item;
            /*! \brief Service pin */
            SwCore::ISwPins_Manager * _pins_manager;
            /*! \brief liste des item pins */
            QMap<QString,_SwEditorPinGraphicItem *> _pin_gitems;
            /*! \brief Offset de generation des positions linaires automatiques */
            double _current_linear_position;
            /*! \brief Acces a pin layer manager*/
            _SwEditorPinsLayerManager * _pl_manager;
            /*! \brief Definition de l'index d'historique*/
            quint64 _history_index;
            /*! \brief Structure de stockage des données temporaires des graphics items*/
            class _TmpGPData {
            public:
                double ppos;
                QPolygonF p;
                _TmpGPData(double rppos) {ppos=rppos;}
            };
            /*! \brief liste des info temporaires des items pin */
            QMap<QString,_TmpGPData *> _tmp_items;
        public:
            /*! \brief Constructeur */
            _SwEditorPinGraphicItemManager();
            /*! \brief Destructeur */
            virtual ~_SwEditorPinGraphicItemManager();
            /*! \brief Initialisation */
            void Initialize(SwCore::SwComponent_Class * host_comp,_SwEditorGraphicItem * host_item,_SwEditorPinsLayerManager * pl_manager);
            /*! \brief Acces a l'index d'historique */
            quint64 GetHistoryIndex();
            //---------------------------------------------------------------------
            // Interface ISwEditorPinGraphicItemManager
            //---------------------------------------------------------------------
            /*! \brief Renvoie l'item graphique relatif a un pin */
            virtual _SwEditorPinGraphicItem * GetItemForPin(QString name);
            //---------------------------------------------------------------------
            // Interface ISwPins_ManagerListener
            //---------------------------------------------------------------------
	        /*! \brief Sur ajout d'un nouveau pin*/
	        virtual void OnAddPin(SwCore::SwPin * pin);
	        /*! \brief Sur suppression d'un pin existant*/
	        virtual void OnRemovePin(SwCore::SwPin * pin);
	        /*! \brief Sur connexion d'un pin*/
	        virtual void OnConnectPin(SwCore::SwPin * local_pin,SwCore::SwPin * remote_pin);
	        /*! \brief Sur deconnexion d'un pin*/
	        virtual void OnDisconnectPin(SwCore::SwPin * local_pin,SwCore::SwPin * remote_pin);
            /*! \brief Nettoyage du manager*/
            void Liberate();
            //---------------------------------------------------------------------
            // Persistent aspect
            //---------------------------------------------------------------------
	        /*! \brief methode permettant de charger des donnees
            \return la valeur de finalisation attendu*/
	        quint64 Load(QDomElement & elt);
	        /*! \brief methode permettant de sauver des donnees */
	        void Save(QDomElement & elt,QDomDocument & doc);
	        /*! \brief Finalisation */
	        void Finalize();
        };
    }
}

#endif
