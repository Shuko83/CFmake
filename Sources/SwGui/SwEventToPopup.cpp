#include "SwEventToPopup.h" 
#include <QMessagebox.h>

SwEventToPopup::SwEventToPopup()
{
	//SwAssistedComponent
	setConsumerServiceAvaibility(true);
	setProviderServiceAvaibility(true);

	//Interface
	_iSwEvent = NULL;	
	_iSwMainWindow = NULL;

	//Attribut
	_popupTitle = "Title";	
	_popupText = "Text";
	_eventButtonConcelVisible = true;
}

SwEventToPopup::~SwEventToPopup()
{
	//---------------------------------------------------
	//				Gestion des Interfaces
	//---------------------------------------------------
	unconsumeInterface("ISwEvent");
	unconsumeInterface("ISwQMainWindow");
}

void SwEventToPopup::initializeComponent() throw(SwException)
{
	//---------------------------------------------------
	//				Gestion des Interfaces
	//---------------------------------------------------
	consumeInterface<ISwEvent>("ISwEvent");
	consumeInterface<ISwQMainWindow>("ISwQMainWindow");

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
	}

	if(interface_name == "ISwQMainWindow")
	{
		_iSwMainWindow = getInterface<ISwQMainWindow>("ISwQMainWindow");
	}
}

void SwEventToPopup::interfaceUnavailable( QString interface_name )
{
	if(interface_name == "ISwEvent")
	{
		_iSwEvent->removeObserver(this);
	}
	if(interface_name == "ISwQMainWindow")
	{
		_iSwMainWindow = NULL;
	}
}

//----------------------------------------------------
// Interface ISwEvent
//----------------------------------------------------
void SwEventToPopup::onEvent( QEvent * event )
{
	if (event->type() == _eventType)
	{
		QMessageBox msgBox((_iSwMainWindow!=NULL)?&(_iSwMainWindow->GetMainWindow()):NULL);
		msgBox.setWindowIcon(_popupIcon);
		msgBox.setWindowTitle(_popupTitle);
		msgBox.setText(_popupText);
		msgBox.setStandardButtons(QMessageBox::Ok | ((_eventButtonConcelVisible)?QMessageBox::Cancel:QMessageBox::Ok));
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

bool SwEventToPopup::getEventButtonConcelVisible() const
{
	return _eventButtonConcelVisible;
}

void SwEventToPopup::setEventButtonConcelVisible( const bool val )
{
	_eventButtonConcelVisible = val;
}

//----------------------------------------------------
// Methode 
//----------------------------------------------------
