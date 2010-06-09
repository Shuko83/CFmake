/*!
 \file NodePropertiesModelImpl.h
 \brief Definit un modele a partir d'une liste de propriétés d'un composant noeud
 \date 02-octobre-2006 16:04:34
 \author F.Bighelli
*/

#ifndef _NodePropertiesModelImpl_H
#define _NodePropertiesModelImpl_H

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

using namespace StreamWork::SwCore;
/*!
 \class NodePropertiesModelImpl 
 \brief Definit un modele a partir d'une liste de propriétés d'un composant noeud
*/
class NodePropertiesModelImpl : public QAbstractItemModel { 
	Q_OBJECT
protected:
	/*! \brief Item representant une propriété */
	class PropertyItem {
	public:
		/*! \brief model hote */
		NodePropertiesModelImpl * _host;
		/*! \brief parent */
		PropertyItem * _parent;
		/*! \brief enfants */
		QMap<QString,PropertyItem *> _childrens;
		/*! \brief interface propriété*/
		ISwProperty * _property;
		/*! \brief label si la propriété n'est pas définit*/
		QString _label;
		/*! \brief constructeur par interface*/
		PropertyItem(NodePropertiesModelImpl * host,QString label,ISwProperty * property);
		/*! \brief constructeur par nom*/
		PropertyItem(NodePropertiesModelImpl * host,QString label);
		/*! \brief Destructeur */
		virtual ~PropertyItem();
		/*! \brief Sur changement de la propriété */
		void OnPropertyChange(ISwProperty * property);
	};
protected:
	/* Handle Propriétés */
	ISwProperties * _properties;
	/* Root Item */
	PropertyItem * _root_item;
protected:
	/*! \brief creation d'un item */
	void CreateItem(ISwProperty * property);
	/*! \brief destruction d'un item */
	void DestroyItem(ISwProperty * property);
public:
	/*! \brief constructeur */
	NodePropertiesModelImpl(QObject * parent);
	/*! \brief destructeur */
	NodePropertiesModelImpl();
	/*! \brief definit les propriétés */
	void SetProperties(ISwProperties * properties,QString root_name);
	/*! \brief callback avant changement */
	void OnBeforeChange(ISwProperties * properties);
	/*! \brief callback apres changement */
	void OnAfterChange(ISwProperties * properties);
	/*! \brief callback sur creation de propriétés */
	void OnCreateProperty(ISwProperties * properties,ISwProperty * property);
	/*! \brief ccallback sur destruction de propriétés*/
	void OnDestroyProperty(ISwProperties * properties,ISwProperty * property);
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
		};
#endif 
