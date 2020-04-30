/*!
\file _SwModelExportTreeview.cpp
\date 12/04/2006
\brief view pour le model exported entities
\author  Big
\version 1.0
 */
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDragMoveEvent>
#include "_SwModelExportTreeView.h"
#include "_SwModelExportedEntitiesModel.h"

/*! \brief Constructeur */
_SwModelExportTreeview::_SwModelExportTreeview(QWidget * parent):QTreeView(parent){

}
/*! \brief Drag enter event */
void _SwModelExportTreeview::dragEnterEvent ( QDragEnterEvent * event ) {
    if (event->mimeData()->hasFormat("application/model_entity")) {
        event->accept();
    } else
        event->ignore();

}
/*! \brief Drag leave event */
void _SwModelExportTreeview::dragLeaveEvent ( QDragLeaveEvent * event ){
    event->accept();
}
/*! \brief Drag move event */
void _SwModelExportTreeview::dragMoveEvent ( QDragMoveEvent * event ){
    if (event->mimeData()->hasFormat("application/model_entity")) {
        event->accept();
    } else
        event->ignore();
}
/*! \brief Sur key press */
void _SwModelExportTreeview::keyPressEvent ( QKeyEvent * event ) {
    if (event->matches(QKeySequence::Delete)) {
        _SwModelExportedEntitiesModel * model=dynamic_cast<_SwModelExportedEntitiesModel *>(this->model());
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
