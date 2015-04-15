/*!
\file _SwModelExportTreeview.h
\date 12/04/2006
\brief view pour le model exported entities
\author  Big
\version 1.0
 */
#ifndef __SwModelExportTreeview_H
#define __SwModelExportTreeview_H
/*
  * INCLUDES GLOBAUX
  */

#include <QTreeView>
/*!
	\class _SwModelExportTreeview 
	\brief  view pour le model exported entities
*/

class _SwModelExportTreeview : public QTreeView{
    Q_OBJECT
public:
    /*! \brief Constructeur */
	_SwModelExportTreeview(QWidget * parent=0);
protected:
    /*! \brief Drag enter event */
    void dragEnterEvent ( QDragEnterEvent * event );
    /*! \brief Drag leave event */
    void dragLeaveEvent ( QDragLeaveEvent * event );
    /*! \brief Drag move event */
    void dragMoveEvent ( QDragMoveEvent * event );
    /*! \brief Sur key press */
    void keyPressEvent ( QKeyEvent * event ) ;
};

#endif
