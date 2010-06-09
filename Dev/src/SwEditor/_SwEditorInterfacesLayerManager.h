/*!
 \file _SwEditorInterfacesLayerManager.h
 \brief Gestionnaire des interfaces dessin�s
 \version 1.0
 \date 23-ao�t-2006 18:59:26
 \author F.Bighelli
*/

#ifndef _SwEditorInterfacesLayerManager_H
#define _SwEditorInterfacesLayerManager_H

#include <QObject>
#include <QMenu>
#include <QMap>
#include <SwComponent_Class.h>
#include <ISwInterfaces_ServicesListener.h>
#include <ISwInterfaces_Provider.h>
#include <ISwInterfaces_Consumer.h>
#include "LibIndeSig.h"

namespace StreamWork
{
	namespace SwEditor
	{
        class _SwEditorInterfaceGraphicItem;
        /*!
	        \class _SwEditorInterfacesLayerManager
	        \brief Implementation of the Class _SwEditorInterfacesLayerManager qui surcharge une tabbar pour la gestion des streams
        */
        class _SwEditorInterfacesLayerManager : public QObject, public SwCore::ISwInterfaces_ServicesListener
        {
	        Q_OBJECT
        protected:
			/*! \brief map des etats interfaces*/
            QMap<QString,QAction *> _interfaces_display;
			/*! \brief menu_principal*/
            QMenu * _interfaces_display_main_menu;
			/*! \brief action show all interfaces*/
            QAction * show_all_action;
			/*! \brief action hide all interfaces*/
            QAction * hide_all_action;
			/*! \brief liste des interfaces mise en evidence*/
            QList<_SwEditorInterfaceGraphicItem *> _highlighted_interfaces;
        public:
            /*! \brief Permet de signaler un changement de visibilite sur une interface*/
			LibIndeSig::iSignal2<const QString *,bool> OnInterfaceVisibilityChange;
            /*! \brief Permet de signaler une mise en evidence des producteur*/
			LibIndeSig::iSignal2<const QString *,bool> OnInterfaceHighlightChange;
		public:
			/*! \brief constructeur*/
            _SwEditorInterfacesLayerManager( QObject * parent = 0);
			/*! \brief destructeur*/
            ~_SwEditorInterfacesLayerManager();
            /*! \brief Ajout d'un composant a espionner */
            void AddCompToSpy(SwCore::SwComponent_Class * host_comp);
            /*! \brief Acces a l'etat d'un type d'interface */
            bool GetInterfaceDisplayState(const QString & interface_type);
            /*! \brief Acces au menu pour la gestion des interfaces */
            QMenu * GetInterfaceDisplayStateMenu();
            /*! \brief Definition de les interfaces produites a mettre en evidence */
            void SetInterfaceToHighlight(const QString & interface_type, bool set_highlight=true);
            /*! \brief Enregistrer les interfaces mises en evidence */
            void RegisterHighlightedInterfaceItem(_SwEditorInterfaceGraphicItem * h_interface);
            /*! \brief Acces a la liste des les interfaces mises en evidence */
            QList<_SwEditorInterfaceGraphicItem *> * GetHighlightedInterfaceItems();
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
        public slots:
	        /*! \brief Sur action triggered */
            void ActionTriggered(QAction * action);
        };
    }
}

#endif // _SwEditorInterfacesLayerManager_H
