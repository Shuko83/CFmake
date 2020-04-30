/**
 * @file SwGuiQActionToWidget2.h
 * @brief Composant qui consomme un ISwAction et une QWidget et qui, sur le triggered de l'action
 *		  ouvre ou ferme le widget associé.
 *		  Le Widget n'est pas encapsulé
 * @version 1.0
 * @date  21/03/2014
 * @author  CGD
 */

#ifndef _SwGuiQActionConsumedToWidget_H
#define _SwGuiQActionConsumedToWidget_H

#include <Component.h>
#include <ISwAction.h>

using namespace StreamWork::SwCore;
using namespace StreamWork::SwGui;

class _SwGuiQActionConsumedToWidget : public Component 
{
	Q_OBJECT


public slots:
		void ManageAction();
		

protected:

	/** @brief Interface QAction*/
	ISwAction * _i_Action;


public:
    /** @brief Constructeur */
    _SwGuiQActionConsumedToWidget();
    /** @brief Destructeur */
    virtual ~_SwGuiQActionConsumedToWidget();
    /*! \brief Initialisation du composant*/
    virtual void initializeComponent() throw(SwException);
    /*! \brief evenement de changement de propriete*/
    virtual void eventPropertyChange(ISwProperty * property);
    /*! \brief evenement avant changement de la disponibilité de l'interface*/
    virtual void eventBeforeInterfaceAvailability(QString interface_name,SwComponent_Class * provider_host);
    /*! \brief evenement apres changement de la disponibilité de l'interface*/
    virtual void eventAfterInterfaceAvailability(QString interface_name,SwComponent_Class * provider_host);



private:
	QWidget * _widget;
	QAction *_action;

	QWidget* topParent;

	bool _isVisible;

	bool isMoved;
	void MoveCenter();//déplace la widget au centre de l'écran

};

#endif
