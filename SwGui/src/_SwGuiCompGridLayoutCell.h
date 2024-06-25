/**
@file _SwGuiCompGridLayoutCell.h
@brief ComposantGridLayoutCell
@author Big
 */

#ifndef __SWGUICOMPGRIDLAYOUTCELL_H
#define __SWGUICOMPGRIDLAYOUTCELL_H

#include <QtCore>
#include <QGridLayout>
#include <QWidget>
#include "Component/Base/Component.h"

using namespace StreamWork::SwCore;

/**
@class _SwGuiCompGridLayoutCell
@brief ComposantGridLayoutCell
*/
class _SwGuiCompGridLayoutCell : public QObject
{
    Q_OBJECT
    Q_PROPERTY(unsigned int row READ getRow WRITE setRow)
    Q_PROPERTY(unsigned int column READ getColumn WRITE setColumn)
    Q_PROPERTY(unsigned int rowSpan READ getRowSpan WRITE setRowSpan)
    Q_PROPERTY(unsigned int columnSpan READ getColumnSpan WRITE setColumnSpan)
    Q_PROPERTY(Qt::Alignment alignment READ getAlignment WRITE setAlignment);
public:
    /** @brief Constructor */
    _SwGuiCompGridLayoutCell(int index,ISwProperties * propertiesAccess,ISwInterfaces_Consumer *consumerAccess);
    /** @brief Destructor */
    virtual ~_SwGuiCompGridLayoutCell();
    //Definition du grid layout
    void setGridLayout(QGridLayout * glayout);
    //-------------------------------------------------------------------------
    //Getter setter property
    //-------------------------------------------------------------------------
    unsigned int getRow();
    void setRow(unsigned int row);
    unsigned int getColumn();
    void setColumn(unsigned int column);
    unsigned int getRowSpan();
    void setRowSpan(unsigned int rowSpan);
    unsigned int getColumnSpan();
    void setColumnSpan(unsigned int columnSpan);
    Qt::Alignment getAlignment();
    void setAlignment(Qt::Alignment alignment);
    //-------------------------------------------------------------------------
    //Fin Getter setter property
    //-------------------------------------------------------------------------
    /*! \brief evenement avant changement de la disponibilité de l'interface
        \note A Surcharger*/
    virtual void eventBeforeInterfaceAvailability(QString interface_name,SwComponent_Class * provider_host);
    /*! \brief evenement apres changement de la disponibilité de l'interface
        \note A Surcharger*/
    virtual void eventAfterInterfaceAvailability(QString interface_name,SwComponent_Class * provider_host);
    //Reconstruction
    void rebuild();
    //Construction
    void build();
    //Destruction
    void destroy();
protected:
    //Here protected methods
    
private:
    QGridLayout * _glayout;
    unsigned int _row;
    unsigned int _column;
    unsigned int _rowSpan;
    unsigned int _columnSpan;
    Qt::Alignment _alignment;
    ISwProperties * _propertiesAccess;
    ISwInterfaces_Consumer * _consumerAccess;
    bool _isBuild;
    QWidget * _hWidget;
};

#endif
