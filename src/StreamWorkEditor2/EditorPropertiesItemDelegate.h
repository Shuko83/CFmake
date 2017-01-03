/**
@file EditorPropertiesItemDelegate.h
@brief
@author Big
*/

#ifndef _EDITORPROPERTIESITEMDELEGATE_H
#define _EDITORPROPERTIESITEMDELEGATE_H

#include "SwGuiDefaultItemDelegate.h"

/**
@class EditorPropertiesItemDelegate
@brief
*/
class EditorPropertiesItemDelegate : public StreamWork::SwGui::SwGuiDefaultItemDelegate
{
	Q_OBJECT;
public:
	/** @brief Constructor */
	EditorPropertiesItemDelegate(QObject *parent = 0);
	/** @brief Destructor */
	virtual ~EditorPropertiesItemDelegate();
	/** @brief paint */
	virtual void paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const;
protected:
	//Here protected methods

private:
	//Here private members

};

#endif
