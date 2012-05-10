#include "ManageLinkColor.h"
#include "..\swdoc\ManageColor.h"

ManageLinkColor * ManageLinkColor::_instance = NULL;


ManageLinkColor::ManageLinkColor(void)
{
}

ManageLinkColor::~ManageLinkColor(void)
{
}

//-------------------------------------------------------------------------
ManageLinkColor * ManageLinkColor::getInstance()
{
	if(_instance == NULL)
	{
		_instance = new ManageLinkColor();
		_instance->loadColor();
	}
	return _instance;
}

//-------------------------------------------------------------------------
void ManageLinkColor::loadColor()
{
	//LoadColor
	if(ManageColor::getInstance())
	{
		_mapColor = ManageColor::getInstance()->getColor();
		connect(ManageColor::getInstance(),SIGNAL(dbLoaded()),this,SLOT(onDbStatusChange()));
	}

	loadStaticColor();
}

//-------------------------------------------------------------------------
QColor ManageLinkColor::getColorForInterface( QString interfaceName )
{
	QString hexColor = "#FF9900";
	
	QMapIterator<QString,QString> it(_mapColor);
	while(it.hasNext())
	{
		it.next();
		if(interfaceName.contains(it.key()))
		{
			return QColor(it.value());
		}
	}
	return QColor(hexColor);
	

}

//-------------------------------------------------------------------------
void ManageLinkColor::onDbStatusChange()
{
	_mapColor = ManageColor::getInstance()->getColor();
	loadStaticColor();
	emit colorHasChange();
}

//-----------------------------------------------------------------------
void ManageLinkColor::loadStaticColor()
{
	if(_mapColor.isEmpty())
	{
		_mapColor.insert("ISwWidget","#9E6DFF");
		_mapColor.insert("ISxModel","#40E1E7");
		_mapColor.insert("ISwAction","#AAE740");
		_mapColor.insert("ISwToolBar","#E7408B");
		_mapColor.insert("ISwDockWidget","#FDFF55");
		_mapColor.insert("ISwLayout","#032F55");
	}
}
