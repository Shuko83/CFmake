#include "SwEventToPopup.h" 
#include "..\src\gui\dialogs\qmessagebox.h"

SwEventToPopup::SwEventToPopup()
{
	//SwAssistedComponent
	setConsumerServiceAvaibility(true);
	setProviderServiceAvaibility(true);

	//Interface
	_iSwEvent = NULL;	

	//Attribut
	_popupTitle = "Title";	
	_popupText = "Text";
}

SwEventToPopup::~SwEventToPopup()
{
	//---------------------------------------------------
	//				Gestion des Interfaces
	//---------------------------------------------------
	unconsummeInterface("ISwEvent");
}

void SwEventToPopup::initializeComponent() throw(SwException)
{
	//---------------------------------------------------
	//				Gestion des Interfaces
	//---------------------------------------------------
	consummeInterface<ISwEvent>("ISwEvent");

	createPropertiesForThisObject(QString(),true);
}

//----------------------------------------------------
// Interface de consomation d'interfaces
//----------------------------------------------------
void SwEventToPopup::interfaceAvailable( QString interface_name )
{
	if(interface_name == "ISwEvent")
	{
		_iSwEvent = getInterface<ISwEvent>("ISwEvent");
		_iSwEvent->addObserver(this);
		qDebug() << "The interface ISwEventObservable is available.";
	}
}

void SwEventToPopup::interfaceUnavailable( QString interface_name )
{
	if(interface_name == "ISwEvent")
	{
		_iSwEvent->removeObserver(this);
		qDebug() << "The interface ISwEventObservable is unavailable.";
	}
}

//----------------------------------------------------
// Interface ISwEvent
//----------------------------------------------------
void SwEventToPopup::onEvent( QEvent * event )
{
	if (event->type() == _eventType)
	{
		QMessageBox msgBox;
		msgBox.setWindowIcon(_popupIcon);
		msgBox.setWindowTitle(_popupTitle);
		msgBox.setText(_popupText);
		msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
		msgBox.setDefaultButton(QMessageBox::Ok);

		switch (msgBox.exec()) 
		{ 
		case QMessageBox::Cancel:
			event->ignore();
			break;
		case QMessageBox::Ok:
		default:
			event->accept();
			break;
		}
	}
}



//----------------------------------------------------
// Accesseur 
//----------------------------------------------------
QString SwEventToPopup::getPopupTitle() const
{
	return _popupTitle;
}

void SwEventToPopup::setPopupTitle( const QString val )
{
	_popupTitle = val;
}

QString SwEventToPopup::getPopupText() const
{
	return _popupText;
}

void SwEventToPopup::setPopupText( const QString val )
{
	_popupText = val;
}

QEvent::Type SwEventToPopup::getEventType() const
{
	return _eventType;
}

void SwEventToPopup::setEventType( const QEvent::Type val )
{
	_eventType = val;
}

QIcon SwEventToPopup::getPopupIcon() const
{
	return _popupIcon;
}

void SwEventToPopup::setPopupIcon( const QIcon val )
{
	_popupIcon = val;
}

//----------------------------------------------------
// Methode 
//----------------------------------------------------
