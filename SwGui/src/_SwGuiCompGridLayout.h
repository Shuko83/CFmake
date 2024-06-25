/**
@file _SwGuiCompGridLayout.h
@brief ComposantGridLayout
@author Big
 */

#ifndef __SWGUICOMPGRIDLAYOUT_H
#define __SWGUICOMPGRIDLAYOUT_H

#include "Component/Base/Component.h"

#include "_SwGuiCompGridLayoutCell.h"

using namespace StreamWork::SwCore;

/**
@class _SwGuiCompGridLayout
@brief ComposantGridLayout
*/
class _SwGuiCompGridLayout : public Component
{
    Q_OBJECT
    Q_PROPERTY(int Widgets_count READ getNbWidgets WRITE setNbWidgets)
    Q_PROPERTY(int horizontalSpacing READ getHorizontalSpacing WRITE setHorizontalSpacing)
    Q_PROPERTY(int verticalSpacing READ getVerticalSpacing WRITE setVerticalSpacing)
    Q_PROPERTY(QString columnStretch READ getColumnStretch WRITE setColumnStretch)
    Q_PROPERTY(QString rowStretch READ getRowStretch WRITE setRowStretch)
    Q_PROPERTY(int leftMargin READ getLeftMargin WRITE setLeftMargin)
    Q_PROPERTY(int topMargin READ getTopMargin WRITE setTopMargin)
    Q_PROPERTY(int rightMargin READ getRightMargin WRITE setRightMargin)
    Q_PROPERTY(int bottomMargin READ getBottomMargin WRITE setBottomMargin)

public:
    /** @brief Constructor */
    _SwGuiCompGridLayout();
    /** @brief Destructor */
    virtual ~_SwGuiCompGridLayout();
    /*! \brief Initialisation du composant */
    virtual void initializeComponent() throw(SwException);
    /*! \brief evenement avant changement de la disponibilité de l'interface
        \note A Surcharger*/
    virtual void eventBeforeInterfaceAvailability(QString interface_name,SwComponent_Class * provider_host);
    /*! \brief evenement apres changement de la disponibilité de l'interface
        \note A Surcharger*/
    virtual void eventAfterInterfaceAvailability(QString interface_name,SwComponent_Class * provider_host);
    //-------------------------------------------------------------------------
    //Getter setter property
    //-------------------------------------------------------------------------
    int getNbWidgets();
    void setNbWidgets(int nbWidgets);
    int getHorizontalSpacing();
    void setHorizontalSpacing(int horizontalSpacing);
    int getVerticalSpacing();
    void setVerticalSpacing(int verticalSpacing);
    QString getColumnStretch();
    void setColumnStretch(QString columnStretch);
    QString getRowStretch();
    void setRowStretch(QString rowStretch);
    int getLeftMargin();
    void setLeftMargin(int value);
    int getTopMargin();
    void setTopMargin(int value);
    int getRightMargin();
    void setRightMargin(int value);
    int getBottomMargin();
    void setBottomMargin(int value);
    //-------------------------------------------------------------------------
    //Fin Getter setter property
    //-------------------------------------------------------------------------


protected:
    void updateStretchToLayoutStretch();
    void updateLayoutStretchToStretch();
    void updateMargins();

private:
    //The Grid layout
	QWidget * _layoutWidget;
	QGridLayout * _layout;
    //Nb Widget
    int _widgets_count;
    //horizontalSpacing
    int _horizontalSpacing;
    //verticalSpacing
    int _verticalSpacing;
    //columnStretch
    QString _columnStretch;
    //rowStretch
    QString _rowStretch;
    //margins
    int _leftMargin;
    int _topMargin;
    int _rightMargin;
    int _bottomMargin;

    //Liste des cell widget et layout
    QList<_SwGuiCompGridLayoutCell *> _widgets;
};

#endif
