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
#include "Component/Interfaces/ISwObserver.h"
#include "Component/Services/ServiceImpl/SwInterfaces_Provider_Class.h"
#include "ISwEditorStreamsManager.h"

using namespace StreamWork::SwCore;

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
