/**
@file QInterestAreaTreeModel.h
@brief QModel pour tree view relative au interest area
@author F.Bighelli
 */

#ifndef _QINTERESTAREATREEMODEL_H
#define _QINTERESTAREATREEMODEL_H

#include <Qt>
#include <QAbstractItemModel>
#include "StreamControler.h"
#include "IStreamControlerObserver.h"
#include "InterestArea.h"
#include "ComponentGraphicItem.h"

/**
@class QInterestAreaTreeModel
@brief QModel pour tree view relative au interest area
*/
class QInterestAreaTreeModel : public QAbstractItemModel, public IStreamControlerObserver {
    Q_OBJECT
public:
    class Item {
    public:
        Item(InterestArea * ia,Item * parent=0);
        Item(ComponentGraphicItem * cg,Item * parent=0);
        ~Item();
        bool isIA();
        Item * getParent();
        QList<Item *> & getChilds();
        QString getText() const;
        QGraphicsItem * getGraphicsItem();
        QIcon getIcon();
    private:
        Item * _parent;
        QList<Item *> _childs;
        InterestArea * _ia;
        ComponentGraphicItem * _cg;
        QIcon _ico;
    };

	/** @brief Constructor */
	QInterestAreaTreeModel(QObject * parent);
    ~QInterestAreaTreeModel();
    /** @brief definition du controler */
    void setStreamControler(StreamControler * controler);
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
    /*! \brief Renvoie l'item index specifie par la ligne et la colonne pour un parent donné*/
    virtual QModelIndex index ( int row, int column, const QModelIndex & parent = QModelIndex() ) const;
    /*! \brief Renvoie l'item parent d'un item index donné */
    virtual QModelIndex parent ( const QModelIndex & index ) const;
    //-------------------------------------------------------------
    // Interface IStreamControlerObserver
    //------------------------------------------------------------
    void controlerHasChanged() override;
public slots:
    //--------------------------------------------------------------------------
    // events gui
    //--------------------------------------------------------------------------
    /** @brief sur selection d'un item */ 
    void onSelectItem(const QModelIndex & index);
private:
    /** @brief controler */
    StreamControler * _controler;
    /** @brief Liste des items */
    QList<Item *> _items;
};

#endif
