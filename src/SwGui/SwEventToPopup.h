/*!
\file SwEventToPopup.h
\brief 
\version 1.0
\author AURELIE.TRAUTMANN
\date 25/10/2011 - 11:43
*/

#ifndef _SwEventToPopup_H
#define _SwEventToPopup_H


/*
* INCLUDES LOCAUX
*/
#include "SwAssistedComponent.h"
#include <QEvent.h>
#include <QIcon.h>
#include "ISwEventObserver.h"
#include "ISwEvent.h"
#include "ISwQMainWindow.h"


using namespace StreamWork::SwFoundation;


/**
*	@class SwEventToPopup 
*	@brief 
*/               
class SwEventToPopup : virtual public SwAssistedComponent, virtual public ISwEventObserver
{
	Q_OBJECT

		Q_PROPERTY(QEvent::Type EventType READ getEventType WRITE setEventType)
		Q_PROPERTY(bool EventButtonCancelVisible READ getEventButtonConcelVisible WRITE setEventButtonConcelVisible)
		Q_PROPERTY(QIcon PopupIcon READ getPopupIcon WRITE setPopupIcon)
		Q_PROPERTY(QString PopupTitle READ getPopupTitle WRITE setPopupTitle)
		Q_PROPERTY(QString PopupText READ getPopupText WRITE setPopupText)

public:

	/*! \brief Constructeur */
	SwEventToPopup();

	/*! \brief Destructeur */
	virtual ~SwEventToPopup();

	/**
	* @brief    : Initialisation du composant
	* @note	 : A surcharger
	*/
	virtual void initializeComponent() throw(SwException);


	//----------------------------------------------------
	// Interface de consomation d'interfaces
	//----------------------------------------------------

	/**
	* @brief    : Callback appel?e lors de la disponibilit? de l'interface
	* @param	 : QString interfaceName - Nom de l'interface
	*/
	virtual void interfaceAvailable(QString interface_name);

	/**
	* @brief    : Callback appel?e lors de l'indisponibilit? de l'interface
	* @param	 : QString interfaceName - Nom de l'interface
	*/
	virtual void interfaceUnavailable(QString interface_name);

	//----------------------------------------------------
	// Interface ISwEvent
	//----------------------------------------------------
	virtual void onEvent ( QEvent * event );  


	//----------------------------------------------------
	// Accesseur 
	//----------------------------------------------------
	QString getPopupTitle() const;
	void setPopupTitle(const QString val);
	QString getPopupText() const;
	void setPopupText(const QString val);
	QEvent::Type getEventType() const;
	void setEventType(const QEvent::Type val);
	QIcon getPopupIcon() const;
	void setPopupIcon(const QIcon val);
	bool getEventButtonConcelVisible() const;
	void setEventButtonConcelVisible(const bool val);
protected:

	//---------------------------------------------------
	//				Declaration des interfaces
	ISwEvent* _iSwEvent;								
	ISwQMainWindow* _iSwMainWindow;


private:
	QIcon _popupIcon;	
	QString _popupTitle;	
	QString _popupText;
	QEvent::Type _eventType;
	bool _eventButtonConcelVisible;
	
};
#endif 
