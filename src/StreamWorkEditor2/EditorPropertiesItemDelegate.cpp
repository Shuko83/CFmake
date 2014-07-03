/**
@file EditorPropertiesItemDelegate.cpp
@brief 
@author Big
 */

#include "EditorPropertiesItemDelegate.h"
#include <SwPropertiesModelImpl.h>
#include <ISwProperty.h>
#include <ISwProperty2.h>

using namespace StreamWork::SwCore;
using namespace StreamWork::SwGui;

/** @brief Constructor */
EditorPropertiesItemDelegate::EditorPropertiesItemDelegate(QObject *parent):SwGuiDefaultItemDelegate(parent) {
	//TO DO creation
}
/** @brief Destructor */
EditorPropertiesItemDelegate::~EditorPropertiesItemDelegate() {
  //TO DO destruction
}
/** @brief paint */
void EditorPropertiesItemDelegate::paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const {
    SwGuiDefaultItemDelegate::paint(painter,option,index);

    if (!index.isValid() || index.column()!=0) 
        return;
    QVariant value = index.model()->data(index, Qt::UserRole);
    void * ptr=value.value<void*>();
    if(ptr==0) {
        return;
    }
    ISwProperty * p=(ISwProperty *)ptr;
    ISwProperty2 * rp=dynamic_cast<ISwProperty2 *>(p);
    if (rp==0 || !rp->HasChanged() || !rp->isResettable()) {
        return;
    }
    if ((option.state & QStyle::State_MouseOver)==QStyle::State_MouseOver) {
        QIcon ico(":/StreamWorkEditor2/reset.png");
        ico.paint(painter,QRect(option.rect.right()-17,option.rect.top(),17,17));
    }

}
