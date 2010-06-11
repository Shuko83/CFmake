/**
@file PropertiesWidget.h
@brief Widget d'edition des propriétés
@author F.Bighelli
 */

#ifndef _PROPERTIESWIDGET_H
#define _PROPERTIESWIDGET_H

#include <QtCore>
#include <QtGui>
#include "ComponentGraphicItem.h"
#include <SwPropertiesModelImpl.h>
#include <SwGuiDefaultItemDelegate.h>

/**
@class PropertiesWidget
@brief Widget d'edition des propriétés
*/
class PropertiesWidget : public QWidget{
    Q_OBJECT
public:
	/** @brief Constructor */
	PropertiesWidget();
    /** @brief Composant en cours d'edition*/
    void setSelectedGraphicComponent(ComponentGraphicItem * cgi);
    /** @brief montre le widget */
    void showProperties(QWidget * srcWidget);

public slots:
    /** @brief sur changement du nom */
    void nameChanged ( const QString & text );
    /** @brief sur fin changement du nom */
    void nameChangedAndValid ();
private:
    /** @brief Composant en cours d'edition*/
    ComponentGraphicItem * _cgi;
    /** @brief properties model **/
    StreamWork::SwCore::SwPropertiesModelImpl * _pModel;
    /** @brief properties view */
    QTreeView * _pView;
    /** @brief name editor */
    QLineEdit * _nameEdit;
};

#endif
