/**
@file QStreamTreeModel.h
@brief Modele arbre du stream pour les trees views
@author F.Bighelli
*/

#ifndef _QSTREAMTREEMODEL_H
#define _QSTREAMTREEMODEL_H

#include <Qt>
#include <QAbstractItemModel>
#include "StreamControler.h"
#include "ISelectionObserver.h"

/**
@class QStreamTreeModel
@brief Modele arbre du stream pour les trees views
*/
class QStreamTreeModel : public QAbstractItemModel, public ISelectionObserver
{
	Q_OBJECT
public:
	/** @brief Constructor */
	QStreamTreeModel(QObject * parent);
	/** @brief definition du controler */
	void setStreamControler(StreamControler * controler);
	//-------------------------------------------------------------
	// Interface QAbstractItemModel
	//------------------------------------------------------------
	/*! \brief Renvoie les capacites du modèle */
	virtual Qt::ItemFlags flags(const QModelIndex & index) const;
	/*! \brief Renvoie le nombre de colonnes pour les enfants d'un parent donné */
	virtual int columnCount(const QModelIndex & parent = QModelIndex()) const;
	/*! \brief Renvoie le nombre de ligne pour un parent donné */
	virtual int rowCount(const QModelIndex & parent = QModelIndex()) const;
	/*! \brief Renvoie les données d'entete */
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	/*! \brief Renvoie les données stockées sous un certain role pour un item nommé index */
	virtual QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
	/*! \brief Renvoie l'item index specifie par la ligne et la colonne pour un parent donné*/
	virtual QModelIndex index(int row, int column, const QModelIndex & parent = QModelIndex()) const;
	/*! \brief Renvoie l'item parent d'un item index donné */
	virtual QModelIndex parent(const QModelIndex & index) const;
	//-------------------------------------------------------------
	// Interface evenement du stream
	//------------------------------------------------------------
	/** @brief evenement du stream : ajout d'un enfant */
	void childAdded(StreamWork::SwCore::SwComponent_Class * parent,
					StreamWork::SwCore::SwComponent_Class *child);
	/** @brief evenement du stream : suppression d'un enfant */
	void childRemoved(StreamWork::SwCore::SwComponent_Class * parent,
					  StreamWork::SwCore::SwComponent_Class *child);
	/** @brief evenement du stream : renommage d'un composant */
	void componentNameChanged(StreamWork::SwCore::SwComponent_Class * component);
	/*! \brief Sur ajout d'une nouvelle interface */
	//--------------------------------------------------------------------------
	// Liaison modele <-> QStreamTreeModel
	//--------------------------------------------------------------------------
	/** @brief connexion des evenements d'un composant au controleur */
	void connectToControler(StreamWork::SwCore::SwComponent_Class * component);
	/** @brief connexion des evenements d'un composant au controleur */
	void recursiveConnectToControler(StreamWork::SwCore::SwComponent_Class * component);
	/** @brief deconnexion des evenements de tous les composant d'un stream au controleur */
	void disconnectFromControler(StreamWork::SwCore::SwComponent_Class * component);
	/** @brief deconnexion des evenements d'un composant au controleur */
	void recursiveDisconnectToControler(StreamWork::SwCore::SwComponent_Class * component);
	//--------------------------------------------------------------------------
	// ISelectionObserver
	//--------------------------------------------------------------------------
	virtual void setSelection(QList<StreamWork::SwCore::SwComponent_Class *> & sel);

public slots:
	//--------------------------------------------------------------------------
	// events gui
	//--------------------------------------------------------------------------
	/** @brief sur selection d'un item */
	void onSelectItem(const QModelIndex & index);
private:
	/** @brief controler */
	StreamControler * _controler;
	/** @brief composant racine */
	StreamWork::SwCore::SwComponent_Class * _rootComponent;
	/** @brief selection */
	QList<StreamWork::SwCore::SwComponent_Class *> _selection;
};

#endif
