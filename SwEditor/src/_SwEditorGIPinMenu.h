/*!
 \file _SwEditorGIPinMenu.h
 \brief menu pour un graphics item pin composant
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/

#ifndef _SwEditorGIPinMenu_H
#define _SwEditorGIPinMenu_H

#include <QMenu>
#include "Component/Base/SwComponent_Class.h"
#include "_SwEditorPinGraphicItem.h"

namespace StreamWork
{
	namespace SwEditor
	{
        /*!
	        \class _SwEditorGIPinMenu 
	        \brief Implementation of the Class _SwEditorGIPinMenu 
        */
        class _SwEditorGIPinMenu : public QMenu
        {
	        Q_OBJECT
		protected:
            /*! \brief composant hote*/
			SwCore::SwComponent_Class *_host_comp;
            /*! \brief related pin item*/
            _SwEditorPinGraphicItem * _pin_item;
		public:
			/*! \brief constructeur*/
            _SwEditorGIPinMenu(_SwEditorPinGraphicItem *pin_item,SwCore::SwComponent_Class * host_comp,QWidget *parent=0);
			/*! \brief destructeur*/
            ~_SwEditorGIPinMenu();
        public slots:
             /*! \brief Deconnection*/
			void Disconnect();
        };

    }
}

#endif // _SwEditorGIPinMenu_H
