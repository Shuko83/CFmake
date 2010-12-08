/**
@file _SwGuiCompGridLayout.h
@brief ComposantGridLayout
@author Big
 */

#ifndef __SWGUICOMPGRIDLAYOUT_H
#define __SWGUICOMPGRIDLAYOUT_H

#include <Component.h>

#include "ISwLayout.h"
#include "_SwGuiCompGridLayoutCell.h"

using namespace StreamWork::SwCore;
using namespace StreamWork::SwGui;
/**
@class _SwGuiCompGridLayout
@brief ComposantGridLayout
*/
class _SwGuiCompGridLayout : public Component , public ISwLayout  {
    Q_OBJECT
    Q_PROPERTY(unsigned int Widgets_count READ getNbWidgets WRITE setNbWidgets)
    Q_PROPERTY(unsigned int Layouts_count READ getNbLayouts WRITE setNbLayouts)
    Q_PROPERTY(int horizontalSpacing READ getHorizontalSpacing WRITE setHorizontalSpacing)
    Q_PROPERTY(int verticalSpacing READ getVerticalSpacing WRITE setVerticalSpacing)
public:
    /** @brief Constructor */
    _SwGuiCompGridLayout();
    /** @brief Destructor */
    virtual ~_SwGuiCompGridLayout();
    /*! \brief Renvoie le Layout
    \return le Layout */
	virtual QLayout & GetLayout();
    /*! \brief Liberation du layout (doit etre appele lors de la liberation du layout)
    Attention, le layout est (et doit) etre detruit par cette methode*/
	virtual void LiberateLayout();
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
    unsigned int getNbWidgets();
    void setNbWidgets(unsigned int nbWidgets);
    unsigned int getNbLayouts();
    void setNbLayouts(unsigned int nbLayouts);
    int getHorizontalSpacing();
    void setHorizontalSpacing(unsigned int horizontalSpacing);
    int getVerticalSpacing();
    void setVerticalSpacing(unsigned int verticalSpacing);
    //-------------------------------------------------------------------------
    //Fin Getter setter property
    //-------------------------------------------------------------------------


protected:
    //Here protected methods
    
private:
    //The Grid layout
    QGridLayout * _layout;
    //Nb Widget
    unsigned int _widgets_count;
    //Nb Layout
    unsigned int _layouts_count;
    //horizontalSpacing
    int _horizontalSpacing;
    //verticalSpacing
    int _verticalSpacing;
    //Liste des cell widget et layout
    QList<_SwGuiCompGridLayoutCell *> _widgets;
    QList<_SwGuiCompGridLayoutCell *> _layouts;
};

#endif
