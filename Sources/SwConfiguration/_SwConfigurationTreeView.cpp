/*!
\file _SwConfigurationTreeView.cpp
\date 12/04/2006
\brief view pour le model exported entities
\author  Big
\version 1.0
 */
#include <QDropEvent>
#include <QMimeData>
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDragMoveEvent>
#include "_SwConfigurationTreeView.h"
#include "_SwConfigurationEntitiesModel.h"

/*! \brief Constructeur */
_SwConfigurationTreeView::_SwConfigurationTreeView(QWidget * parent):QTreeView(parent){

}
/*! \brief Drag enter event */
void _SwConfigurationTreeView::dragEnterEvent ( QDragEnterEvent * event ) {
    if (event->mimeData()->hasFormat("application/model_entity")) {
        event->accept();
    } else
        event->ignore();

}
/*! \brief Drag leave event */
void _SwConfigurationTreeView::dragLeaveEvent ( QDragLeaveEvent * event ){
    event->accept();
}
/*! \brief Drag move event */
void _SwConfigurationTreeView::dragMoveEvent ( QDragMoveEvent * event ){
    if (event->mimeData()->hasFormat("application/model_entity")) {
        event->accept();
    } else
        event->ignore();
}
/*! \brief Sur key press */
void _SwConfigurationTreeView::keyPressEvent ( QKeyEvent * event ) {
    if (event->matches(QKeySequence::Delete)) {
        _SwConfigurationEntitiesModel * model=dynamic_cast<_SwConfigurationEntitiesModel *>(this->model());
        if (model!=NULL) {
            QModelIndexList list_to_remove;
            list_to_remove.push_back(currentIndex());
            model->RemoveSelectedItems(list_to_remove);
            event->accept();
            return;
        }
    }
    QTreeView::keyPressEvent(event);
}
