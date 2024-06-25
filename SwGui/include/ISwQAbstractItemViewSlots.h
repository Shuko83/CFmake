/*!
 \file ISwQAbstractItemViewSlots.h
 \brief interface definissant des slots connectable au signaux de la vue QAbstractItemView
 \date 02-octobre-2006 16:04:34
 \author F.Bighelli
*/

#ifndef _ISwQAbstractItemViewSlots_H
#define _ISwQAbstractItemViewSlots_H

/*
  * INCLUDES GLOBAUX
  */
#include <QAbstractItemView>
#include <QMouseEvent>
#include <QPoint>


namespace StreamWork
{
	namespace SwGui
	{
        /*!
		 \class ISwQAbstractItemViewSlots 
		 \brief interface definissant des slots connectable au signaux de la vue QAbstractModelView widget
		*/
        class ISwQAbstractItemViewSlots 
		{   
		public:
            /*! \brief sur activation */
			virtual void onActivated ( const QModelIndex & index )=0;
            /*! \brief sur click */
			virtual void onClicked ( const QModelIndex & index )=0;
            /*! \brief sur double click */
			virtual void onDoubleClicked ( const QModelIndex & index )=0;
            /*! \brief sur entree */
			virtual void onEntered ( const QModelIndex & index )=0;
            /*! \brief sur  appuie de la sourie sur l'item */
			virtual void onPressed ( const QModelIndex & index )=0;
            /*! \brief sur entre dans la vue*/
			virtual void onViewportEntered ()=0;
            /*! \brief sur evenement sourie quelconque*/
			virtual void onMouseEvent(QMouseEvent * mouse_event)=0;
            /*! \brief sur demande de l'aide (F1)*/
			virtual void onHelp()=0;
            /*! \brief OnMenuRequest*/
            virtual void onMenuRequested( const QPoint & globalpos,QModelIndex index)=0;
		};
	}
}
#endif 
