#include "ManageLinkColor.h"

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

	//To do SQlite
}

//-------------------------------------------------------------------------
QColor ManageLinkColor::getColorForInterface( QString interfaceName )
{
	QString hexColor = "#FF9900";

	if( interfaceName.contains("ISwWidget") )
		hexColor = "#9E6DFF";
	else
		if(interfaceName.contains( "ISxModel") )
			hexColor = "#40e1e7";
		else
			if(interfaceName.contains( "ISwAction") )
				hexColor = "#aae740";
			else
				if(interfaceName.contains("ISwToolBar") )
					hexColor = "#e7408b";
				else
					if(interfaceName.contains("ISwDockWidget") )
						hexColor = "#fdff55";

	return QColor(hexColor);
}
