#include "ManageColor.h"

/**
/*Include QT 
*/
#include "ManageSQL.h"



// Initialisation du singleton à NULL
ManageColor *ManageColor::m_singleton = NULL;

//-------------------------------------------------------------------
ManageColor::ManageColor( void )
{
	if(ManageSQL::getInstance())
	{
		connect(ManageSQL::getInstance(),SIGNAL(connectionStateChange()),this,SLOT(colorChangeStatus()));
	}
}

//-------------------------------------------------------------------
ManageColor::~ManageColor(void)
{
	
}

//-------------------------------------------------------------------
ManageColor* ManageColor::getInstance ()
{
	if (m_singleton  == NULL)
		m_singleton = new ManageColor;

	ManageSQL::getInstance()->isOpen();

	return m_singleton;
}

//-------------------------------------------------------------------
void ManageColor::kill()
{
	if (NULL != m_singleton)
	{
		delete m_singleton;
		m_singleton = NULL;
	}
}

//-------------------------------------------------------------------------
QMap<QString,QString> ManageColor::getColor()
{
	if(ManageSQL::getInstance()->isOpen() && _map.isEmpty())
	{
		//get all color from database
		QList<TColor*> listColor = ManageSQL::getInstance()->getOrmObject<TColor>();
		foreach(TColor* color, listColor)
		{
			_map.insert(color->getInterfaceName(),color->getColor());
		}
	}

	return _map;
}

//-------------------------------------------------------------------------
void ManageColor::colorChangeStatus()
{
	emit dbLoaded();
}

