/*!
 \file _SwEditorGIInterfaceMenu.h
 \brief menu pour un graphics item interface composant
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/

#ifndef _SwEditorGIInterfaceMenu_H
#define _SwEditorGIInterfaceMenu_H

#include <QMenu>
#include "Component/Base/SwComponent_Class.h"
#include "_SwEditorInterfaceGraphicItem.h"


namespace StreamWork
{
	namespace SwEditor
	{
        /*!
	        \class _SwEditorGIInterfaceMenu 
	        \brief Implementation of the Class _SwEditorGIInterfaceMenu 
        */
        class _SwEditorGIInterfaceMenu : public QMenu
        {
	        Q_OBJECT
		protected:
            /*! \brief interface operations*/
			SwCore::SwComponent_Class *_host_comp;
            /*! \brief interface navigation*/
            _SwEditorInterfaceGraphicItem * _interface_item;
		public:
			/*! \brief constructeur*/
            _SwEditorGIInterfaceMenu(_SwEditorInterfaceGraphicItem *interface_item,SwCore::SwComponent_Class * host_comp,QWidget *parent=0);
			/*! \brief destructeur*/
            ~_SwEditorGIInterfaceMenu();
        public slots:
             /*! \brief Deconnectiont*/
			void Disconnect();
        };

    }
}

#endif // _SwEditorGIInterfaceMenu_H
