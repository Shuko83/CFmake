/*!
 \file ComponentTreeModel.h
 \brief Modele pour la selection de composant
 \version 1.0
 \author EPO
 */

#ifndef _COMPONENT_TREEMODEL_H
#define _COMPONENT_TREEMODEL_H

/*
  * INCLUDES GLOBAUX
  */
#include <QList>
#include <QAbstractItemModel>
#include "Component/Base/SwComponent_Class.h"

/*
  * INCLUDES LOCAUX
  */

namespace StreamWork
{

	namespace SwGui
	{

		/*!
		\class ComponentTreeModel
		\brief Modele pour la selection des composants
		*/
		class ComponentTreeModel : public QAbstractItemModel
		{
			Q_OBJECT
		public:
			/*!
			\class _Item
			\brief Item pour la selection de composants
			*/
			class _Item
			{
			public:
				/*! \brief Handle sur le host */
				StreamWork::SwCore::SwComponent_Class * _host;
				/*! \brief Nom */
				QString _name;
				/*! \brief Enfants*/
				QList<_Item *> _childs;
				/*! \brief Parent*/
				_Item * _parent;
				/*! \brief Constructeur */
				_Item(_Item * parent, StreamWork::SwCore::SwComponent_Class * host, QString name);
				/*! \brief Destructeur */
				~_Item();
			};
		private:
			/*! \brief composant racine */
			StreamWork::SwCore::SwComponent_Class * _root_component;
			/*! \brief item racine */
			_Item * _root_item;

		public:
			/*! \brief Constructeur */
			ComponentTreeModel(QObject * parent, StreamWork::SwCore::SwComponent_Class * root_component);
			/*! \brief Constructeur */
			virtual ~ComponentTreeModel();
			/*! \brief Construction des items */
			virtual void buildItems(StreamWork::SwCore::SwComponent_Class * comp, _Item * item);

			//-------------------------------------------------------------
			// Interface QAbstractItemModel
			//------------------------------------------------------------
			/*! \brief Renvoie les capacites du mod le */
			virtual Qt::ItemFlags flags(const QModelIndex & index) const;
			/*! \brief list mimeTypes */
			QStringList mimeTypes() const;
			/*! \brief acces a la mimeData correspondantes */
			QMimeData * mimeData(const QModelIndexList &indexes) const;
			/*! \brief Renvoie le nombre de colonnes pour les enfants d'un parent donn  */
			virtual int columnCount(const QModelIndex & parent = QModelIndex()) const;
			/*! \brief Renvoie le nombre de ligne pour un parent donn  */
			virtual int rowCount(const QModelIndex & parent = QModelIndex()) const;
			/*! \brief Renvoie les donn es d'entete */
			virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
			/*! \brief Renvoie les donn es stock es sous un certain role pour un item nomm  index */
			virtual QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
			/*! \brief Renvoie l'item index specifie par la ligne et la colonne pour un parent donn */
			virtual QModelIndex index(int row, int column, const QModelIndex & parent = QModelIndex()) const;
			/*! \brief Renvoie l'item parent d'un item index donn  */
			virtual QModelIndex parent(const QModelIndex & index) const;

		};
	}
}

#endif 
