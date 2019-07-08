/**
@file WidgetFactory.cpp
@brief Fabrique de widget
@author F.Bighelli
 */

#include "WidgetFactory.h"

#include "AnchorableContainer.h"
#include <SwPropertiesModelImpl.h>
#include <SwGuiDefaultItemDelegate.h>
#include "PluginOverview.h"
#include "LegendView.h"

/** @brief Constructor */
WidgetFactory::WidgetFactory() {
	_graphPalette.setColor(QPalette::Window,QColor(50,50,50,255));
	_graphPalette.setColor(QPalette::Base,QColor(50,50,50,255));
	_graphPalette.setColor(QPalette::AlternateBase,QColor(10,10,10,255));
	_graphPalette.setColor(QPalette::Button,QColor(128,128,128,255));
	_graphPalette.setColor(QPalette::WindowText,QColor(255,255,255,255));
	_graphPalette.setColor(QPalette::Text,QColor(255,255,255,255));
	_graphPalette.setColor(QPalette::BrightText,QColor(128,128,128,255));
	_graphPalette.setColor(QPalette::ButtonText,QColor(255,255,255,255));

}
/** @brief ~Destructor */
WidgetFactory::~WidgetFactory() {

}
/** @brief acces singleton */
WidgetFactory * WidgetFactory::getInstance() {
    static WidgetFactory instance;
    return &instance;
}

/** @brief Construction widget representant les composants disponibles*/
QWidget * WidgetFactory::buildPluginsBankView(bool isGraphViewHosted) {
    
	PluginOverview * w = new PluginOverview(isGraphViewHosted,_graphPalette);
    return w;
}
/** @brief Construction widget vue en arbre*/
QWidget * WidgetFactory::buildStreamTreeView(QStreamTreeModel * model) {
    QTreeView * w=new QTreeView();
    w->setAlternatingRowColors(true);
    w->setModel(model);
    //setGraphViewMode(w);
    return w;
}
/** @brief Construction widget propriétés*/
PropertiesWidget * WidgetFactory::buildPropertiesWidget() {
    PropertiesWidget * w=new PropertiesWidget();
    return w;
}

/** @brief Modification attribut lorsque le widget est hébergé par un GraphView */
void WidgetFactory::setGraphViewMode(QWidget * w) {
    w->setPalette(_graphPalette);
    //w->setAutoFillBackground(true);
}

//-------------------------------------------------------------------------
QWidget * WidgetFactory::buildLegendView()
{
	LegendView *w = new LegendView;
	return w;
}


