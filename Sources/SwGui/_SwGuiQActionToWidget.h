/**
 * @file SwGuiQActionToWidget.h
 * @brief NoDescription
 * @version 1.0
 * @date Mon Mar 14 16:59:33 CET 2011
 * @author AuthorOfPlugin
 */

#ifndef _SwGuiQActionToWidget_H
#define _SwGuiQActionToWidget_H
/*
  * INCLUDES LOCAUX
  */
#include <SwEnum.h>
#include <Component.h>
#include <SwPin.h>
#include <ISwProperty.h>
#include <QAction>

#include "_SwContainerCloseableWidget.h"
#include "ISwQMainWindow.h"

//Check namespace needed 
using namespace StreamWork::SwCore;
using namespace StreamWork::SwGui;

/**
 *	@class SwGuiQActionToWidget
 *	@brief NoDescription
 */ 
class _SwGuiQActionToWidget : public Component
{
	Q_OBJECT
public:
    /**
    * @brief Type d'orientation pour les pistes dans la carto :
    * - NUP : North up : la symbologie restera orientée plein nord
    * - TUP : Track up : la symbologie suivra l'orientation de la piste
    */ 
    Q_ENUMS (WindowFlag);
    typedef enum
    {
        WIDGET              = Qt::Widget,
        WINDOW              = Qt::Window,
        DIALOG              = Qt::Dialog,
        SHEET               = Qt::Sheet,
        DRAWER              = Qt::Drawer,
        POPUP               = Qt::Popup,
        TOOL                = Qt::Tool,
        TOOL_TIP            = Qt::ToolTip,
        SPLASH_SCREEN       = Qt::SplashScreen,
        FRAMELESSWINDOWHINT = Qt::FramelessWindowHint
    } WindowFlag;

	// Properties accessors
	QString getShowName();
	void setShowName(QString name);
	QString getHiddenName();
    void setHiddenName (QString name);	
	WindowFlag getFlag();
    void setFlag (WindowFlag flag);	
	bool getStayOnTop() const;
	void setStayOnTop(bool val);
	
public slots:
		void ManageAction();
		void ManageWidget();
		
protected:
    /** @brief Property value identUndefinedColor */
	Q_PROPERTY (QString show_name
        READ getShowName
        WRITE setShowName);

	Q_PROPERTY (QString hidden_name
        READ getHiddenName
        WRITE setHiddenName);
    
	Q_PROPERTY (WindowFlag flag
        READ getFlag
        WRITE setFlag);

	Q_PROPERTY(bool stayOnTop						
		READ getStayOnTop 
		WRITE setStayOnTop);
    
protected:
 	//--------------------------------------------------------------
	//Properties
	//--------------------------------------------------------------

 	//--------------------------------------------------------------
	//Handle interfaces
	//--------------------------------------------------------------
    /** @brief Interface Widget*/
	ISwQMainWindow * _iSwQMainWindow;

 	//--------------------------------------------------------------
	//Pins
	//--------------------------------------------------------------

public:
    /** @brief Constructeur */
    _SwGuiQActionToWidget();
    /** @brief Destructeur */
    virtual ~_SwGuiQActionToWidget();
    /*! \brief Initialisation du composant*/
    virtual void initializeComponent() throw(SwException);
    /*! \brief evenement de changement de propriete*/
    virtual void eventPropertyChange(ISwProperty * property);
    /*! \brief evenement avant changement de la disponibilité de l'interface*/
    virtual void eventBeforeInterfaceAvailability(QString interface_name,SwComponent_Class * provider_host);
    /*! \brief evenement apres changement de la disponibilité de l'interface*/
    virtual void eventAfterInterfaceAvailability(QString interface_name,SwComponent_Class * provider_host);

	/*! \brief Renvoie le Action
	\return le Action */
	QAction & GetAction() ;

private:
	QAction _action;
	QWidget * _widget;
	_SwContainerCloseableWidget * _hostWidget;//création d'un conteneur qui encapsule la widget pour capter l'event close widget
	QString _showName;
	QString _hiddenName;
	bool _isVisible;
	bool _stayOnTop;

	bool isMoved;
	void MoveCenter();//déplace la widget au centre de l'écran

};

#endif
