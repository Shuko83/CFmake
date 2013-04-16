/*!
 \file __SwQAbstractItemViewSlots_Catcher.h
 \brief interface definissant des slots connectable au signaux de la vue QAbstractModelView
 \date 02-octobre-2006 16:04:34
 \author F.Bighelli
*/

/*
  * INCLUDES LOCAUX
  */
#include "_SwQAbstractItemViewSlots_Catcher.h"
#include <QTreeView>


using namespace StreamWork::SwGui;

/*! \brief constructeur */
_SwQAbstractItemViewSlots_Catcher::_SwQAbstractItemViewSlots_Catcher(QAbstractItemView * host):QObject() {
	_handle_interface=NULL;
    _host=host;
}
/*! \brief definition de l'interface  */
void _SwQAbstractItemViewSlots_Catcher::DefineInterfaceSlots(ISwQAbstractItemViewSlots * handle_inteface) {
	_handle_interface=handle_inteface;
}
/*! \brief Filtre d'evenement (envoie les etats sourie a l'interface ISwQAbstractItemViewSlots); */
bool _SwQAbstractItemViewSlots_Catcher::eventFilter(QObject *obj, QEvent *event) {
    
    if (_handle_interface==NULL)
        return false;
    QEvent::Type t=event->type();
    if ((event->type() == QEvent::MouseButtonPress) ||
        (event->type() == QEvent::MouseButtonRelease)||
        (event->type() == QEvent::MouseMove)||
        (event->type() == QEvent::MouseButtonDblClick)) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        if (_handle_interface!=NULL) {
            _handle_interface->onMouseEvent(mouseEvent);
        }
    }
    if (event->type()==QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->matches(QKeySequence::HelpContents)) {
            _handle_interface->onHelp();
            keyEvent->accept();
            return true;
        }
    }
    return false;
}
/*! \brief sur activation */
void _SwQAbstractItemViewSlots_Catcher::onActivated ( const QModelIndex & index ) {
	if (_handle_interface!=NULL) _handle_interface->onActivated(index);
}
/*! \brief sur click */
void _SwQAbstractItemViewSlots_Catcher::onClicked ( const QModelIndex & index ) {
	if (_handle_interface!=NULL) _handle_interface->onClicked(index);
}
/*! \brief sur double click */
void _SwQAbstractItemViewSlots_Catcher::onDoubleClicked ( const QModelIndex & index ) {
	if (_handle_interface!=NULL) _handle_interface->onDoubleClicked(index);
}
/*! \brief sur entree */
void _SwQAbstractItemViewSlots_Catcher::onEntered ( const QModelIndex & index ) {
	if (_handle_interface!=NULL) _handle_interface->onEntered(index);
}
/*! \brief sur  appuie de la sourie sur l'item */
void _SwQAbstractItemViewSlots_Catcher::onPressed ( const QModelIndex & index ) {
	if (_handle_interface!=NULL) _handle_interface->onPressed(index);
}
/*! \brief sur entre dans la vue*/
void _SwQAbstractItemViewSlots_Catcher::onViewportEntered () {
	if (_handle_interface!=NULL) _handle_interface->onViewportEntered();
}
/*! \brief OnMenuRequest*/
void _SwQAbstractItemViewSlots_Catcher::OnMenuRequested( const QPoint & pos) {
    QModelIndex index=_host->indexAt(pos);
    if (!index.isValid()) {
        return;
    }
    QPoint globalpos=_host->viewport()->mapToGlobal(pos);
    if (_handle_interface!=NULL) _handle_interface->onMenuRequested(globalpos,index); 
}

//-------------------------------------------------------------------------
void _SwQAbstractItemViewSlots_Catcher::resizeColumn( const QModelIndex& )
{
	QTreeView * tmpItem = reinterpret_cast<QTreeView*>(_host);
	if(tmpItem)
	{
		tmpItem->resizeColumnToContents(0);
		int value = tmpItem->columnWidth(0);
		tmpItem->setColumnWidth(0, value+10);
	}
	
}
