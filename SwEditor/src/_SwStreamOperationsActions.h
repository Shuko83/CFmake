/*!
 \file _SwStreamOperationsActions.h
 \brief Implementations des QActions relatives a un manager de stream
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/

#ifndef __SwStreamOperationsActions_H
#define __SwStreamOperationsActions_H

#include <QObject>
#include <QAction>
#include "Component/Interfaces/ISwObserver.h"
#include "Component/Services/ServiceImpl/SwInterfaces_Provider_Class.h"
#include "ISwEditorStreamOperations.h"

using namespace StreamWork::SwCore;

namespace StreamWork
{
	namespace SwEditor
	{
        /*!
	        \class _SwStreamOperationsActions 
	        \brief Implementations des QActions relatives a un manager de stream
        */
		class _SwStreamOperationsActions : public QObject , public ISwObserver
		{
			Q_OBJECT
        private:
            /*! \brief StreamsManager */
            ISwEditorStreamOperations * _selection;
            /* \brief service de fourniture d'interface */
            SwInterfaces_Provider_Class * _provider_service;
            /* \brief Actions */
            QAction * _select_all;
            QAction * _select_none;
            QAction * _select_invert;
            QAction * _remove_selection;
            QAction * _copy_selection;
            QAction * _paste_selection;
            QAction * _cut_selection;
		public:
		    _SwStreamOperationsActions(QObject *parent,SwInterfaces_Provider_Class * provider_service);
		    ~_SwStreamOperationsActions();
            /*! \brief Attach un stream manager */
            void AttachStreamOperations(ISwEditorStreamOperations * selection);
            /*! \briefdetach un stream manager */
            void DetachStreamOperations();
            //---------------------------------------------------------------------
            // Interface ISwObserver
            //---------------------------------------------------------------------
	        /*! \brief methode appelée par l'observable*/
			virtual void Update(StreamWork::SwCore::ISwObservable* sender = nullptr);
		private slots:
	        /*! \brief callback sur select all*/
	        void OnSelectAll();
	        /*! \brief callback sur select none*/
	        void OnSelectNone();
	        /*! \brief callback sur inversion de selection*/
	        void OnSelectInvert();
	        /*! \brief callback sur suppression de la selection*/
	        void OnRemoveSelection();
	        /*! \brief callback sur copy de la selection*/
	        void OnCopySelection();
	        /*! \brief callback sur coller la selection*/
	        void OnPasteSelection();
	        /*! \brief callback sur couper la selection*/
	        void OnCutSelection();
		};
	}	
}

#endif // __SwEditorStreamsManagerActions_H
