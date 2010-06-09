/*!
 \file _SwQAbstractItemViewSlots_Catcher.h
 \brief objet permettant la capture des signaux de QAbstractModelView
 \date 02-octobre-2006 16:04:34
 \author F.Bighelli
*/

#ifndef __SwQAbstractItemViewSlots_Catcher_H
#define __SwQAbstractItemViewSlots_Catcher_H

/*
  * INCLUDES GLOBAUX
  */
#include <QAbstractItemModel>
#include <QAbstractItemView>
/*
  * INCLUDES LOCAUX
  */
#include "ISwQAbstractItemViewSlots.h"

namespace StreamWork
{
	namespace SwGui
	{
        /*!
		 \class _SwQAbstractItemViewSlots_Catcher 
		 \brief objet permettant la capture des signaux de QAbstractModelView
		*/
        class _SwQAbstractItemViewSlots_Catcher : public QObject { 
			Q_OBJECT
		private:
            /*! \brief interface contenant les handles; */
			ISwQAbstractItemViewSlots * _handle_interface;
            /*! \brief vue hote */  
            QAbstractItemView * _host;
        protected:
            /*! \brief Filtre d'evenement (envoie les etats sourie a l'interface ISwQAbstractItemViewSlots); */
            bool eventFilter(QObject *obj, QEvent *event);
		public:
			/*! \brief constructeur */
			_SwQAbstractItemViewSlots_Catcher(QAbstractItemView * host);
            /*! \brief definition de l'interface  */
			void DefineInterfaceSlots(ISwQAbstractItemViewSlots * handle_inteface);
        public slots:
			/*! \brief sur activation */
			virtual void onActivated ( const QModelIndex & index );
            /*! \brief sur click */
			virtual void onClicked ( const QModelIndex & index );
            /*! \brief sur double click */
			virtual void onDoubleClicked ( const QModelIndex & index );
            /*! \brief sur entree */
			virtual void onEntered ( const QModelIndex & index );
            /*! \brief sur  appuie de la sourie sur l'item */
			virtual void onPressed ( const QModelIndex & index );
            /*! \brief sur entre dans la vue*/
			virtual void onViewportEntered ();
            /*! \brief OnMenuRequest*/
            virtual void OnMenuRequested( const QPoint & pos);

		};
	}
}
#endif 
