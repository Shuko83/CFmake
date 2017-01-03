/**
@file WidgetFactory.h
@brief Fabrique de widget
@author F.Bighelli
 */

#ifndef _WIDGETFACTORY_H
#define _WIDGETFACTORY_H

#if QT_VERSION >= 0x050000
#include <QtWidgets>
#else
#include <QtGui>
#endif
#include "PropertiesWidget.h"
#include "QStreamTreeModel.h"

/**
@class WidgetFactory
@brief Fabrique de widget
*/
class WidgetFactory {

public:
    /** @brief acces singleton */
    static WidgetFactory * getInstance();

public:
    /** @brief Construction widget representant les composants disponibles*/
    QWidget * buildPluginsBankView(bool isGraphViewHosted);
    /** @brief Construction widget vue en arbre*/
    QWidget * buildStreamTreeView(QStreamTreeModel * model);
    /** @brief Construction widget propriétés*/
    PropertiesWidget * buildPropertiesWidget();
	/** @brief Construction widget Color legend*/
	QWidget * buildLegendView();

private:
	/** @brief Constructor */
	WidgetFactory();
	/** @brief ~Destructor */
	~WidgetFactory();
	/** @brief Modification attribut lorsque le widget est hébergé par un GraphView */
    void setGraphViewMode(QWidget * w);
private:
    /** @brief Palette d'affichage pour la vue graph */
    QPalette _graphPalette;

};

#endif
