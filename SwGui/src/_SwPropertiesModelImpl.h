/*!
 \file _SwPropertiesModelImpl.h
 \brief Definit un modele a partir d'une liste de propriétés d'un composant
 \date 02-octobre-2006 16:04:34
 \author F.Bighelli
*/

#ifndef __SwPropertiesModelImpl_H
#define __SwPropertiesModelImpl_H

/*
  * INCLUDES GLOBAUX
  */
#include <QAbstractItemModel>
#include <QMap>
#include <QString>
/*
  * INCLUDES LOCAUX
  */
#include "Component/Base/SwComponent_Class.h"
#include "Properties/ISwProperty.h"
#include "Component/Services/ISwProperties.h"
#include "Component/Services/ISwController.h"
#include "ISwQAbstractItemViewSlots.h"
namespace StreamWork
{
	namespace SwGui
	{
        /*!
		 \class _SwPropertiesModelImpl 
		 \brief Definit un modele a partir d'une liste de propriétés d'un composant
		*/
        class _SwPropertiesModelImpl : public QAbstractItemModel , public ISwQAbstractItemViewSlots{ 
			Q_OBJECT
        protected:
            /*! \brief Item representant une propriété */
            class PropertyItem {
            public:
                /*! \brief model hote */
                _SwPropertiesModelImpl * _host;
                /*! \brief parent */
                PropertyItem * _parent;
                /*! \brief enfants */
                QMap<QString,PropertyItem *> _childrens;
                /*! \brief interface propriété*/
                SwCore::ISwProperty * _property;
                /*! \brief interface controller*/
                SwCore::ISwController * _controller;
                /*! \brief interface propriétés du controller*/
                SwCore::ISwProperties * _controller_properties;
                /*! \brief label si la propriété n'est pas définit*/
                QString _label;
                /*! \brief constructeur par interface*/
                PropertyItem(_SwPropertiesModelImpl * host,QString label,SwCore::ISwProperty * property);
                /*! \brief constructeur par controller*/
                PropertyItem(_SwPropertiesModelImpl * host,SwCore::ISwController * controller);
                /*! \brief constructeur par nom*/
                PropertyItem(_SwPropertiesModelImpl * host,QString label);
                /*! \brief Destructeur */
                virtual ~PropertyItem();
                /*! \brief Sur changement de la propriété */
                void OnPropertyChange(SwCore::ISwProperty * property);
            };
        protected:
            /* Handle Propriétés */
            SwCore::ISwProperties * _properties;
            /*! \brief changement en cours */
            bool _change_in_progress;
            /* Root Item */
            PropertyItem * _root_item;
            /* Action item */
            PropertyItem * _action_item;
            /* Map des PropertyItem relatif aux controlleurs */
            QMap<SwCore::ISwController *,PropertyItem *> _map_controller_to_item;
            /* Map des PropertyItem relatif aux properties de controlleurs */
            QMap<SwCore::ISwProperties *,PropertyItem *> _map_properties_to_item;
        protected:
			/*! \brief creation d'un item */
            void CreateItem(SwCore::ISwProperty * property);
			/*! \brief destruction d'un item */
            void DestroyItem(SwCore::ISwProperty * property);
		public:
			/*! \brief constructeur */
			_SwPropertiesModelImpl(QObject * parent);
			/*! \brief destructeur */
			_SwPropertiesModelImpl();
			/*! \brief definit les propriétés */
            void SetProperties(SwCore::ISwProperties * properties);
			/*! \brief callback sur changement de controle */
            void OnControlChange(SwCore::ISwProperty * property,SwCore::ISwController *new_controller,SwCore::ISwController *old_controller);
			/*! \brief callback avant changement */
            void OnBeforeChange(SwCore::ISwProperties * properties);
			/*! \brief callback apres changement */
            void OnAfterChange(SwCore::ISwProperties * properties);
			/*! \brief callback sur creation de propriétés */
            void OnCreateProperty(SwCore::ISwProperties * properties,SwCore::ISwProperty * property);
			/*! \brief ccallback sur destruction de propriétés*/
            void OnDestroyProperty(SwCore::ISwProperties * properties,SwCore::ISwProperty * property);
            //-------------------------------------------------------------
            // Interface QAbstractItemModel
            //------------------------------------------------------------
            /*! \brief Renvoie les capacites du modèle */
            virtual Qt::ItemFlags flags ( const QModelIndex & index ) const;
            /*! \brief Renvoie le nombre de colonnes pour les enfants d'un parent donné */
            virtual int columnCount ( const QModelIndex & parent = QModelIndex() ) const;			
             /*! \brief Renvoie le nombre de ligne pour un parent donné */
            virtual int rowCount ( const QModelIndex & parent = QModelIndex() ) const;
            /*! \brief Renvoie les données d'entete */
            virtual QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
            /*! \brief Renvoie les données stockées sous un certain role pour un item nommé index */
            virtual QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;
            /*! \brief Permet de definir la valeur d'un item*/
            virtual bool setData ( const QModelIndex & index, const QVariant & value, int role = Qt::EditRole );
            /*! \brief Renvoie l'item index specifie par la ligne et la colonne pour un parent donné*/
            virtual QModelIndex index ( int row, int column, const QModelIndex & parent = QModelIndex() ) const;
            /*! \brief Renvoie l'item parent d'un item index donné */
            virtual QModelIndex parent ( const QModelIndex & index ) const;
            //-------------------------------------------------------------
            // Interface ISwQAbstractItemViewSlots
            //------------------------------------------------------------
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
            /*! \brief sur evenement sourie quelconque*/
			virtual void onMouseEvent(QMouseEvent * mouse_event);
            /*! \brief sur demande de l'aide (F1)*/
			virtual void onHelp();
            /*! \brief OnMenuRequest*/
            virtual void onMenuRequested( const QPoint & globalpos,QModelIndex index);
        public slots:
            /*! \brief OnDetachController*/
            void OnDetachController(QAction * a);
            /*! \brief OnAttachController*/
            void OnAttachController(QAction * a);
		};
	}
}
#endif 
