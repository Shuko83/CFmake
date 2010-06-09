/*!
 \file _SwEditorPinsLayerManager.h
 \brief Gestionnaire des pins dessin�s
 \version 1.0
 \date 23-ao�t-2006 18:59:26
 \author F.Bighelli
*/

#ifndef _SwEditorPinsLayerManager_H
#define _SwEditorPinsLayerManager_H

#include <QObject>
#include <QMenu>
#include <QMap>
#include <SwComponent_Class.h>
#include <ISwPins_ManagerListener.h>
#include "LibIndeSig.h"

namespace StreamWork
{
	namespace SwEditor
	{
        class _SwEditorPinGraphicItem;
        /*!
	        \class _SwEditorPinsLayerManager
	        \brief Implementation of the Class _SwEditorPinsLayerManager q
        */
        class _SwEditorPinsLayerManager : public QObject, public SwCore::ISwPins_ManagerListener
        {
	        Q_OBJECT
        protected:
			/*! \brief map des etats interfaces*/
            QMap<QString,QAction *> _pins_display;
			/*! \brief menu_principal*/
            QMenu * _pins_display_main_menu;
			/*! \brief action show all interfaces*/
            QAction * show_all_action;
			/*! \brief action hide all interfaces*/
            QAction * hide_all_action;
			/*! \brief liste des pins mise en evidence*/
            QList<_SwEditorPinGraphicItem *> _highlighted_pins;
        public:
            /*! \brief Permet de signaler un changement de visibilite sur un pin*/
			LibIndeSig::iSignal2<const QString *,bool> OnPinVisibilityChange;
            /*! \brief Permet de signaler une mise en evidence des pins de meme type*/
			LibIndeSig::iSignal2<const QString *,bool> OnPinHighlightChange;
		public:
			/*! \brief constructeur*/
            _SwEditorPinsLayerManager( QObject * parent = 0);
			/*! \brief destructeur*/
            ~_SwEditorPinsLayerManager();
            /*! \brief Ajout d'un composant a espionner */
            void AddCompToSpy(SwCore::SwComponent_Class * host_comp);
            /*! \brief Acces a l'etat d'un type de pin */
            bool GetPinDisplayState(const QString & pin_type);
            /*! \brief Acces au menu pour la gestion des pins */
            QMenu * GetPinDisplayStateMenu();
            /*! \brief Definition des pins a mettre en evidence */
            void SetPinToHighlight(const QString & pin_type, bool set_highlight=true);
            /*! \brief Enregistrer les interfaces mises en evidence */
            void RegisterHighlightedPinsItem(_SwEditorPinGraphicItem * h_pin);
            /*! \brief Acces a la liste des pins mis en evidence */
            QList<_SwEditorPinGraphicItem *> * GetHighlightedPinsItems();
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
        public slots:
	        /*! \brief Sur action triggered */
            void ActionTriggered(QAction * action);
        };
    }
}

#endif // _SwEditorPinsLayerManager_H
