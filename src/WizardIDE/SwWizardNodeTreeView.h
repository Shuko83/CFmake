/*!
\file SwWizardNodeTreeView.h
\date 12/04/2006
\brief view pour le model de noeuds
\author  Big
\version 1.0
 */
#ifndef _SwWizardNodeTreeView_H
#define _SwWizardNodeTreeView_H
/*
  * INCLUDES GLOBAUX
  */

#include <QTreeView>
#include <QPoint>
#include <QModelIndex>
/*!
	\class SwWizardNodeTreeView 
	\brief  view pour le model de noeuds
*/

class SwWizardNodeTreeView : public QTreeView{
    Q_OBJECT
public:
    /*! \brief Constructeur */
	SwWizardNodeTreeView(QWidget * parent=0);
protected slots:
    /*! \brief Sur demande de menu contextuel */
    void OnContextMenuRequested( const QPoint & pos);
signals:
    /*! \brief Sur demande de menu contextuel */
    void OnMenuRequested(const QPoint & globalpos,QModelIndex index);

};

#endif
