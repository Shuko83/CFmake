/*!
 \file _SwStreamNavigationActions.h
 \brief Implementations des QActions relatives a un manager de stream
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/

#ifndef __SwStreamNavigationActions_H
#define __SwStreamNavigationActions_H

#include <QObject>
#include <QAction>
#include <QLineEdit>
#include "Component/Interfaces/ISwObserver.h"
#include "Component/Services/ServiceImpl/SwInterfaces_Provider_Class.h"
#include "ISwEditorStreamNavigation.h"

using namespace StreamWork::SwCore;

namespace StreamWork {
    namespace SwEditor {
        /*!
            \class _SwStreamsNavigationActions
            \brief Implementations des QActions relatives a un manager de stream
        */
        class _SwStreamNavigationActions : public QObject, public ISwObserver
        {
            Q_OBJECT
        private:
            /*! \brief StreamsManager */
            ISwEditorStreamNavigation * _navigator;
            /* \brief service de fourniture d'interface */
            SwInterfaces_Provider_Class * _provider_service;
            /* \brief Actions */
            QAction * _go_to_parent;
            /* \brief Widget */
            QLineEdit * _current_path;
        public:
            _SwStreamNavigationActions( QObject * parent, SwInterfaces_Provider_Class * provider_service );
            ~_SwStreamNavigationActions();
            /*! \brief Attach un stream manager */
            void AttachStreamNavigator( ISwEditorStreamNavigation * navigator );
            /*! \briefdetach un stream manager */
            void DetachStreamNavigator();
            //---------------------------------------------------------------------
            // Interface ISwObserver
            //---------------------------------------------------------------------
            /*! \brief methode appelée par l'observable*/
            virtual void Update( StreamWork::SwCore::ISwObservable * sender = nullptr );
        private slots:
            /*! \brief callback _Acces au parent*/
            void OnGoToParent();
        };
    }
}

#endif // __SwEditorStreamsManagerActions_H
