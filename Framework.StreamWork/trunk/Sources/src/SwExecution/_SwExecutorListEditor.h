/*!
 \file _SwExecutorListEditor.h
 \brief Objet pour l'edition de la liste
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/

#ifndef __SwExecutorListEditor_H
#define __SwExecutorListEditor_H

/*
  * INCLUDES GLOBAUX
  */
#include <Qt>
#include <QWidget>
#include <QIcon>
#include <QFont>
#include <QColor>
#include <QList>

#include <SwComponent_Class.h>
#include "_SwExecution_Service.h"

using namespace StreamWork::SwCore;

/*!
	\class _SwExecutorListEditor
	\briefModele pour l'edition de la liste des composant executables
*/
class _SwExecutorListEditor : public QWidget {
	Q_OBJECT
protected:
	/*! \brief composant racine */
	SwComponent_Class * _root_component;
	/*! \brief liste des paths */
    QList<QString> * _path_list;
	/*! \brief liste des modes */
    QList<_SwExecutionMode> * _mode_list;
	/*! \brief montrer la position du drop */
    bool _show_pos_drop;
	/*! \brief index de position du drop */
    int _pos_drop_index;
	/*! \brief liste des selections */
    QList<bool> _sel_list;
    /*! \brief position on press */
    QPoint _click_pos;
	/*! \brief demarrage du drag a faire */
    bool _start_drag_to_do;
	/*! \brief arrow left */
    QIcon _arrowLeft;
	/*! \brief arrow right */
    QIcon _arrowRight;
	/*! \brief hover index */
    int _hoverIndex;


public:
    /*! \brief Constructeur */
	_SwExecutorListEditor(QWidget * parent,SwComponent_Class * root_component,QList<QString> * liste,QList<_SwExecutionMode> * liste_mode);
    /*! \brief Constructeur */
	virtual ~_SwExecutorListEditor();
    /*! \brief UpdateList */
	void UpdateList();
protected:
	/*! \brief dragEnterEvent */
    void dragEnterEvent(QDragEnterEvent *event);
	/*! \brief dragLeaveEvent */
    void dragLeaveEvent(QDragLeaveEvent *event);
	/*! \brief dragMoveEvent */
    void dragMoveEvent(QDragMoveEvent *event);
 	/*! \brief dropEvent */
    void dropEvent(QDropEvent *event);
 	/*! \brief mousePressEvent */
    void mousePressEvent(QMouseEvent *event);
 	/*! \brief mouseMoveEvent */
    void mouseMoveEvent ( QMouseEvent * event );
 	/*! \brief mouseReleaseEvent */
    void mouseReleaseEvent ( QMouseEvent * event );
    /*! \brief paintEvent */
    void paintEvent(QPaintEvent *event);
    /*! \brief keyPressEvent */
    void keyPressEvent ( QKeyEvent * event );

 	/*! \brief Index de l'item pointé*/
    int GetIndex(const QPoint & p);

signals:
    /*! \brief signal de reception d'une liste de path de composant ajouté */
    void OnExeAdded ( const QStringList & comps );
    /*! \brief signal de reception d'une liste de path de composant supprimé */
    void OnExeRemoved ( const QStringList & comps );

};

#endif
