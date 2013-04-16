/*!
\file SwWizardFrontEnd.h
\date 12/04/2006
\brief Ihm du wizard de generation de plugin
\author  Big
\version 1.0
 */

#ifndef _SwWizardFrontEnd_H
#define _SwWizardFrontEnd_H
/*
  * INCLUDES GLOBAUX
  */

#include <QDialog>
#include <QAction>
#include <QList>
#include <QTreeView>
#include <QTextEdit>
#include "SwRefPtr.h"
#include "SwSimpleComponent_Class.h"
#include "SwWizardNodeModel.h"
/*
  * INCLUDES LOCAUX
  */

#include <ISwAction.h>
#include "SwWizardConstantes.h"


using namespace StreamWork::SwCore;
using namespace StreamWork::SwGui;

/*!
	\class SwWizardFrontEnd 
	\brief Ihm du wizard de generation de plugin
*/
class BUILD_SWWIZARD SwWizardFrontEnd : public QDialog , public ISwAction
{
    Q_OBJECT
protected:
    /* action pour l'appel de cette fenetre de dialogue */
    QAction * _call_action;
    /* Root component */
    SwRefPtr<SwSimpleComponent_Class> _root;
    /* model pour l'edition */
    SwWizardNodeModel * _model;
    /* zone de texte */
    QTextEdit *_tedit;
public:
    /*! \brief Constructeur */
    SwWizardFrontEnd(QWidget *parent);
    /*! \brief Destructeur */
    ~SwWizardFrontEnd();
    
	//--------------------------------------------------------------
	// Interface ISwAction
	//--------------------------------------------------------------
    /*! \brief Renvoie le Action
    \return le Action */
	virtual QAction & GetAction();
private slots:
    /*! \brief slot pour la visualisation de ce wizard */
    void ShowWizard();
    /*! \brief slot pour la generation de ce wizard */
    void Generate();
};

#endif
