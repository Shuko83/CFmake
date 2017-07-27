/*!
 \file _SwStreamsActions.h
 \brief Implementations des QActions relatives a un manager de stream
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/

#ifndef __SwStreamsActions_H
#define __SwStreamsActions_H

#include <QObject>
#include <QAction>
#include <ISwObserver.h>
#include <ISwAction.h>
#include <SwInterfaces_Provider_Class.h>
#include "ISwEditorStreamsManager.h"

using namespace StreamWork::SwCore;
using namespace StreamWork::SwGui;

namespace StreamWork
{
	namespace SwEditor
	{
        /*!
	        \class _SwStreamsActions 
	        \brief Implementations des QActions relatives a un manager de stream
        */
		class _SwStreamsActions : public QObject , public ISwObserver
		{
			Q_OBJECT
        private:
            /*! \brief container d'actions */
            class _ActionContainer : public ISwAction {
            private:
                QAction * _action;
            public:
                _ActionContainer(QAction * action);
                /*! \brief Renvoie le Action
                \return le Action */
			    virtual QAction & GetAction();
            };
            /*! \brief StreamsManager */
            ISwEditorStreamsManager * _manager;
            /* \brief service de fourniture d'interface */
            SwInterfaces_Provider_Class * _provider_service;
            /* \brief Actions */
            QAction * _new;
            QAction * _open;
            QAction * _save;
            QAction * _saveas;
            QAction * _close;
            QAction * _quit;
            _ActionContainer * _new_c;
            _ActionContainer * _open_c;
            _ActionContainer * _save_c;
            _ActionContainer * _saveas_c;
            _ActionContainer * _close_c;
            _ActionContainer * _quit_c;
            bool _quit_in_progress;
		public:
		    _SwStreamsActions(QObject *parent,SwInterfaces_Provider_Class * provider_service);
		    ~_SwStreamsActions();
            /*! \brief Attach un stream manager */
            void AttachStreamsManager(ISwEditorStreamsManager * manager);
            /*! \briefdetach un stream manager */
            void DetachStreamsManager();
            //---------------------------------------------------------------------
            // Interface ISwObserver
            //---------------------------------------------------------------------
	        /*! \brief methode appelée par l'observable*/
			virtual void Update(StreamWork::SwCore::ISwObservable* sender = nullptr);
		private slots:
	        /*! \brief callback _new*/
	        void OnNew();
	        /*! \brief callback _open*/
	        void OnOpen();
	        /*! \brief callback _save*/
	        void OnSave();
	        /*! \brief callback _save*/
	        void OnSaveAs();
	        /*! \brief callback _close*/
	        void OnClose();
	        /*! \brief callback _quit*/
	        void OnQuit();

		    
		};
	}	
}

#endif // __SwEditorStreamsManagerActions_H
