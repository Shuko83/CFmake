/*!
 \file _SwStreamsTabBar.h
 \brief Implementation of the Class _SwStreamsTabBar qui surcharge une tabbar pour la gestion des streams
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/

#ifndef _SwStreamsTabBar_H
#define _SwStreamsTabBar_H

#include <QTabBar>
#include "Component/Interfaces/ISwObserver.h"
#include "ISwEditorStreamsManager.h"


namespace StreamWork
{
	namespace SwEditor
	{
        /*!
	        \class _SwStreamsTabBar 
	        \brief Implementation of the Class _SwStreamsTabBar qui surcharge une tabbar pour la gestion des streams
        */
        class _SwStreamsTabBar : public QTabBar, public SwCore::ISwObserver
        {
	        Q_OBJECT
        private:
            /*! \brief StreamsManager */
            ISwEditorStreamsManager * _manager;
            /*! \brief Activation de la reconstruction */
            bool _rebuild_enabled;
        public:
            _SwStreamsTabBar(QWidget *parent);
            ~_SwStreamsTabBar();

            /*! \brief Attach un stream manager */
            void AttachStreamsManager(ISwEditorStreamsManager * manager);
            /*! \briefdetach un stream manager */
            void DetachStreamsManager();
            /*! \brief(Re)Construction du widget */
            void BuildWidget();
            //---------------------------------------------------------------------
            // Interface ISwObserver
            //---------------------------------------------------------------------
	        /*! \brief methode appelée par l'observable*/
			virtual void Update(StreamWork::SwCore::ISwObservable* sender = nullptr);
        public slots:
	        /*! \brief sur changement de l'index*/
            void OnIndexChanged(int index);
            
        };

    }
}

#endif // _SWSTREAMSTABBAR_H
