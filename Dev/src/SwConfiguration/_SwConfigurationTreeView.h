/*!
\file _SwModelExportTreeview.h
\date 12/04/2006
\brief view pour le model exported entities
\author  Big
\version 1.0
 */
#ifndef ___SwConfigurationTreeView_H
#define ___SwConfigurationTreeView_H
/*
  * INCLUDES GLOBAUX
  */

#include <QTreeView>
/*!
	\class _SwConfigurationTreeView 
	\brief  view pour le model exported entities
*/

class _SwConfigurationTreeView : public QTreeView{
    Q_OBJECT
public:
    /*! \brief Constructeur */
	_SwConfigurationTreeView(QWidget * parent=0);
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
