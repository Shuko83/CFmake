/**
 * file : SwPropertiesModelImpl.h
 * brief : Definit un modele a partir d'une liste de propriétés d'un composant
 *			(pour l'affichage dans un QTreeView)
 * date : 02-octobre-2006 16:04:34
 * author : F.Bighelli - modified by CGD (12/09/2013)
*/

#ifndef _SwPropertiesModelImpl_H
#define _SwPropertiesModelImpl_H

/*
  * INCLUDES GLOBAUX
  */
#include <QAbstractItemModel>
#include <QMap>
#include <QString>
/*
  * INCLUDES LOCAUX
  */
#include <SwComponent_Class.h>
#include <ISwProperty.h>
#include <ISwProperties.h>
#include "SwCoreConstantes.h"
#include "ISwConfPropertiesObserver.h"


namespace StreamWork
{
	namespace SwCore
	{
        /**
		 * class : SwPropertiesModelImpl 
		 * brief : Definit un modele a partir d'une liste de propriétés d'un composant
         * @ingroup SwCoreGrp
		*/
        class BUILD_SWCORE SwPropertiesModelImpl : public QAbstractItemModel,
													public ISwConfPropertiesObserver
		{ 
			Q_OBJECT

		public:
			/** @brief : Structure pour chaque proerty Starlinx, opérateur == réimplémenté pour pouvoir utiliser indexOf() de QList.
			 **			structure utilisée dans la classe ConfigurationToTreeViewWidget de la conf 
			 */
			typedef struct StarlinxProperty{
				QString prefix;
				QString propertyName;
				int order;
				ISwProperty* property;
 
 				bool operator==( const StarlinxProperty& source) const
  				{
  					if(source.order == order
  						&& source.prefix == prefix
  						&& source.property == property
  						&& source.propertyName == propertyName)
  						return true;
  					else
  						return false;
  				}
			};

        protected:
            /** @brief : Item representant une propriété */
            class PropertyItem {
            public:
                /** @brief : model hote */
                SwPropertiesModelImpl * _host;
                /** @brief : parent */
                PropertyItem * _parent;
                /** @brief : enfants */
                QMap<QString,PropertyItem *> _childrens;
                /** @brief : enfants */
                QList<PropertyItem *> _showChildrens;
                /** @brief : interface propriété*/
                SwCore::ISwProperty * _property;
                /** @brief : label si la propriété n'est pas définit*/
                QString _label;
                /** @brief : constructeur par interface*/
                PropertyItem(SwPropertiesModelImpl * host,QString label,SwCore::ISwProperty * property);
                /** @brief : constructeur par nom*/
                PropertyItem(SwPropertiesModelImpl * host,QString label);
                /** @brief : Destructeur */
                virtual ~PropertyItem();
                /** @brief : Sur changement de la propriété */
                void OnPropertyChange(SwCore::ISwProperty * property);
            };

        protected:
            /** @brief :  Handle Propriétés */
            SwCore::ISwProperties * _properties;
            /** @brief : brief changement en cours */
            bool _change_in_progress;
            /** @brief : Root Item */
            PropertyItem * _root_item;
            /** @brief : Action item */
            PropertyItem * _action_item;
            /** @brief : Map des PropertyItem relatif aux properties de controlleurs */
            QMap<ISwProperties *,PropertyItem *> _map_properties_to_item;
            /** @brief : Label du root */
            QString _rootLabel;

        protected:
			/** @brief : creation d'un item */
            void CreateItem(SwCore::ISwProperty * property, QString customConstructedPropertyName = "");

			/** @brief : destruction d'un item */
            void DestroyItem(SwCore::ISwProperty * property, QString customConstructedPropertyName = "");

		public:
			/** @brief : constructeur */
			SwPropertiesModelImpl(QObject * parent);
			/** @brief : destructeur */
			~SwPropertiesModelImpl();

			/** @brief : definit les propriétés via un ISwProperties */
            void SetProperties(SwCore::ISwProperties * properties,QString rootName=QString(),QString rootLabel=QString());

			/** @brief : definit les propriétés via une QHash<ISwProperty*, QString> , le QString étant l'arborescence de la property */
			void SetProperties(QList<StarlinxProperty> inProperties_list, QString rootName, QString rootLabel) ;

			/** @brief : callback avant changement */
            void OnBeforeChange(SwCore::ISwProperties * properties);
			/** @brief : callback apres changement */
            void OnAfterChange(SwCore::ISwProperties * properties);
			/** @brief : callback sur creation de propriétés */
            void OnCreateProperty(SwCore::ISwProperties * properties,SwCore::ISwProperty * property);
			/** @brief : ccallback sur destruction de propriétés*/
            void OnDestroyProperty(SwCore::ISwProperties * properties,SwCore::ISwProperty * property);

            //-------------------------------------------------------------
            // Interface QAbstractItemModel
            //------------------------------------------------------------
            /** @brief : Renvoie les capacites du modèle */
            virtual Qt::ItemFlags flags ( const QModelIndex & index ) const;
            /** @brief : Renvoie le nombre de colonnes pour les enfants d'un parent donné */
            virtual int columnCount ( const QModelIndex & parent = QModelIndex() ) const;			
            /** @brief : Renvoie le nombre de ligne pour un parent donné */
            virtual int rowCount ( const QModelIndex & parent = QModelIndex() ) const;
            /** @brief : Renvoie les données d'entete */
            virtual QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
            /** @brief : Renvoie les données stockées sous un certain role pour un item nommé index */
            virtual QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;
            /** @brief : Permet de definir la valeur d'un item*/
            virtual bool setData ( const QModelIndex & index, const QVariant & value, int role = Qt::EditRole );
            /** @brief : Renvoie l'item index specifie par la ligne et la colonne pour un parent donné*/
            virtual QModelIndex index ( int row, int column, const QModelIndex & parent = QModelIndex() ) const;
            /** @brief : Renvoie l'item parent d'un item index donné */
            virtual QModelIndex parent ( const QModelIndex & index ) const;

			//-------------------------------------------------------------
			// Interface ISwConfPropertiesObserver
			//------------------------------------------------------------
			virtual void onPropertyDeleted( ISwProperty * propertyDeleted, QString propertyDecoratedName, QString confName );

		};
	}
}
#endif 
