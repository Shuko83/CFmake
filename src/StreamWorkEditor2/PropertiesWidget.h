/**
@file PropertiesWidget.h
@brief Widget d'edition des propriétés
@author F.Bighelli
*/

#ifndef _PROPERTIESWIDGET_H
#define _PROPERTIESWIDGET_H

#include <QtCore>
#if QT_VERSION >= 0x050000
#include <QtWidgets>
#else
#include <QtGui>
#endif
#include "ComponentGraphicItem.h"
#include <SwPropertiesModelImpl.h>
#include <EditorPropertiesItemDelegate.h>

/**
@class PropertiesWidget
@brief Widget d'edition des propriétés
*/
class PropertiesWidget : public QWidget
{
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
	void nameChanged(const QString & text);
	/** @brief sur fin changement du nom */
	void nameChangedAndValid();
	/** @brief on color click */
	void onColorClick();
	/** @brief on text color click */
	void onTextColorClick();
	/** @brief on text color click */
	void onReset(const QModelIndex & index);
private:
	/** @brief change la couleur du bouton */
	void setTextColorToButton(const QColor & color);
	/** @brief change la couleur du bouton */
	void setBgColorToButton(const QColor & color);
private:
	/** @brief Composant en cours d'edition*/
	ComponentGraphicItem * _cgi;
	/** @brief properties model **/
	StreamWork::SwCore::SwPropertiesModelImpl * _pModel;
	/** @brief properties view */
	QTreeView * _pView;
	/** @brief name editor */
	QLineEdit * _nameEdit;
	/** @brief text pixmap color*/
	QPixmap _pixmapTextColor;
	/** @brief backgroung pixmap color */
	QPixmap _pixmapBgColor;
	/** @brief button for text pixmap color*/
	QPushButton * _buttonTextColor;
	/** @brief button for backgroung pixmap color */
	QPushButton * _buttonBgColor;
};

#endif
